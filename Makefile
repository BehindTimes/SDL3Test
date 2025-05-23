# Compiler and flags
CC = gcc
CXX = g++
CFLAGS = -w -std=c++20
CXXFLAGS = $(CFLAGS)

HAVE_SDL3_MIXER = $(shell ! pkg-config --libs sdl3-mixer 2> /dev/null ; echo $$?)
 
# Find SDL3 packages using pkg-config
PKGS = sdl3 sdl3-image sdl3-ttf libxml-2.0
CFLAGS += $(shell pkg-config --cflags $(PKGS))
CXXFLAGS += $(shell pkg-config --cflags $(PKGS)) -DHAVE_SDL3_MIXER=$(HAVE_SDL3_MIXER)
LIBS = $(shell pkg-config --libs $(PKGS))
 
# Directories
SRC_DIR = SDL3Test
OBJ_DIR = obj
BIN_DIR = Bin
 
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