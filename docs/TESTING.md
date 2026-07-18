# Testing

## Portable Tests

Run the protocol suite with sanitizers:

```sh
make -C tests SANITIZE=1 test
```

It checks V0 header parsing, read/write encoding, every 12-bit output length,
all valid header offsets, deterministic fuzz vectors, immutable-corpus pairing,
the capture-only acceptance boundary, and strict CapImg decoding across all
15,213 retained frames. It does not execute kernel workqueues or SPI transfers.

Build the optional libFuzzer harness for the pure V0 decoder only:

```sh
make -C tests fuzz
./tests/protocol_fuzz -runs=1000000 -max_len=65536
```

The fuzz target has no kernel module, SPI, GPIO, or HID-input dependency.

## Immutable Corpus

Generate a JSON summary of the three frozen Windows CSV traces:

```sh
python3 tools/analyze_raw_corpus.py
```

The tool verifies the SHA-256 contract before parsing and reports only observed
transactions. It pairs a prefixed V0 header read with its following prefixed
body read on the same connection. A missing `raw_0c_frames` entry means the CSV
corpus does not contain an extractable complete raw frame; it is not evidence
about the device.

The report also locks capture characterization: payload stream hashes, gaps over
20 ms, header-to-body delay, and fixed/variable byte regions. These metrics
falsify overly broad layout claims; they do not identify payload semantics.

## Passive Capture Files

`raw_capture_only=1` is not part of this test procedure and does not enable raw
mode. If a future separately-authorized observation produces a binary
`raw_capture` snapshot, validate it offline with:

```sh
sudo python3 tools/extract_raw_capture.py /path/to/raw_capture --output /tmp/raw-v0
```

The tool requires exactly eight 4304-byte slots, emits hashes for every slot,
and writes only envelopes matching the frozen `ce 10 0c` boundary. It has no
device-I/O code.

Raw-mode hardware work additionally follows [Raw Mode Validation](RAW_MODE_VALIDATION.md).

## Kernel Fault Tests

The running kernel must enable at least:

```text
CONFIG_KUNIT=y
CONFIG_FAULT_INJECTION=y
CONFIG_FAIL_FUNCTION=y
CONFIG_KASAN=y
CONFIG_KCSAN=y
CONFIG_PROVE_LOCKING=y
```

Run fault tests on a disposable kernel and use a physical keyboard. Required
scenarios are failed SPI reads/writes at every sequencer state, delayed or
dropped IRQs, truncated headers/bodies, capture-ring overwrite, and module
removal while every delayed worker is queued. A passing run has no KASAN,
KCSAN, lockdep, workqueue, or use-after-free report and finishes in either ready
or an explicit failed state. Standard mode has no descriptor fallback.

The distribution kernel currently used for development has KUnit and fault
injection disabled, so these scenarios require a dedicated test kernel.
