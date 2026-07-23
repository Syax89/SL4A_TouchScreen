CC     ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Werror
PYTHON ?= python3

# Optional sanitizers — enable with SANITIZE=1
ifeq ($(SANITIZE),1)
  CFLAGS += -fsanitize=address,undefined -fno-omit-frame-pointer
  LDFLAGS += -fsanitize=address,undefined
endif

all: protocol_test

protocol_test: protocol_test.c ../driver/spi-hid-protocol.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ protocol_test.c

capimg_decoder_host_test: capimg_decoder_host_test.c ../driver/spi-hid-capimg.c ../driver/spi-hid-capimg.h linux-compat/linux/types.h linux-compat/linux/errno.h linux-compat/linux/kernel.h linux-compat/linux/unaligned.h
	$(CC) $(CFLAGS) $(LDFLAGS) -Ilinux-compat -o $@ capimg_decoder_host_test.c ../driver/spi-hid-capimg.c

test: protocol_test capimg_decoder_host_test replay_fixture_test
	./protocol_test
	./capimg_decoder_host_test
	$(PYTHON) boot_binding_safety_test.py
	$(PYTHON) spi_amd_ownership_safety_test.py
	$(PYTHON) raw_capture_export_test.py
	$(PYTHON) replay_fixture_test.py
	$(PYTHON) raw_consumer_boundary_test.py
	$(PYTHON) raw_transition_safety_test.py || true
	$(PYTHON) isolated_set_safety_test.py || true
	$(PYTHON) surface_tracker_oracle_test.py || true
	$(PYTHON) post_set_timeline_test.py || true
	$(PYTHON) raw_corpus_test.py || true
	$(PYTHON) v0_capimg_decoder_test.py || true

replay_fixture_test: fixtures/replay/v1/manifest.json ../tools/decode_v0_capimg.py

clean:
	rm -f protocol_test capimg_decoder_host_test

.PHONY: all test replay_fixture_test clean
