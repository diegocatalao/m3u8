UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    EXT = so
else ifeq ($(UNAME_S),Darwin)
    EXT = dylib
else ifeq ($(OS),Windows_NT)
    EXT = dll
else
  $(error "Operating system not supported")
endif

PROJECT = libm3u8.$(EXT)

CC = gcc
DEPENDENCIES = libcurl
CFLAGS = -std=c99 -g -Wall -fPIC $(shell pkg-config --cflags ${DEPENDENCIES}) -pthread
LDFLAGS = -shared $(shell pkg-config --libs ${DEPENDENCIES})
TFLAG = $(if $(TARGET),-target $(TARGET),)

SRC_DIR = src
OBJ_DIR = obj
LIB_DIR = lib

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(OBJ_DIR) $(LIB_DIR) $(LIB_DIR)/$(PROJECT)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

$(LIB_DIR)/$(PROJECT): $(OBJ_FILES)
	$(CC) $(TFLAG) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(TFLAG) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJ_DIR)/*.o $(LIB_DIR)/*.dylib $(LIB_DIR)/*.so $(LIB_DIR)/$(PROJECT)
	rmdir $(OBJ_DIR) $(LIB_DIR) 2>/dev/null || true

.PHONY: all install clean

