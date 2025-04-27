# Compiler and flags
CC = gcc
CXX = g++
CFLAGS = -w -std=c++20
CXXFLAGS = $(CFLAGS)
 
# Find SDL3 packages using pkg-config
PKGS = sdl3 sdl3-image sdl3-ttf libxml-2.0
CFLAGS += $(shell pkg-config --cflags $(PKGS))
CXXFLAGS += $(shell pkg-config --cflags $(PKGS))
LIBS = $(shell pkg-config --libs $(PKGS))
 
# Directories
SRC_DIR = SDL3Test
OBJ_DIR = obj
BIN_DIR = bin
 
# Find all source files
SRCS = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(filter %.c,$(SRCS))) \
    $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(filter %.cpp,$(SRCS)))
 
# Target executable name
TARGET = $(BIN_DIR)/LairwareSDL3
 
# Default target
all: dirs $(TARGET)
 
# Create necessary directories
dirs:
    @mkdir -p $(OBJ_DIR) $(BIN_DIR)
 
# Link object files
$(TARGET): $(OBJS)
    $(CXX) -o $@ $^ $(LIBS)
 
# Compile C source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
    $(CC) $(CFLAGS) -c $< -o $@
 
# Compile C++ source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
    $(CXX) $(CXXFLAGS) -c $< -o $@
 
# Clean up
clean:
    rm -rf $(OBJ_DIR) $(BIN_DIR)
 
# Run the application
run: all
    $(TARGET)
 
.PHONY: all dirs clean run