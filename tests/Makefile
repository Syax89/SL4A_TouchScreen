CC     ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Werror

# Optional sanitizers — enable with SANITIZE=1
ifeq ($(SANITIZE),1)
  CFLAGS += -fsanitize=address,undefined -fno-omit-frame-pointer
  LDFLAGS += -fsanitize=address,undefined
endif

all: protocol_test

protocol_test: protocol_test.c ../driver/spi-hid-protocol.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ protocol_test.c

test: protocol_test
	./protocol_test

clean:
	rm -f protocol_test

.PHONY: all test clean
