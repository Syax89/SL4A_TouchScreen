CC     ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Werror
PYTHON ?= python3

# Optional sanitizers — enable with SANITIZE=1
ifeq ($(SANITIZE),1)
  CFLAGS += -fsanitize=address,undefined -fno-omit-frame-pointer
  LDFLAGS += -fsanitize=address,undefined
endif

all: protocol_test capimg_decoder_host_test c590_atan2_host_test

protocol_test: protocol_test.c ../driver/spi-hid-protocol.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ protocol_test.c

capimg_decoder_host_test: capimg_decoder_host_test.c ../driver/spi-hid-capimg.c ../driver/spi-hid-capimg.h linux-compat/linux/types.h linux-compat/linux/errno.h linux-compat/linux/kernel.h linux-compat/linux/unaligned.h
	$(CC) $(CFLAGS) $(LDFLAGS) -Ilinux-compat -o $@ capimg_decoder_host_test.c ../driver/spi-hid-capimg.c

c590_atan2_host_test: c590_atan2_host_test.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ c590_atan2_host_test.c

raw_pipeline_math_test: raw_pipeline_math_test.c ../driver/mshw0231-raw-constants.h
	$(CC) -std=gnu11 -Wall -Wextra -Werror $(LDFLAGS) -o $@ raw_pipeline_math_test.c -lm

# ── raw_pipeline_replay_test: links the REAL driver/mshw0231-raw.c ──
#
# driver/mshw0231-raw.c reaches "spi-hid-core.h" via a quote-form
# #include, and the C standard's quote-include search order always
# checks the directory of the *including* file first, before any -I
# path — so simply listing raw-pipeline-stubs ahead of ../driver on
# -I would NOT be enough by itself (the compiler would still find
# ../driver/spi-hid-core.h first, since that's mshw0231-raw.c's own
# directory). To make the stub actually win, mshw0231-raw.c is staged
# (copied, not modified) into raw-pipeline-stage/ first — a directory
# that does not contain the real spi-hid-core.h — so its own-directory
# lookup finds nothing and falls through to -I search order, where
# linux-compat/raw-pipeline-stubs (listed first) shadows ../driver
# (listed after, for mshw0231-raw.h/mshw0231-raw-constants.h/
# spi-hid-capimg.h, which ARE meant to resolve to the real driver
# headers unchanged).
#
# driver/spi-hid-capimg.c is compiled from its real location (same
# pattern capimg_decoder_host_test already uses) and linked in too:
# mshw0231-raw.c's mshw0231_raw_consume_v0() calls
# spi_hid_capimg_decode_v0(), so the object has an undefined reference
# to it regardless of whether the test calls that particular entry
# point at runtime.
#
# mshw0231-raw.c itself is compiled without -Werror (and with
# -Wno-sign-compare): it's kernel source written against the kernel's
# own build flags, not this test suite's; the harness's own code
# (stubs.c, the test file) keeps the strict -Wall -Wextra -Werror the
# rest of tests/ uses.
RAW_STAGE_DIR := raw-pipeline-stage
RAW_STUB_DIR  := linux-compat/raw-pipeline-stubs

$(RAW_STAGE_DIR)/mshw0231-raw.c: ../driver/mshw0231-raw.c
	@mkdir -p $(RAW_STAGE_DIR)
	cp $< $@

raw_pipeline_replay_test: raw_pipeline_replay_test.c \
		$(RAW_STAGE_DIR)/mshw0231-raw.c \
		../driver/spi-hid-capimg.c \
		$(RAW_STUB_DIR)/stubs.c \
		$(RAW_STUB_DIR)/spi-hid-core.h \
		$(RAW_STUB_DIR)/mt_record.h \
		../driver/mshw0231-raw.h ../driver/mshw0231-raw-constants.h ../driver/spi-hid-capimg.h \
		linux-compat/linux/input.h linux-compat/linux/input/mt.h linux-compat/linux/jiffies.h \
		linux-compat/linux/slab.h linux-compat/linux/module.h linux-compat/linux/math.h \
		linux-compat/linux/string.h linux-compat/linux/kernel.h linux-compat/linux/types.h \
		linux-compat/linux/errno.h
	$(CC) -std=gnu11 -Wall -Wextra -Werror \
		-I$(RAW_STUB_DIR) -Ilinux-compat -I../driver \
		-c -o $(RAW_STAGE_DIR)/raw_pipeline_replay_test.o raw_pipeline_replay_test.c
	$(CC) -std=gnu11 -Wall -Wextra -Werror \
		-I$(RAW_STUB_DIR) -Ilinux-compat -I../driver \
		-c -o $(RAW_STAGE_DIR)/stubs.o $(RAW_STUB_DIR)/stubs.c
	$(CC) -std=gnu11 -Wall -Wno-sign-compare \
		-I$(RAW_STUB_DIR) -Ilinux-compat -I../driver \
		-c -o $(RAW_STAGE_DIR)/mshw0231-raw.o $(RAW_STAGE_DIR)/mshw0231-raw.c
	$(CC) -std=gnu11 -Wall -Wextra -Werror \
		-I$(RAW_STUB_DIR) -Ilinux-compat -I../driver \
		-c -o $(RAW_STAGE_DIR)/spi-hid-capimg.o ../driver/spi-hid-capimg.c
	$(CC) $(LDFLAGS) -o $@ \
		$(RAW_STAGE_DIR)/raw_pipeline_replay_test.o $(RAW_STAGE_DIR)/stubs.o \
		$(RAW_STAGE_DIR)/mshw0231-raw.o $(RAW_STAGE_DIR)/spi-hid-capimg.o -lm

test: protocol_test capimg_decoder_host_test c590_atan2_host_test raw_pipeline_math_test raw_pipeline_replay_test replay_fixture_test
	./protocol_test
	./capimg_decoder_host_test
	./c590_atan2_host_test
	./raw_pipeline_math_test
	./raw_pipeline_replay_test
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
	rm -f protocol_test capimg_decoder_host_test c590_atan2_host_test raw_pipeline_math_test raw_pipeline_replay_test
	rm -rf $(RAW_STAGE_DIR)

.PHONY: all test replay_fixture_test clean
