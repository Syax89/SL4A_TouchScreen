#!/usr/bin/env python3
"""Lock the Secure Boot installer contract to the identity DKMS actually uses."""

from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
INSTALLER = ROOT / "tools" / "sl4a-touch.sh"
src = INSTALLER.read_text(encoding="utf-8")

required = {
    "MOK resolver": "resolve_dkms_mok_paths()",
    "Ubuntu signing-key default": 'mok_signing_key="/var/lib/shim-signed/mok/MOK.priv"',
    "Ubuntu certificate default": 'mok_certificate="/var/lib/shim-signed/mok/MOK.der"',
    "upstream signing-key default": 'mok_signing_key="/var/lib/dkms/mok.key"',
    "upstream certificate default": 'mok_certificate="/var/lib/dkms/mok.pub"',
    "DKMS framework config": "/etc/dkms/framework.conf",
    "DKMS framework drop-ins": "/etc/dkms/framework.conf.d/*.conf",
    "pair validator": "mok_pair_paths_match()",
    "private-key validation": 'openssl pkey -in "$key" -passin pass: -noout',
    "certificate validation": 'openssl x509 -in "$cert" -inform DER -noout',
    "public-key match": "openssl dgst -sha256",
    "rotation flag": "--rotate-mok",
    "backup directory": "/var/lib/dkms/sl4a-mok-backup-",
    "existing pair reuse": "Reusing the existing DKMS signing key pair",
    "actual-path assertion": "paths DKMS will use",
    "system-wide rotation warning": "can affect every DKMS module on the system",
    "alternate pair import": "Use another existing key pair",
    "incomplete material refusal": "MOK material is incomplete/invalid",
    "final pair invariant": "signing identity is not a complete matching key pair after setup",
    "enrollment test": 'mokutil --test-key "$MOK_CERT"',
    "installed signature verification": 'modinfo -F signer "$signed_module"',
    "override mismatch refusal": "differ from DKMS mok_signing_key/mok_certificate",
}

missing = [name for name, needle in required.items() if needle not in src]
if missing:
    raise SystemExit("secure boot installer contract missing: " + ", ".join(missing))

if 'if [ ! -f /var/lib/dkms/mok.pub ]; then' in src:
    raise SystemExit("certificate-only MOK existence check has returned")

activate = src.split("cmd_activate() {", 1)[1].split("\n\tlocal controllers=", 1)[0]
activate_required = {
    "distro detection before MOK resolution": "detect_distro",
    "active DKMS identity resolution": "resolve_dkms_mok_paths",
    "resolved certificate enrollment test": 'mokutil --test-key "$MOK_CERT"',
    "resolved certificate import": 'mokutil --import "$MOK_CERT"',
}
activate_missing = [
    name for name, needle in activate_required.items() if needle not in activate
]
if activate_missing:
    raise SystemExit(
        "Secure Boot activate contract missing: " + ", ".join(activate_missing)
    )
if "/var/lib/dkms/mok.pub" in activate or "/var/lib/dkms/mok.key" in activate:
    raise SystemExit(
        "activate must not hard-code upstream DKMS MOK paths; use the resolved identity"
    )

help_block = src.split("usage() {", 1)[1].split("\n}", 1)[0]
if "--rotate-mok" not in help_block:
    raise SystemExit("--rotate-mok is not exposed by executable help")
if "shared DKMS signing identity" not in help_block:
    raise SystemExit("--rotate-mok help must explain its system-wide DKMS effect")

if 're-run deliberately with --rotate-mok' not in src:
    raise SystemExit("non-interactive incomplete-key safety contract missing")

if "umask 077" in src:
    raise SystemExit("MOK generation must not change the installer's process-wide umask")


def extract_shell_function(name: str) -> str:
    marker = name + "() {"
    start = src.find(marker)
    if start < 0:
        raise SystemExit(f"cannot locate {name}()")
    brace = src.find("{", start)
    depth = 0
    for i in range(brace, len(src)):
        ch = src[i]
        if ch == "{":
            depth += 1
        elif ch == "}":
            depth -= 1
            if depth == 0:
                return src[start:i + 1]
    raise SystemExit(f"cannot isolate {name}()")


