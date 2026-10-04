CC = g++
CFLAGS = -std=c++17 -pedantic-errors -Wall -Wextra -O3

SRC_DIR = src
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)

SRC_FILES = $(wildcard $(SRC_DIR)/**/*.cc) $(wildcard $(SRC_DIR)/*.cc)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.cc=$(OBJ_DIR)/%.o)
EXEC = $(BIN_DIR)/quicksand

build: $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJ_FILES) -o $(EXEC)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

run: build
	./$(EXEC)

test: clean
	cmake -S . -B build && cmake --build build && cd build && ctest
