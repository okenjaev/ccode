
CC ?= gcc
CFLAGS := -Iinclude/ 
DBGFLAGS := -g -fsanitize=address
COBJFLAGS := $(CFLAGS) -c

# path macros
BIN_PATH := bin
OBJ_PATH := bin-obj
SRC_PATH := src
DBG_PATH := bin-debug

# compile macros
TARGET_NAME := 4me
ifeq ($(OS),Windows_NT)
	TARGET_NAME := $(addsuffix .exe,$(TARGET_NAME))
endif
TARGET := $(BIN_PATH)/$(TARGET_NAME)
TARGET_DEBUG := $(DBG_PATH)/$(TARGET_NAME)

# src files & obj files
SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.c*)))
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))
OBJ_DEBUG := $(addprefix $(DBG_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

# clean files list
DISTCLEAN_LIST := $(OBJ) \
                  $(OBJ_DEBUG)
CLEAN_LIST := $(TARGET) \
			  $(TARGET_DEBUG) \
			  $(DISTCLEAN_LIST)

# default rule
default: makedir all

# non-phony targets
$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CC) $(COBJFLAGS) -o $@ $<

$(DBG_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CC) $(COBJFLAGS) $(DBGFLAGS) -o $@ $<

$(TARGET_DEBUG): $(OBJ_DEBUG)
	$(CC) $(CFLAGS) $(DBGFLAGS) $(OBJ_DEBUG) -o $@

# phony rules
.PHONY: makedir all debug clean distclean

makedir:
	@mkdir -p $(BIN_PATH) $(OBJ_PATH) $(DBG_PATH)

all: $(TARGET)

debug: $(TARGET_DEBUG)

clean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -f $(CLEAN_LIST)

distclean:
	@echo CLEAN $(DISTCLEAN_LIST)
	@rm -f $(DISTCLEAN_LIST)