# Behavioral identity-selection probe. Use the actual resolver source but
# redirect its DKMS configuration reads into a temporary tree so the test is
# deterministic and never depends on the CI host's /etc/dkms state.
resolver = extract_shell_function("resolve_dkms_mok_paths")

with tempfile.TemporaryDirectory(prefix="sl4a-mok-resolver-") as td:
    td = Path(td)
    etc = td / "etc" / "dkms"
    drop = etc / "framework.conf.d"
    drop.mkdir(parents=True)
    framework = etc / "framework.conf"
    framework.write_text(
        'mok_signing_key="/tmp/pairB.key"\n'
        'mok_certificate="/tmp/pairB.der"\n',
        encoding="utf-8",
    )

    # Replace the common prefix once; replacing the glob first then the
    # prefix nested the fixture directory twice, silently bypassing drop-ins.
    probe_resolver = resolver.replace(
        "/etc/dkms/framework.conf", str(framework),
    )

    common = f"""
set -e
unset SL4A_MOK_KEY SL4A_MOK_CERT
fail() {{ echo "FAIL:$*" >&2; exit 97; }}
info() {{ :; }}
ID=debian
MOK_KEY=
MOK_CERT=
{probe_resolver}
"""

    # No override: the installer must resolve exactly the pair DKMS config says
    # it will use.
    p = subprocess.run(
        ["bash", "-c", common + '\nresolve_dkms_mok_paths\nprintf "%s|%s\\n" "$MOK_KEY" "$MOK_CERT"\n'],
        text=True,
        capture_output=True,
    )
    if p.returncode != 0 or p.stdout.strip() != "/tmp/pairB.key|/tmp/pairB.der":
        raise SystemExit(
            "resolver did not select configured DKMS identity: "
            f"rc={p.returncode} out={p.stdout!r} err={p.stderr!r}"
        )

    # Distinct installer override A vs DKMS identity B must be rejected. This
    # is the exact pair-A/pair-B failure mode from review.
    p = subprocess.run(
        ["bash", "-c",
         common
         + '\nexport SL4A_MOK_KEY=/tmp/pairA.key\n'
           'export SL4A_MOK_CERT=/tmp/pairA.der\n'
           'resolve_dkms_mok_paths\n'],
        text=True,
        capture_output=True,
    )
    if p.returncode != 97 or "differ from DKMS" not in p.stderr:
        raise SystemExit(
            "distinct A/B MOK identities were not rejected: "
            f"rc={p.returncode} out={p.stdout!r} err={p.stderr!r}"
        )

    # An override that merely repeats DKMS's configured pair is harmless.
    p = subprocess.run(
        ["bash", "-c",
         common
         + '\nexport SL4A_MOK_KEY=/tmp/pairB.key\n'
           'export SL4A_MOK_CERT=/tmp/pairB.der\n'
           'resolve_dkms_mok_paths\n'
           'printf "%s|%s\\n" "$MOK_KEY" "$MOK_CERT"\n'],
        text=True,
        capture_output=True,
    )
    if p.returncode != 0 or p.stdout.strip() != "/tmp/pairB.key|/tmp/pairB.der":
        raise SystemExit(
            "matching override/configured DKMS identity was not accepted: "
            f"rc={p.returncode} out={p.stdout!r} err={p.stderr!r}"
        )

    # DKMS sources drop-ins in lexical order after framework.conf.
    # A real drop-in overrides the base B paths with pair C.
    override = drop / "90-identity.conf"
    override.write_text(
        'mok_signing_key="/tmp/pairC.key"\n'
        'mok_certificate="/tmp/pairC.der"\n', encoding="utf-8"
    )

    # Last sourced drop-in must take precedence; explicitly assert C.
    p = subprocess.run(
        ["bash", "-c",
         common + '\nresolve_dkms_mok_paths\n'
                  'printf "%s|%s\\n" "$MOK_KEY" "$MOK_CERT"\n'],
        text=True, capture_output=True,
    )
    if p.returncode != 0 or p.stdout.strip() != "/tmp/pairC.key|/tmp/pairC.der":
        raise SystemExit(
            "DKMS drop-in precedence regression: "
            f"rc={p.returncode} out={p.stdout!r} err={p.stderr!r}"
        )


subprocess.run(["bash", "-n", str(INSTALLER)], check=True)
print("secure boot installer source/behavior test: PASS")
