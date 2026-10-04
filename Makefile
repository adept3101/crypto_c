CC := cc
CFLAGS := -Wall
PROJECT := $(CURDIR)
BUILD := $(PROJECT)/build
SOURCES := $(PROJECT)/src/crypto/*.c
TARGET := $(BUILD)/main

.PHONY: all clean

all:
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) \
	$(SOURCES) \
	-lsodium -lncursesw -o $(TARGET)

clean:
	rm -rf $(BUILD)
