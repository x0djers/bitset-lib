CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11 -DDEBUG

BUILD_DIR = build

TARGET = $(BUILD_DIR)/bitset-main
SRC_DIR = src
OBJ_DIR = $(BUILD_DIR)/obj
SOURCES = $(shell find $(SRC_DIR) -name "*.c")
SOURCES := $(filter-out $(SRC_DIR)/tests/%, $(SOURCES))
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

TEST_EXEC = $(BUILD_DIR)/test
TESTS_SRC_DIR = tests
TESTS_OBJ_DIR = $(OBJ_DIR)/tests
TESTS_SOURCES = $(shell find $(TESTS_SRC_DIR) -name "*.c")
TESTS_OBJECTS = $(TESTS_SOURCES:$(TESTS_SRC_DIR)%.c=$(TESTS_OBJ_DIR)%.o)
TESTS_DEPENDENCIES = $(filter-out $(OBJ_DIR)/main.o, $(OBJECTS)) $(TESTS_OBJECTS)

all: $(TARGET)

$(TARGET): $(OBJECTS) $(BUILD_DIR)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

$(TEST_EXEC): $(TESTS_OBJECTS) $(OBJECTS) $(BUILD_DIR)
	$(CC) $(CFLAGS) $(TESTS_DEPENDENCIES) -o $(TEST_EXEC)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)/$(dir $*)
	$(CC) $(CFLAGS) -c $< -o $@

$(TESTS_OBJ_DIR)%.o: $(TESTS_SRC_DIR)%.c
	@mkdir -p $(TESTS_OBJ_DIR)$(dir $*)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_EXEC)
	./$(TEST_EXEC)

valgrind: $(TEST_EXEC)
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./$(TEST_EXEC)

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean all

clang-format:
	@clang-format --style=file -i $$(find . -name "*.c")
	@clang-format --style=file -i $$(find . -name "*.h")