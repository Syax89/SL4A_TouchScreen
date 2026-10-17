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
    "Zorin fail-closed refusal": "Refusing to assume DKMS MOK signing paths on Zorin",
    "incomplete-pair refusal": "DKMS configuration sets only one of",
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


def redirected_resolver(root: Path) -> str:
    """Point the resolver's DKMS config reads at <root>/etc/dkms.

    The single prefix replacement covers both the framework.conf read and the
    framework.conf.d glob (".../framework.conf.d/*.conf") — replacing the glob
    separately after the prefix nested the fixture directory twice, which
    silently bypassed drop-ins.
    """
    etc = root / "etc" / "dkms"
    (etc / "framework.conf.d").mkdir(parents=True, exist_ok=True)
    (etc / "framework.conf").write_text("", encoding="utf-8")
    return resolver.replace(
        "/etc/dkms/framework.conf", str(etc / "framework.conf"),
    )


with tempfile.TemporaryDirectory(prefix="sl4a-mok-resolver-") as td:
    td = Path(td)
    etc = td / "etc" / "dkms"
    drop = etc / "framework.conf.d"
    framework = etc / "framework.conf"
    probe_resolver = redirected_resolver(td)
    framework.write_text(
        'mok_signing_key="/tmp/pairB.key"\n'
        'mok_certificate="/tmp/pairB.der"\n',
        encoding="utf-8",
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


# ── Zorin fail-closed contract ───────────────────────────────────────────────
# Zorin is in detect_distro()'s ID_LIKE=debian allowlist but not in the
# shim-signed allowlist inside the resolver, so it used to fall through to
# /var/lib/dkms — while packaged Zorin DKMS (Noble 3.0.11-1ubuntu13) works out
# its distro from ID_LIKE='ubuntu debian' and signs with the shim-signed MOK
# pair. The installer and DKMS then disagree about the signing identity. No
# per-release inventory of packaged defaults is verified here, so the resolver
# must refuse to invent one: only an explicitly configured, complete pair may
# pass, and nothing else (partial config, installer-only overrides) may sneak
# past the refusal. These cases drive the real resolver with its DKMS config
# reads redirected into a scratch tree.

ZORIN_REFUSAL = "Refusing to assume DKMS MOK signing paths on Zorin"


def run_resolver(ident, framework_text="", dropin_text=None, env=None):
    """Run the real resolver against a scratch /etc/dkms and report the pair."""
    with tempfile.TemporaryDirectory(prefix="sl4a-mok-zorin-") as rd:
        root = Path(rd)
        probe = redirected_resolver(root)
        etc = root / "etc" / "dkms"
        (etc / "framework.conf").write_text(framework_text, encoding="utf-8")
        if dropin_text is not None:
            (etc / "framework.conf.d" / "90-identity.conf").write_text(
                dropin_text, encoding="utf-8"
            )
        # Overrides are exported after the unset so a stray value inherited
        # from the host environment can never decide the outcome.
        exports = "".join(
            f'export {name}="{value}"\n' for name, value in (env or {}).items()
        )
        script = f"""
set -e
unset SL4A_MOK_KEY SL4A_MOK_CERT
{exports}fail() {{ echo "FAIL:$*" >&2; exit 97; }}
info() {{ :; }}
ID={ident}
MOK_KEY=
MOK_CERT=
{probe}
resolve_dkms_mok_paths
printf "%s|%s\\n" "$MOK_KEY" "$MOK_CERT"
"""
        return subprocess.run(
            ["bash", "-c", script], text=True, capture_output=True
        )


def expect_refusal(case, **kwargs):
    p = run_resolver(**kwargs)
    if p.returncode != 97 or ZORIN_REFUSAL not in p.stderr:
        raise SystemExit(
            f"Zorin refusal missing for {case}: "
            f"rc={p.returncode} out={p.stdout!r} err={p.stderr!r}"
        )


def expect_pair(case, pair, **kwargs):
    p = run_resolver(**kwargs)
    if p.returncode != 0 or p.stdout.strip() != pair:
        raise SystemExit(
            f"Zorin resolver did not honor {case}: "
            f"rc={p.returncode} out={p.stdout!r} err={p.stderr!r}"
        )


PAIR_A = 'mok_signing_key="/tmp/zorinA.key"\nmok_certificate="/tmp/zorinA.der"\n'

# No DKMS configuration at all: refuse instead of assuming a default.
expect_refusal("zorin with no DKMS MOK config", ident="zorin")

# Half a pair is still not a verified identity.
expect_refusal(
    "zorin with only mok_signing_key configured",
    ident="zorin",
    framework_text='mok_signing_key="/tmp/zorinA.key"\n',
)
expect_refusal(
    "zorin with only mok_certificate configured",
    ident="zorin",
    framework_text='mok_certificate="/tmp/zorinA.der"\n',
)

# Installer-side environment overrides are not DKMS configuration: they must
# not be able to satisfy (or bypass) the refusal.
expect_refusal(
    "zorin with installer-only SL4A override and no DKMS config",
    ident="zorin",
    env={"SL4A_MOK_KEY": "/tmp/zorinA.key", "SL4A_MOK_CERT": "/tmp/zorinA.der"},
)
expect_refusal(
    "zorin with partial DKMS config completed only by an installer override",
    ident="zorin",
    framework_text='mok_signing_key="/tmp/zorinA.key"\n',
    env={"SL4A_MOK_KEY": "/tmp/zorinA.key", "SL4A_MOK_CERT": "/tmp/zorinA.der"},
)

# An explicit, complete DKMS pair passes — framework.conf and drop-in alike.
expect_pair(
    "explicit framework.conf pair", "/tmp/zorinA.key|/tmp/zorinA.der",
    ident="zorin", framework_text=PAIR_A,
)
expect_pair(
    "explicit framework.conf.d drop-in pair", "/tmp/zorinA.key|/tmp/zorinA.der",
    ident="zorin", dropin_text=PAIR_A,
)

# Divergent installer-side identity is still rejected when DKMS is configured.
p = run_resolver(
    ident="zorin",
    framework_text=PAIR_A,
    env={"SL4A_MOK_KEY": "/tmp/zorinB.key", "SL4A_MOK_CERT": "/tmp/zorinB.der"},
)
if p.returncode != 97 or "differ from DKMS" not in p.stderr:
    raise SystemExit(
        "Zorin divergent SL4A override was not rejected: "
        f"rc={p.returncode} out={p.stdout!r} err={p.stderr!r}"
    )

# Other distros keep their existing implicit defaults.
expect_pair(
    "unchanged upstream default",
    "/var/lib/dkms/mok.key|/var/lib/dkms/mok.pub",
    ident="debian",
)
expect_pair(
    "unchanged shim-signed default",
    "/var/lib/shim-signed/mok/MOK.priv|/var/lib/shim-signed/mok/MOK.der",
    ident="ubuntu",
)


# ── Partial DKMS configuration (Noble DKMS 3.0.11 framework.conf) ────────────
# A framework.conf (or drop-in) that sets only ONE of mok_signing_key /
# mok_certificate is not a resolved identity: DKMS keeps the configured half and
# works out the other one itself, by rules that differ between DKMS versions and
# packagings. The resolver used to seed the distro defaults BEFORE sourcing the
# config, so a partial config produced a hybrid pair (one configured half plus
# one distro-default half) that DKMS may never sign with — the installer would
# then validate/enroll a certificate that does not match the modules. Both paths
# must therefore start empty, and an incomplete pair must be refused instead of
# completed by either a distro default or an SL4A_* override.

PARTIAL_REFUSAL = "DKMS configuration sets only one of"
SHIM_PAIR = "/var/lib/shim-signed/mok/MOK.priv|/var/lib/shim-signed/mok/MOK.der"
UBUNTU_A = (
    'mok_signing_key="/tmp/ubuntuA.key"\nmok_certificate="/tmp/ubuntuA.der"\n'
)


def expect_partial_refusal(case, **kwargs):
    p = run_resolver(**kwargs)
    if p.returncode != 97 or PARTIAL_REFUSAL not in p.stderr:
        raise SystemExit(
            f"partial DKMS config was not refused for {case}: "
            f"rc={p.returncode} out={p.stdout!r} err={p.stderr!r}"
        )


# Ubuntu is the distro whose packaged default is the shim-signed pair, so a
# partial config there used to yield half shim-signed + half configured.
expect_partial_refusal(
    "ubuntu framework.conf with only mok_signing_key",
    ident="ubuntu",
    framework_text='mok_signing_key="/tmp/ubuntuA.key"\n',
)
expect_partial_refusal(
    "ubuntu framework.conf with only mok_certificate",
    ident="ubuntu",
    framework_text='mok_certificate="/tmp/ubuntuA.der"\n',
)
# Explicitly emptying the other half is still a one-half config, not a request
# for the distro default pair.
expect_partial_refusal(
    "ubuntu framework.conf with key set and certificate explicitly emptied",
    ident="ubuntu",
    framework_text='mok_signing_key="/tmp/ubuntuA.key"\nmok_certificate=""\n',
)
expect_partial_refusal(
    "ubuntu framework.conf with certificate set and key explicitly emptied",
    ident="ubuntu",
    framework_text='mok_certificate="/tmp/ubuntuA.der"\nmok_signing_key=""\n',
)
# Same failure through a drop-in, with no framework.conf content at all.
expect_partial_refusal(
    "ubuntu drop-in with only mok_signing_key",
    ident="ubuntu",
    dropin_text='mok_signing_key="/tmp/ubuntuA.key"\n',
)
expect_partial_refusal(
    "ubuntu drop-in with only mok_certificate",
    ident="ubuntu",
    dropin_text='mok_certificate="/tmp/ubuntuA.der"\n',
)
# A drop-in that empties one half of an otherwise complete framework.conf is
# also incomplete once sourced, in lexical order.
expect_partial_refusal(
    "ubuntu complete framework.conf emptied by a later drop-in",
    ident="ubuntu",
    framework_text=UBUNTU_A,
    dropin_text='mok_certificate=""\n',
)
# Installer-side overrides cannot complete an incomplete DKMS configuration:
# DKMS does not read SL4A_MOK_KEY/SL4A_MOK_CERT, so supplying both here would
# only make the installer validate a pair DKMS is not signing with.
expect_partial_refusal(
    "ubuntu partial config completed only by an SL4A override",
    ident="ubuntu",
    framework_text='mok_signing_key="/tmp/ubuntuA.key"\n',
    env={"SL4A_MOK_KEY": "/tmp/ubuntuA.key", "SL4A_MOK_CERT": "/tmp/ubuntuA.der"},
)
# Debian/upstream default is /var/lib/dkms, but a partial config is just as
# unresolved there.
expect_partial_refusal(
    "debian framework.conf with only mok_certificate",
    ident="debian",
    framework_text='mok_certificate="/tmp/pairA.der"\n',
)

# Controls: a COMPLETE pair from the configuration is honored unchanged,
# including a drop-in that supplies the pair and one that supplies the half
# framework.conf omitted (both halves configured => resolved).
expect_pair(
    "ubuntu complete framework.conf pair",
    "/tmp/ubuntuA.key|/tmp/ubuntuA.der",
    ident="ubuntu", framework_text=UBUNTU_A,
)
expect_pair(
    "ubuntu complete drop-in pair",
    "/tmp/ubuntuA.key|/tmp/ubuntuA.der",
    ident="ubuntu", dropin_text=UBUNTU_A,
)
expect_pair(
    "ubuntu pair split across framework.conf and drop-in",
    "/tmp/ubuntuA.key|/tmp/ubuntuA.der",
    ident="ubuntu",
    framework_text='mok_signing_key="/tmp/ubuntuA.key"\n',
    dropin_text='mok_certificate="/tmp/ubuntuA.der"\n',
)
# Both options explicitly empty is "nothing configured", so the distro default
# still applies (the pre-existing behavior must not regress into a refusal).
expect_pair(
    "ubuntu both options explicitly empty uses distro default",
    SHIM_PAIR,
    ident="ubuntu",
    framework_text='mok_signing_key=""\nmok_certificate=""\n',
)
expect_pair(
    "debian both options explicitly empty uses distro default",
    "/var/lib/dkms/mok.key|/var/lib/dkms/mok.pub",
    ident="debian",
    dropin_text='mok_signing_key=""\nmok_certificate=""\n',
)


subprocess.run(["bash", "-n", str(INSTALLER)], check=True)
print("secure boot installer source/behavior test: PASS")
