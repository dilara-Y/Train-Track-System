# Compiler and flags
CXX = g++
CXXFLAGS = -g -std=c++11 -Wall
# Directories
OBJ_DIR = obj
BIN_DIR = bin

# Executable name
TARGET = $(BIN_DIR)/HUTrain

# Source and object files
SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Default target
all: $(TARGET)

# Ensure directories exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Link all objects into final executable
$(TARGET): $(BIN_DIR) $(OBJ_DIR) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile source files into object files
$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Run the program
run: all
	./bin/HUTrain io/input.txt

# Run with Valgrind
mem: all
	valgrind --leak-check=full --track-origins=yes ./bin/HUTrain io/input.txt

.PHONY: all clean run mem
