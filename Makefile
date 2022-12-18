SRC_DIR = src
INC_DIR = inc
LIB_DIR = lib

CC = gcc
LD = $(CC)

CFLAGS = -std=c89 -pedantic -c -I$(INC_DIR)
LDFLAGS =

SOURCES = $(shell find $(SRC_DIR) -name *.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(LIB_DIR)/%.o, $(SOURCES))
OUT_BIN = guagacode

all: $(OUT_BIN)

clean:
	rm -rf $(LIB_DIR)/*
	rm -f $(OUT_BIN)

$(OUT_BIN): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

$(LIB_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $@
	rmdir $@
	$(CC) $(CFLAGS) -o $@ $<
