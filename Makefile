# generated by ai

# Define the source directory and the build directories
SRC_DIR := code
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
OUTPUT_DIR := ./f

# Define the compiler and the flags
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++20 -I $(SRC_DIR)/include

# Find all .cpp files in the source directory
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)

# Generate the list of .o files based on the sources
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

# Define the target executable
TARGET := $(OUTPUT_DIR)/demo

# Default target
all: $(TARGET)

# Rule to link the final executable
$(TARGET): $(OBJECTS)
	@mkdir -p $(OUTPUT_DIR)
	$(CXX) $(OBJECTS) -o $@

# Rule to compile .cpp files to .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build directory
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Phony targets
.PHONY: all clean
