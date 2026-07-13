# Testing

## Portable Tests

Run the protocol suite with sanitizers:

```sh
make -C tests SANITIZE=1 test
```

It checks V0 header parsing, read/write encoding, every 12-bit output length,
all valid header offsets, and deterministic fuzz vectors. It does not execute
kernel workqueues or SPI transfers.

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
dropped IRQs, truncated headers/bodies, raw SET_FEATURE silence, and module
removal while every delayed worker is queued. A passing run has no KASAN,
KCSAN, lockdep, workqueue, or use-after-free report and finishes in either
ready, standard-HID fallback, or an explicit failed state.

The distribution kernel currently used for development has KUnit and fault
injection disabled, so these scenarios require a dedicated test kernel.
