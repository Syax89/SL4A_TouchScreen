#!/usr/bin/env python3
"""Pin the installer's failure-path contract (adversarial reviews R12/R14/R16).

These are the strings and code shapes the fix passes keep touching: the
diagnostic bundle's write check, the `-o` validation, the DKMS version parse
(both real `dkms status` shapes) and the ownership markers. A revert of any of
them must fail here — the shell suite alone never noticed (review R16 found that
none of the round-3 corrections was pinned by any test).
"""

from pathlib import Path

root = Path(__file__).parents[1]
tool = (root / "tools" / "sl4a-touch.sh").read_text()

# Diagnostic bundle: a failed redirect or a write that stopped early must be
# reported, never glossed over with "Diagnostic bundle written" for a file that
# nothing wrote (a stale bundle from a previous run is non-empty too).
assert "bundle_status=$?" in tool
assert "set +e +o pipefail" in tool          # pipefail off for the block, or a
assert "set -e -o pipefail" in tool          # dmesg|grep miss fails a good bundle
# The -o path must be quoted at the call site too: an unquoted `${VAR:+...}` is
# word-split, so "logs -o my file.txt" reached the child as three arguments and
# was rejected as an unknown option (review R3-F4).
assert 'logs -o "$OUT"' in tool
assert 'logs ${OUT:+-o' not in tool
assert 'if [ "$bundle_status" -ne 0 ] || [ ! -s "$OUT" ] || \\' in tool
assert "! grep -q '^--- dmesg' \"$OUT\"" in tool
assert "the diagnostic bundle could not be written to" in tool

# `-o`: symlink, non-regular file, foreign file and empty path are all refused,
# and the "is it ours" test looks at the first line only.
assert "refusing to write the bundle through the symlink" in tool
assert "refusing to overwrite $OUT: not a regular file" in tool
assert "it is not a diagnostic bundle" in tool
assert '"-o requires a non-empty path"' in tool
assert 'head -n 1 "$OUT"' in tool

# DKMS cleanup: both real `dkms status` shapes parse the same way, a legacy
# 2.x line matches nothing, and only a plausible version can reach
# `dkms remove -v` and the `rm -rf` that follows it.
assert "printf '%s\\n' \"$line\" | sed -n" in tool
assert "''|*[!A-Za-z0-9.+~_-]*) continue ;;" in tool
assert tool.count("^PACKAGE_NAME=\"sl4a-touch\"[[:space:]]*$") >= 3

# A stale registration that survives must be loud: it can win the next kernel
# update, which is the whole reason the cleanup exists.
assert "stays registered and can win the next kernel update" in tool

# Every menu tput is failure-tolerant under `set -e`: a TERM without
# cuu/ed/cnorm used to kill the interactive flow mid-selection.
for seq in ("tput civis", "tput cuu", "tput ed", "tput cnorm"):
    for line in tool.splitlines():
        if line.strip().startswith(seq):
            assert line.rstrip().endswith("|| true"), line

# A pull that leaves VERSION unchanged must still rebuild: DKMS caches the built
# module per (module, version, kernel), so a plain `dkms build` answers "already
# built" and reinstalls the stale object — the loaded module then silently stops
# matching the checkout (found in the field: an install whose module carried
# none of the new attributes).
assert 'dkms build -m "$PKG_NAME" -v "$PKG_VERSION" --force' in tool
assert 'dkms install -m "$PKG_NAME" -v "$PKG_VERSION" --force' in tool

# ... and both must be reachable. An early skip when the DKMS version matched
# was what made the --force above dead code: after a pull, install reported
# success and left the previously built (stale) module in place.
assert "profile_only" not in tool, \
    "the version-match build skip is back; the --force rebuild below is unreachable again"
assert "already registered; rebuilding from this checkout" in tool, \
    "install no longer says it rebuilds a matching DKMS version"
# Scoped to the install path: hunt's rebuild helper also stages and builds, and
# a global index() then compared the helper's copy against install's branch.
# The claim is about install's sequence, so measure it inside install.
_install = tool[tool.index("cmd_install() {"):tool.index("cmd_uninstall() {")]
assert _install.index("already_added=1") < _install.index('dkms build -m "$PKG_NAME"'), \
    "the rebuild must follow the already-registered branch"
