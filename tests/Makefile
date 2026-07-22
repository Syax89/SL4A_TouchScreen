CC     ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Werror
CLANG  ?= clang
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

test: protocol_test capimg_decoder_host_test raw_corpus_test raw_capture_export_test v0_capimg_decoder_test
	./protocol_test
	./capimg_decoder_host_test
	$(PYTHON) raw_transition_safety_test.py
	$(PYTHON) isolated_set_safety_test.py
	$(PYTHON) post_set_timeline_test.py
	$(PYTHON) surface_tracker_oracle_test.py
	$(PYTHON) raw_corpus_test.py
	$(PYTHON) raw_capture_export_test.py
	$(PYTHON) v0_capimg_decoder_test.py

raw_corpus_test:

raw_capture_export_test: ../tools/extract_raw_capture.py

v0_capimg_decoder_test: ../tools/decode_v0_capimg.py

fuzz: protocol_fuzz.c ../driver/spi-hid-protocol.h
	$(CLANG) -std=c11 -Wall -Wextra -Werror -g -O1 \
		-fsanitize=fuzzer,address,undefined -fno-omit-frame-pointer \
		-o protocol_fuzz protocol_fuzz.c

clean:
	rm -f protocol_test protocol_fuzz capimg_decoder_host_test

.PHONY: all test raw_corpus_test raw_capture_export_test v0_capimg_decoder_test fuzz clean
