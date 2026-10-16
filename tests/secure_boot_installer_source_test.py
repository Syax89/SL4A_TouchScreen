#!/usr/bin/env python3
"""Lock the Secure Boot installer contract to complete, matching MOK pairs."""

from pathlib import Path
import subprocess

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
    "system-wide rotation warning": "affects all DKMS modules",
    "alternate pair import": "Use another existing key pair",
    "incomplete material refusal": "MOK material is incomplete/invalid",
    "final pair invariant": "signing identity is not a complete matching key pair after setup",
    "enrollment test": 'mokutil --test-key "$MOK_CERT"',
    "installed signature verification": 'modinfo -F signer "$signed_module"',
}

missing = [name for name, needle in required.items() if needle not in src]
if missing:
    raise SystemExit("secure boot installer contract missing: " + ", ".join(missing))

if 'if [ ! -f /var/lib/dkms/mok.pub ]; then' in src:
    raise SystemExit("certificate-only MOK existence check has returned")

if 'MOK_KEY="${SL4A_MOK_KEY:-/var/lib/dkms/mok.key}"' in src:
    raise SystemExit("installer must not hard-code the upstream DKMS MOK path")

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

if 're-run deliberately with --rotate-mok' not in src:
    raise SystemExit("non-interactive incomplete-key safety contract missing")

if "umask 077" in src:
    raise SystemExit("MOK generation must not change the installer's process-wide umask")

subprocess.run(["bash", "-n", str(INSTALLER)], check=True)
print("secure boot installer source test: PASS")
