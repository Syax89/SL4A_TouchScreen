#!/usr/bin/env python3
"""Compile and execute the real seq_handle_data() body with counted endpoints."""

from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
CORE = (ROOT / "driver" / "spi-hid-core.c").read_text(encoding="utf-8")
PROTOCOL = (ROOT / "driver" / "spi-hid-protocol.h").read_text(encoding="utf-8")

def extract_c_function(source: str, signature: str) -> str:
    start = source.find(signature)
    if start < 0:
        raise SystemExit(f"cannot locate {signature}")
    brace = source.find("{", start)
    if brace < 0:
        raise SystemExit(f"cannot locate body for {signature}")
    depth = 0
    for i in range(brace, len(source)):
        if source[i] == "{":
            depth += 1
        elif source[i] == "}":
            depth -= 1
            if depth == 0:
                return source[start:i + 1]
    raise SystemExit(f"cannot isolate {signature}")

data_policy = extract_c_function(
    PROTOCOL,
    "static inline int spi_hid_protocol_data_should_drain",
)
publish_policy = extract_c_function(
    PROTOCOL,
    "static inline int spi_hid_protocol_hid_publish_ready",
)

needle = "static void seq_handle_data(struct spi_hid *shid, int type, u16 blen)"
start = CORE.rfind(needle)
if start < 0:
    raise SystemExit("cannot locate production seq_handle_data()")

brace = CORE.find("{", start)
if brace < 0:
    raise SystemExit("cannot locate seq_handle_data() body")
depth = 0
end = None
for i in range(brace, len(CORE)):
    if CORE[i] == "{":
        depth += 1
    elif CORE[i] == "}":
        depth -= 1
        if depth == 0:
            end = i + 1
            break
if end is None:
    raise SystemExit("cannot isolate seq_handle_data()")
handler = CORE[start:end]

prelude = r'''
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

struct device { int dummy; };
struct spi_device { struct device dev; };
struct spi_hid_desc { u32 max_input_length; };
struct spi_hid {
    struct spi_device *spi;
    void *hid;
    bool hid_creating;
    void *touch_input;
    unsigned long stat_reset_rsp;
    unsigned long stat_data;
    unsigned long stat_frames_dropped;
    unsigned long stat_raw_observed;
    bool raw_mode_active;
    bool raw_handshake_confirmed;
    unsigned int storm_resets;
    unsigned int raw_probe_attempts;
    unsigned int stream_watchdog_reinits;
    bool poll_active;
    bool stream_watchdog_active;
    int raw_handshake_watchdog;
    int raw_probe_retry_work;
    int stream_watchdog;
    struct spi_hid_desc desc;
    u32 data_buf_len;
    u8 *data_buf;
};

#define SPI_HID_REPORT_TYPE_COMMAND_RESP 4
#define SPI_HID_REPORT_TYPE_GET_FEATURE_RESP 5
#define SPI_HID_REPORT_TYPE_REPORT_DESC 2
#define SPI_HID_SEQ_DEVICE_RESET 9
#define HID_INPUT_REPORT 0
#define min_t(type, a, b) ((type)(a) < (type)(b) ? (type)(a) : (type)(b))
#define seq_dbg(...) do { } while (0)
#define dev_warn(...) do { } while (0)
#define dev_warn_ratelimited(...) do { } while (0)
#define dev_info(...) do { } while (0)

static int stream_watchdog_ms = 2000;
static bool raw_input_beta = true;
static int std_bridge_enabled = 1;
static u8 mock_report_id = 0x0c;
static u32 last_read_len;
static int read_calls;
static int consume_calls;
static u32 consume_len;
static int hid_publish_calls;
static int schedule_calls;
static int mark_live_calls;

static void reset_counts(void)
{
    last_read_len = 0;
    read_calls = 0;
    consume_calls = 0;
    consume_len = 0;
    hid_publish_calls = 0;
    schedule_calls = 0;
    mark_live_calls = 0;
}

static void spi_hid_seq_handle_sync_response(struct spi_hid *s, int t, u16 b)
{ (void)s; (void)t; (void)b; }
static int spi_hid_seq_restart_discovery(struct spi_hid *s, int r)
{ (void)s; (void)r; return 0; }

__DATA_POLICY__
__PUBLISH_POLICY__
static int spi_hid_protocol_raw_confirms_handshake(u8 cid, u16 rl)
{
    return cid == 0x0c && rl >= 4300 && rl <= 4310;
}
static int spi_hid_std_capimg_enabled(const struct spi_hid *s)
{
    (void)s;
    return std_bridge_enabled;
}
static void cancel_delayed_work(void *w) { (void)w; }
static void stream_watchdog_mark_live(struct spi_hid *s)
{ (void)s; mark_live_calls++; }
static unsigned long msecs_to_jiffies(unsigned int ms) { return ms; }
static int schedule_delayed_work(void *w, unsigned long delay)
{ (void)w; (void)delay; schedule_calls++; return 1; }

static int spi_hid_seq_read(struct spi_hid *s, u8 *buf, u32 len)
{
    read_calls++;
    last_read_len = len;
    memset(buf, 0, len);
    if (len >= 9) {
        /* report_len = 4302, report id = 0x0c; rblen is 4309 so the
         * tracker receives 4304 bytes starting at body[5]. */
        buf[5] = 0xce;
        buf[6] = 0x10;
        buf[7] = mock_report_id;
    }
    return 0;
}

static int mshw0231_raw_consume_v0(struct spi_hid *s, const u8 *buf, u32 len)
{
    (void)s; (void)buf;
    consume_calls++;
    consume_len = len;
    return 0;
}

static int hid_input_report(void *hid, int type, const u8 *buf, u32 len, int intr)
{
    (void)hid; (void)type; (void)buf; (void)len; (void)intr;
    hid_publish_calls++;
    return 0;
}
'''