assert 'if [ "$already_added" -eq 0 ]; then' in tool, \
    "dkms add must stay conditional: it refuses an entry that already exists"

# ── round 6: the upgrade order, kernel-aware status, real boot-unit check ──
# An upgrade must not remove the working version until the new one has built
# AND installed: with the removal first, a failed build left the machine with
# no registered driver at all.
assert tool.index('dkms_remove_other_versions "$PKG_VERSION"') > \
    tool.index('dkms build -m "$PKG_NAME" -v "$PKG_VERSION" --force'), \
    "the old DKMS registration is removed before the new version is built"
assert tool.index('dkms_remove_other_versions "$PKG_VERSION"') > \
    tool.index('dkms install -m "$PKG_NAME" -v "$PKG_VERSION" --force'), \
    "the old DKMS registration is removed before the new version is installed"
# ... and removing it also deletes the shared /updates/dkms objects, so the new
# version is installed once more right after (DKMS do_uninstall removes the
# destination file both versions record).
assert tool.count('dkms install -m "$PKG_NAME" -v "$PKG_VERSION" --force') >= 2, \
    "nothing re-installs the new version after the old registration is dropped"
# ... and a failed build/install must not undo a version that was already
# registered and installed: VERSION is reused between commits, so the plain
# cleanup (`dkms remove --all`) would uninstall the working module.
assert "stage_failed() {" in tool
assert '[ "$already_added" -eq 1 ] || cleanup_staged_install' in tool, \
    "the failure path runs the unconditional cleanup again: an upgrade of the " \
    "same version then uninstalls the module that works"
assert "cleanup_staged_install; fail" not in tool, \
    "a failure path still calls the unconditional cleanup before failing"

# `dkms status` without -k answers for some other kernel's entry.
assert 'dkms status -m "$PKG_NAME" -k "$(uname -r)"' in tool, \
    "dkms_installed_version is not pinned to the running kernel"
assert "installed for kernel $kernel" in tool, \
    "status does not say which kernel the DKMS version belongs to"

# The boot-activation promise needs more than file state: is-enabled is true
# for a checkout that has since moved or been deleted (systemd fails the unit
# with 203/EXEC at every boot).
assert "boot_unit_loadable" in tool and "systemd-analyze verify" in tool, \
    "the enabled unit is never checked for loadability"
assert "is enabled but systemd cannot load it" in tool
assert "cannot load it (ExecStart points at" in tool, \
    "install must fail loudly instead of promising boot activation"

# A running module is not replaced by modprobe, so a completed activation can
# still leave the previous build answering: compare srcversion and say it.
assert "is NOT the build just installed" in tool
assert 'sudo modprobe -r sl4a-spi-hid sl4a-spi-amd && sudo ./tools/sl4a-touch.sh activate' in tool
assert '"/sys/module/$mod/srcversion"' in tool
# hunt: the bisect is one command, and the file it writes carries a verdict
# per variant — otherwise the diagnosis costs the user a shell session again.
# hunt must not sweep a module built before the last pull: reloading does not
# rebuild, and a sweep against a three-commit-old driver describes a revision
# that no longer exists. The stamp plus the rebuild branch is the guard.
assert "stamp_installed_head" in tool and "installed_head()" in tool, \
    "the installed-revision stamp is gone: nothing can tell a stale module"
assert 'if [ "$head_built" != "$head_now" ]' in tool, \
    "hunt no longer rebuilds a stale module before sweeping"
assert "restage_and_rebuild" in tool and 'cp -a "$DRIVER_DIR"/. "$SRC_DEST"/' in tool, \
    "the rebuild no longer re-stages the checkout: it would stamp the new " \
    "revision onto the old sources"

# A bundle must say which revision its figures describe: srcversion compares
# the loaded module with the file on disk and both can be stale together.
assert "Installed revision vs this checkout" in tool and "installed_head" in tool, \
    "the bundle no longer reports the installed revision"
