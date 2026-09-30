.PHONY: build

PROJECT := $(CURDIR)
BUILD := $(PROJECT)/build
SOURCES := $(PROJECT)/src/crypto

build:
	cc $(SOURCES)/main.c \
	$(SOURCES)/encryptor.c \
	-lsodium -o $(BUILD)/main

