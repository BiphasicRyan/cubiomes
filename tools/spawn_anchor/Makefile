CC ?= gcc
CFLAGS ?= -O3 -Wall -Wextra -fwrapv
LDFLAGS ?= -lm -pthread

TARGET := spawn_anchor
SRC := spawn_anchor.c

ROOT := ../..

all: $(TARGET)

$(ROOT)/libcubiomes.a:
	$(MAKE) -C $(ROOT)

$(TARGET): $(SRC) $(ROOT)/libcubiomes.a
	$(CC) $(CFLAGS) -I$(ROOT) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)