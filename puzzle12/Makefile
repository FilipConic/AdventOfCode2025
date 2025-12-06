CC = gcc
FLAGS = -Wall -Wextra -ggdb -I./.. -std=c23 -lm

TARGET = main
BUILD_DIR = ./build

.PHONY: all clear

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(BUILD_DIR)/main.o
	$(CC) $(FLAGS) $^ -o $@

$(BUILD_DIR)/main.o: main.c
	$(CC) $(FLAGS) -c $< -o $@

clear:
	rm -rf $(BUILD_DIR) $(TARGET)
