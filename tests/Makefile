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

test: protocol_test raw_corpus_test raw_capture_export_test v0_capimg_decoder_test
	./protocol_test
	$(PYTHON) raw_corpus_test.py
	$(PYTHON) raw_capture_export_test.py
	$(PYTHON) v0_capimg_decoder_test.py

raw_corpus_test: ../tools/analyze_raw_corpus.py ../tools/parse_spb_csv.py

raw_capture_export_test: ../tools/extract_raw_capture.py

v0_capimg_decoder_test: ../tools/decode_v0_capimg.py

fuzz: protocol_fuzz.c ../driver/spi-hid-protocol.h
	$(CLANG) -std=c11 -Wall -Wextra -Werror -g -O1 \
		-fsanitize=fuzzer,address,undefined -fno-omit-frame-pointer \
		-o protocol_fuzz protocol_fuzz.c

clean:
	rm -f protocol_test protocol_fuzz

.PHONY: all test raw_corpus_test raw_capture_export_test v0_capimg_decoder_test fuzz clean