assert "MISMATCH — the installed modules predate this checkout" in tool, \
    "a stale installed module no longer produces a warning in the bundle"
assert "run_host_self_tests" in tool and "suite result: PASS" in tool, \
    "the bundle no longer runs the host suite, so its result is not collected"
# The bug the user actually hit, and the reason hunt is now tested end to end:
# a comment whose second line lost its leading '#' became a command, so hunt
# died with rc=127 *after* unloading the driver — invisible, because everything
# past that point is redirected into the artifact. The sandbox test in
# hunt_sandbox_test.sh runs the real sweep against stubs; these pins keep the
# two mechanisms that make such a death impossible to miss.
assert "exec 3>&2" in tool, \
    "hunt lost its terminal fd: progress and errors would vanish into the file again"
assert ">\u00263" in tool or ">&3" in tool, \
    "nothing writes to the terminal fd during the sweep"
assert "trap 'rc=$?;" in tool and "hunt stopped at line $LINENO" in tool, \
    "the ERR trap is gone: a failing command inside the sweep is silent again"
assert "( cmd_activate >/dev/null 2>&1 ) || true" in tool, \
    "the final restore is not isolated in a subshell: cmd_activate's fail() \
     would exit the whole sweep after the artifact was already complete"
assert "|| true" in tool.split("modinfo sl4a_spi_hid")[1][:60], \
    "modinfo is unguarded again: on a machine where the module is not installed, \
     the sweep dies at the very end"

# ── the double-blind leg's findings ────────────────────────────────────────
# The worst regression of the whole campaign, and the last one standing: a fix
# of mine rewrote stamp_installed_head and swallowed restage_and_rebuild with
# it, so 'hunt' — the one command the user has to run — died with rc=127,
# "command not found", in exactly the case the fix chain exists for. bash -n
# cannot see it, the suite could not see it, and the pin that was supposed to
# protect hunt passed on the shipped tree while hunt was broken. A helper that
# is called but never defined is now a test failure.
for _fn in ("restage_and_rebuild", "quarantine_unowned", "run_host_self_tests",
            "stamp_installed_head", "installed_head", "hunt_verdict",
            "modprobe_profile", "cleanup_staged_install"):
    assert f"{_fn}() {{" in tool, \
        f"{_fn} is called but never defined — every caller dies with rc=127"
# Every call site must come after the definition, so a reordering cannot
# silently pick up a same-named command from the system.
assert tool.index("restage_and_rebuild() {") < tool.index("\t\trestage_and_rebuild\n"), \
    "restage_and_rebuild is used before it is defined"

# ── the installer review's findings (leg 2026-09-16) ───────────────────────
# F1: under `set -e -o pipefail` a dmesg|grep that matches nothing aborted hunt
# after it unloaded the driver and before it put the module back.
assert "| grep -i sl4a_spi_hid | tail -n 60 || true" in tool, \
    "the hunt dmesg pipeline is unguarded again: a variant with no log lines \
     aborts the sweep with the driver unloaded"
# F2: a glob that matches no panel made every counter unreadable, and the
# verdict then recorded "silent" — a measurement that was never taken.
assert "NO COUNTERS READ" in tool and "MSHW*" in tool, \
    "the hunt verdict can claim silence without having read a counter"
# F3: the suite runs inside a diagnostic path; it must not propagate failure.
assert "had_errexit" in tool, \
    "run_host_self_tests no longer respects the caller's errexit state"
# F4: -o fed a root redirect with none of the guards logs -o has.
assert "does not look like a diagnostic file of ours" in tool, \
    "hunt -o overwrites any path as root again"
# F5: the stamp redirect truncated before git ran, so a git refusal left an
# empty stamp and every bundle printed a false MISMATCH.
assert "INSTALLED_HEAD_STAMP.$$" in tool and '|| head=""' in tool, \
    "the revision stamp is written non-atomically again"
# F6: rebuild's stamp write must use the same privilege as its copy step.
assert 'stamp_installed_head "$SUDO"' in tool, \
    "rebuild stamps unprivileged, so a non-root rebuild records nothing"