main = r'''
static int failures;
#define CHECK(c, msg) do { if (!(c)) { fprintf(stderr, "FAIL: %s\n", msg); failures++; } } while (0)

static void init(struct spi_hid *s, struct spi_device *spi, u8 *buf)
{
    memset(s, 0, sizeof(*s));
    memset(spi, 0, sizeof(*spi));
    s->spi = spi;
    s->touch_input = (void *)0x2;
    s->desc.max_input_length = 8192;
    s->data_buf_len = 8192;
    s->data_buf = buf;
}

int main(void)
{
    struct spi_hid s;
    struct spi_device spi;
    static u8 buf[8192];

    /* DATA arrives while hid_add_device() is active: read/consume all of it,
     * but do not publish through HID and do not arm raw runtime recovery. */
    init(&s, &spi, buf);
    s.hid = (void *)0x1;
    s.hid_creating = true;
    reset_counts();
    seq_handle_data(&s, 1, 4304);
    CHECK(read_calls == 1, "hid_creating: DATA body was not read");
    CHECK(last_read_len == 4309, "hid_creating: handler did not request the full 4309-byte frame");
    CHECK(consume_calls == 1 && consume_len == 4304,
          "hid_creating: full 4304-byte CapImg body did not reach tracker");
    CHECK(hid_publish_calls == 0,
          "hid_creating: HID publication occurred before registration completed");
    CHECK(schedule_calls == 0 && mark_live_calls == 0,
          "standard bridge incorrectly armed the raw runtime watchdog");

    /* The tracker can own DATA before shid->hid exists or hid_creating flips. */
    init(&s, &spi, buf);
    s.hid = NULL;
    s.hid_creating = false;
    reset_counts();
    seq_handle_data(&s, 1, 4304);
    CHECK(read_calls == 1 && consume_calls == 1,
          "pre-HID tracker ownership failed to drain/consume DATA");
    CHECK(hid_publish_calls == 0,
          "pre-HID tracker ownership published through an absent HID device");
    CHECK(schedule_calls == 0,
          "pre-HID standard bridge scheduled standard-liveness/runtime watchdog work");

    /* Default standard mode remains upstream-equivalent: hid_creating alone
     * does not acquire early DATA ownership when no CapImg tracker exists. */
    init(&s, &spi, buf);
    s.touch_input = NULL;
    s.hid = (void *)0x1;
    s.hid_creating = true;
    std_bridge_enabled = 0;
    reset_counts();
    seq_handle_data(&s, 1, 4304);
    CHECK(read_calls == 0 && consume_calls == 0 && hid_publish_calls == 0,
          "default standard HID creation unexpectedly drained/published DATA");
    CHECK(schedule_calls == 0,
          "default standard HID creation scheduled runtime watchdog work");
    std_bridge_enabled = 1;

    /* Once HID creation completes, normal HID publication still occurs. */
    init(&s, &spi, buf);
    s.hid = (void *)0x1;
    s.hid_creating = false;
    reset_counts();
    seq_handle_data(&s, 1, 4304);
    CHECK(last_read_len == 4309 && consume_calls == 1 && consume_len == 4304,
          "post-create DATA path did not read and track the full frame");
    CHECK(hid_publish_calls == 0,
          "post-create CapImg 0x0c must remain held for tracker, not HID-dispatched");
    CHECK(schedule_calls == 0,
          "standard bridge armed raw runtime recovery after HID creation");

    /* Syax89 review: default standard (NO transition, NO tracker) must
     * still HOLD 0x0c after HID creation, exactly as upstream did.
     * The real descriptor includes report 12 but that does not
     * authorize a change to unsolicited HID publication policy. */
    init(&s, &spi, buf);
    s.touch_input = NULL;
    s.hid = (void *)0x1;
    std_bridge_enabled = 0;
    mock_report_id = 0x0c;
    reset_counts();
    seq_handle_data(&s, 1, 4304);
    CHECK(read_calls == 1 && consume_calls == 0,
          "default standard mode no longer reads/holds report 0x0c");
    CHECK(hid_publish_calls == 0,
          "REGRESSION: default standard 0x0c was forwarded to HID");

    /* HID publication is unchanged for a different standard report. */
    mock_report_id = 0x40;
    reset_counts();
    seq_handle_data(&s, 1, 4304);
    CHECK(read_calls == 1 && hid_publish_calls == 1,
          "default standard report 0x40 should still be HID-dispatched");

    /* The active CapImg bridge must consume 0x0c through its own
     * tracker and NOT duplicate it through HID report dispatch. */
    init(&s, &spi, buf);
    s.hid = (void *)0x1;
    mock_report_id = 0x0c;
    std_bridge_enabled = 1;
    reset_counts();
    seq_handle_data(&s, 1, 4304);
    CHECK(read_calls == 1 && consume_calls == 1 &&
          hid_publish_calls == 0,
          "opt-in bridge must route 0x0c only to CapImg tracker");

    /* Raw mode keeps its inherited runtime watchdog behavior. */
    init(&s, &spi, buf);
    s.raw_mode_active = true;
    std_bridge_enabled = 0;
    s.hid = (void *)0x1;
    reset_counts();
    seq_handle_data(&s, 1, 4304);
    CHECK(schedule_calls == 1 && mark_live_calls == 1,
          "raw mode no longer arms its runtime watchdog");
    CHECK(consume_calls == 1 && hid_publish_calls == 0,
          "raw tracker must retain upstream 0x0c HID hold semantics");

    /* Raw mode without a tracker historically still dispatches to HID. */
    s.touch_input = NULL;
    reset_counts();
    seq_handle_data(&s, 1, 4304);
    CHECK(hid_publish_calls == 1 && consume_calls == 0,
          "raw mode without tracker lost base HID publication behavior");

    if (failures)
        return 1;
    puts("std CapImg production DATA handler test: PASS");
    return 0;
}
'''

with tempfile.TemporaryDirectory(prefix="sl4a-data-handler-") as td:
    td = Path(td)
    src = td / "probe.c"
    exe = td / "probe"
    exact_prelude = (
        prelude
        .replace("__DATA_POLICY__", data_policy)
        .replace("__PUBLISH_POLICY__", publish_policy)
    )
    src.write_text(exact_prelude + "\n" + handler + "\n" + main, encoding="utf-8")
    subprocess.run(
        ["cc", "-std=c11", "-Wall", "-Wextra", "-Werror",
          # Extracted kernel code meets simplified host mocks; retain Werror
          # for everything except diagnostics caused by stubbed loggers and
          # kernel u16/unsigned comparisons.
          "-Wno-unused-parameter", "-Wno-unused-variable",
          "-Wno-sign-compare",
         "-fsanitize=address,undefined", "-fno-omit-frame-pointer",
         str(src), "-o", str(exe)],
        check=True,
    )
    subprocess.run([str(exe)], check=True)
