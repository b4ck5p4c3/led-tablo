BUILD_DIR=build
C_SOURCES=$(wildcard $(dir)/*.c)
CFLAGS=-Isrc -Os -mmcu=atmega8515

MAIN_SOURCES=src/main.c
MAIN_OBJECTS=$(foreach src,$(notdir $(MAIN_SOURCES)),$(patsubst %.c,%.c.o,$(BUILD_DIR)/$(src)))

SOURCES=$(MAIN_SOURCES)
OBJECTS=$(MAIN_OBJECTS)
OUTPUT_DIRS=$(BUILD_DIR)

CC=avr-gcc

$(BUILD_DIR):
	mkdir -p $@

$(SOURCES): $(OUTPUT_DIRS)

$(OBJECTS): $(SOURCES)
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR)/main: $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $(SOURCES)

all: $(BUILD_DIR)/main

.DEFAULT_GOAL := all
.PHONY: default all clean