# # Makefile

# # Compiler
# CXX = g++

# # Directories
# SRC_DIR = src
# INCLUDE_DIR = include

# # Source files
# SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
# OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,%.o,$(SRC_FILES))

# # Output executable
# OUT = program

# all: $(OUT)

# $(OUT): $(OBJ_FILES)
# 	$(CXX) -o $(OUT) $(OBJ_FILES) -pthread

# %.o: $(SRC_DIR)/%.cpp
# 	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# clean:
# 	rm -f $(OUT) $(OBJ_FILES)

CXX = g++

# Compiler flags
CXXFLAGS = -std=c++20

# Directories
SRC_DIR = src
INCLUDE_DIR = include

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,%.o,$(SRC_FILES))

# Output executable
OUT = program

all: $(OUT)

$(OUT): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(OBJ_FILES) -pthread

# Compile each source file into an object file
%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Include dependency files, if they exist
-include $(OBJ_FILES:.o=.d)

# Generate dependency files for each source file
%.d: $(SRC_DIR)/%.cpp
	@set -e; rm -f $@; \
	$(CXX) -I$(INCLUDE_DIR) -MM $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

clean:
	rm -f $(OUT) $(OBJ_FILES) $(OBJ_FILES:.o=.d)