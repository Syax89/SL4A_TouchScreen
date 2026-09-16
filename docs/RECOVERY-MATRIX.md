# Recovery matrix: which stall has a timer

The driver can stall in a handful of states, and only some of them have a timer
watching them. This page is the map: what is armed where, in which profile, what
it does when it fires, and — for the combinations that have nothing — the state
that stays silent at the default `sl4a_debug_level=0`.

Written from the code, not from intent: every row was read back out of
`driver/spi-hid-core.c` / `driver/mshw0231-raw.c`.

## Timers

| Timer | Period | Armed by | Covers | Fire → effect | Level-0 log |
| --- | --- | --- | --- | --- | --- |
| `descreq_work` | 100 ms | entering `WAIT_DESC` (both profiles) | `WAIT_DESC` | re-reads the input register: a `DEVICE_DESC` advances, a `RESET_RSP` goes to the hardcoded fallback, anything else re-schedules. Never re-sends the DESCREQ. | only on the fallback |
| `raw_handshake_watchdog` | 2000 ms | entering `WAIT_RESET`/`WAIT_DESC`/`WAIT_RPT` and `DONE` **in raw mode**, resume, and several error paths | raw `WAIT_RESET`, `WAIT_DESC`, `WAIT_RPT`, `DONE` before confirmation, `WAIT_FEATURE` (deferred) | warns, restarts discovery; after the retry budget a 5 s cold-boot restart ×2, then **falls back to standard HID** | yes (every step) |
| `raw_probe_retry_work` | 5000 ms | raw watchdog exhaustion | raw cold boot | restarts discovery | via the watchdog |
| `feat_delay_work` | `getfeat_delay_ms` | raw `WAIT_RPT` when the delay is > 0 | raw `WAIT_RPT` | vendor init + GET_FEATURE + SET_FEATURE | write failures |
| `stream_watchdog` | `stream_watchdog_ms` (2000) | first raw `0x0C` frame; standard `DONE` only if `std_liveness_ms > 0` | raw `DONE`; standard `DONE` (one-shot) | raw: re-init after 3 misses, up to `stream_watchdog_max_retries`; standard: logs, recovers only with `std_liveness_recover` | yes |
| `poll_work` | 20 ms | raw `DONE` | raw `DONE` | reads the input register, confirms the handshake, **cannot reset the device** | only oversized frames |
| `wait_reset_watchdog` | `wait_reset_kick_ms` | `WAIT_RESET` entry | **standard only** `WAIT_RESET` | DESCREQ kick, up to 3 attempts, then gives up | yes |
| `error_work` (ACPI `_PS3→_PS0`) | — | fatal sync timeout, IRQ storm, `std_liveness_recover`, create-device failure | any state once triggered | power cycle, parks `WAIT_RESET` | yes |

`poll_interval_ms` is not a knob: it is an internal value set to 20 ms when raw
`DONE` is entered.

## The stalls with nothing watching

| # | Profile / state | Sequence | What exists | What it does |
| --- | --- | --- | --- | --- |
| 1 | standard `WAIT_RESET` | probe or resume, controller silent | `wait_reset_watchdog`, but `wait_reset_kick_ms=0` by default | never armed |
| 2 | standard `WAIT_DESC` | `RESET_RSP`, then the DESCREQ is ignored | `descreq_work` | only re-reads; no level-0 log, no recovery |
| 3 | standard `WAIT_RPT` | `DEVICE_DESC` received, no `RPT_DESC` | nothing (the raw arm is raw-only) | silent forever |
| 4 | standard `DONE` | device never streams | `std_liveness_ms=0` by default | no monitor at all |
| 5 | standard `DONE` | stream worked, then stalls | the liveness check is one-shot | no second check |
| 6 | raw `WAIT_RESET` at cold probe | controller never sends a `RESET_RSP` | the raw arm covers `WAIT_DESC`/`WAIT_RPT`/`DONE` and (since the resume fix) `WAIT_RESET` on resume — cold probe is covered by the `WAIT_RESET` arm added with it | closed |
| 7 | raw `DONE` after confirmation | `stream_watchdog_ms=0` (not the default) | `poll_work` | ticks forever, cannot reset |
| 8 | raw `DONE` | `stream_watchdog_max_retries=0` | — | one logged give-up, then nothing |

Standard mode has no fallback *to* anything: there is no second publisher to
switch to, so a stall there is a dead panel until the next resume or reload.
Raw mode's advantage is exactly that it can hand the panel back to standard HID.

## What a raw-mode bundle should show when the device stalls

For `raw_mode=Y raw_input_beta=Y skip_getfeat=Y`, `wait_reset_kick_ms=0`,
`sl4a_debug_level=0`, controller answering every IRQ with a `RESET_RSP` — the
lines that are *not* gated by the debug level:

```
SEQ: raw_mode handshake watchdog: no heatmap data after 2000ms, retrying (2 left)
SEQ: raw_mode handshake watchdog: no heatmap data after 2000ms, retrying (1 left)
SEQ: raw_mode handshake watchdog: no heatmap data after 2000ms, retrying (0 left)
SEQ: raw handshake failed (attempt 2/3), restarting in 5s...
SEQ: raw handshake failed (attempt 3/3), restarting in 5s...
SEQ: raw_mode handshake failed after 3 attempts, falling back to standard HID
SEQ: raw handshake failed; using standard HID
```

The state *transitions* stay hidden at level 0 (`seq_dbg` is gated): nothing in
the list above names the state the device was stuck in. `sl4a_debug_level=1`
adds them.

## Knobs that remove recovery

- `wait_reset_kick_ms=0` (default): removes the only standard-mode pre-`DONE`
  timer.
- `std_liveness_ms=0` (default): removes standard-mode post-`DONE` detection;
  `std_liveness_recover=0` means detection would not act anyway.
- `stream_watchdog_ms=0`: removes raw runtime recovery after confirmation.
- `stream_watchdog_max_retries=0`: raw recovery stops after one logged batch.
- `raw_mode=0`: disables every raw watchdog at once.

`skip_getfeat`, `raw_input_beta`, `wire_double_opcode`, `getfeat_delay_ms` and
the `blob_*`/calibration family do not remove recovery: the raw watchdog is
armed on `WAIT_RPT` and `DONE` entry regardless of which post-`RPT` shape
`skip_getfeat` picks.
