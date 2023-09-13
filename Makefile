# source files directory, i.e. '.c' or '.cpp' et cetera
SRC_DIR=src

OUT_DIR=target
# object files directory
RELEASE_LIB_DIR=$(OUT_DIR)/release/lib
DEBUG_LIB_DIR=$(OUT_DIR)/debug/lib
# header files directory
INCLUDE_DIR=include

# sets the compiler to be used
CC=clang
CFLAGS=--std=c99 -Wall -Wextra
RELEASE_CFLAGS=-O3 $(CFLAGS)
DEBUG_CFLAGS=-g $(CFLAGS)


# sets the output binary
BIN=maze-solver

RELEASE_BIN=$(OUT_DIR)/release/$(BIN)
DEBUG_BIN=$(OUT_DIR)/debug/$(BIN)
# finds all header files located in the include directory
HEADERS=$(shell find $(INCLUDE_DIR) -name '*.h')
# finds all source files located in the src directory
SRC_FILES=$(shell find $(SRC_DIR) -name '*.c')
# finds all object files located in the lib directory
RELEASE_OBJECTS=$(patsubst $(SRC_DIR)/%.c, $(RELEASE_LIB_DIR)/%.o, $(SRC_FILES))
DEBUG_OBJECTS=$(patsubst $(SRC_DIR)/%.c, $(DEBUG_LIB_DIR)/%.o, $(SRC_FILES))

.PHONY: debug
debug: $(DEBUG_BIN)

.PHONY: release
release: $(RELEASE_BIN)

# compiles source files to object files
$(DEBUG_LIB_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	mkdir -p $(dir $@)
	$(CC) $(DEBUG_CFLAGS) -o $@ -c $<

# builds binary from object files
$(DEBUG_BIN): $(DEBUG_OBJECTS)
	$(CC) $(DEBUG_CFLAGS) -o $@ $^


# compiles source files to object files
$(RELEASE_LIB_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	mkdir -p $(dir $@)
	$(CC) $(RELEASE_CFLAGS) -o $@ -c $<

# builds binary from object files
$(RELEASE_BIN): $(RELEASE_OBJECTS)
	$(CC) $(RELEASE_CFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -rf $(OUT_DIR)