# F7: the legacy migration removes the old artifact before the build and lied
# about it in the failure message.
assert "legacy_removed" in tool and "there is no driver installed right now" in tool, \
    "the legacy migration path can leave the machine without a driver and \
     report that state as unchanged"
# F8: status claimed "matches this checkout" from a version string that never
# moves between commits.
assert "Installed modules were built from $head_built" in tool, \
    "status no longer reports which revision the installed modules came from"
# F11: the profile was read with a whole-file grep, so a commented-out option
# could decide the answer.
# Round-9 T59: a marker-less file at one of our own paths blocked install AND
# uninstall, with no documented escape — the user had to find and delete it by
# hand on a machine that could have no driver at all. The escape must exist,
# must move the file aside rather than delete it, must be passed through the
# elevation re-exec, and must be named in the failure message.
assert "quarantine_unowned() {" in tool and 'mv "$f" "$dest"' in tool, \
    "the --repair escape is gone: an unowned file at our path is a dead end again"
assert 'dest="$f.unowned-$(date' in tool, \
    "--repair no longer keeps the file it moves aside"
assert "&& echo --repair" in tool, \
    "install no longer passes --repair through the elevate re-exec, so it is a no-op"
assert "re-run with '--repair'" in tool, \
    "the refusal no longer tells the user how to get unstuck"
assert '--repair) REPAIR=1 ;;' in tool, \
    "uninstall no longer accepts --repair"
assert "options[[:space:]]+sl4a_spi_hid" in tool, \
    "the profile is read from anywhere in the config file again"

assert "rm -f \"$INSTALLED_HEAD_STAMP\"" in tool, \
    "uninstall leaves the revision stamp behind, so the next check lies"

assert "Modules built from revision:" in tool, \
    "the sweep file no longer records the revision the modules came from"

assert "cmd_hunt" in tool and "hunt_verdict" in tool and \
    'read_frame_variant="$variant"' in tool, \
    "the frame hunt is gone: a read-shape bisect would need hand commands"
assert "sl4a_debug_level=3" in tool, \
    "hunt no longer raises the debug level, so the read bytes are not captured"
assert ">>> TOUCH THE PANEL NOW" in tool, \
    "hunt no longer tells the user when to touch the panel"

assert "Unloading the previous build and loading the new one" in tool and \
    "modprobe -r sl4a_spi_hid sl4a_spi_amd" in tool, \
    "install detected a running build that is not the installed one but only " \
    "described the reload instead of doing it"

# `logs -o -x`: the path reaches head/grep/chmod as well as the redirect, and a
# dash-leading one is read as an option (bundle written, completion check
# fails). Normalised once, at the root.
assert '\t\t-*) OUT="./$OUT" ;;' in tool, "a dash-leading -o path is not normalised"

# The bundle must carry the LOADED module identity, not just the on-disk one:
# srcversion moves with every source edit and that comparison is the staleness
# answer the reader needs first.
assert "Loaded vs installed module (srcversion — read first)" in tool
assert "checkout/toolchain string, not the loaded module's identity" in tool, \
    "build_info still reads like the loaded module's version"

# git stderr is never swallowed: refused / clean / modified are three states.
assert "git status: clean (no local modifications)" in tool
assert "git status: FAILED (exit $git_rc)" in tool
assert 'git -C "$REPO_DIR" rev-parse HEAD 2>&1' in tool

# VERSION is interpolated by sed into dkms.conf and by DKMS into its build
# line: '1.0.&' staged PACKAGE_VERSION="1.0.#VERSION#" and a space split the
# build command, so the charset is enforced before either can happen.
assert "=~ ^[0-9A-Za-z][0-9A-Za-z.+~_-]*$" in tool, \
    "VERSION is not strictly validated before it reaches sed and dkms"

# One helper reads the live profile, used by install's profile-change check
# and by status — status must not print the config file's value as "active".
assert tool.count("loaded_raw_mode") >= 3, \
    "loaded_raw_mode must be defined once and used by install and status"
assert "Profile running right now" in tool
assert "Modprobe profile for the next boot" in tool

print("installer recovery contract: PASS")
