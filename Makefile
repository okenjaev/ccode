TARGET = 4me

CC = gcc

PYTHON_CFLAGS := $(shell python3-config --cflags)
PYTHON_LFLAGS := $(shell python3-config --ldflags --embed)

CFLAGS = -I$(INCLUDE_DIR) -Wall -g -fsanitize=address
LFLAGS = $(PYTHON_LFLAGS) -fsanitize=address

SRC_DIR = src
BUILD_DIR = bin
OBJ_DIR = $(BUILD_DIR)/objs
INCLUDE_DIR = include

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

default: $(OBJ_DIR) $(BUILD_DIR)/$(TARGET)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c -o $@ $^ $(CFLAGS)

$(OBJ_DIR)/interp.o: $(SRC_DIR)/interp.c
	$(CC) -c -o $@ $^ $(CFLAGS) $(PYTHON_CFLAGS)

clean:
	rm -rf $(BUILD_DIR)
