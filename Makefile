BUILD_DIR=build
C_SOURCES=$(wildcard $(dir)/*.c)
CFLAGS=-Isrc -Ithird_party/yaMBSiavr -Os -mmcu=atmega8515 -DF_CPU=7372800UL

MAIN_SOURCES=src/main.c third_party/yaMBSiavr/yaMBSiavr.c
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

flash: $(BUILD_DIR)/main
	avrdude -F -p m8515 -c usbasp -U flash:w:$(BUILD_DIR)/main

all: $(BUILD_DIR)/main

.DEFAULT_GOAL := all
.PHONY: default all clean