TARGET = 4me

CC = gcc

CFLAGS = $(shell python3-config --cflags) -I$(INCLUDE_DIR) -Wall -g -fsanitize=address
LFLAGS = $(shell python3-config --ldflags --embed) -fsanitize=address

SRC_DIR = src
BUILD_DIR = bin
OBJ_DIR = $(BUILD_DIR)/objs
INCLUDE_DIR = include

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

default: $(OBJ_DIR) $(BUILD_DIR)/$(TARGET)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c -o $@ $^ $(CFLAGS)

clean:
	rm -rf $(BUILD_DIR)
