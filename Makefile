SRC_DIR = src
BUILD_DIR = build
VENDOR = vendor

OUT := $(BUILD_DIR)/libeagle.a

ifeq ($(OS),Windows_NT)
	CALL := call
	DELETE := powershell.exe rm
	AND := &
else
	CALL :=
	DELETE := rm
	AND := ;
endif

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/objs/%.o, $(SRCS))
HEADERS := $(wildcard $(SRC_DIR)/*.h)

TEST := test
TEST_BINS := $(patsubst $(TEST)/%.c,$(TEST)/tests/%.exe,$(wildcard $(TEST)/*.c))
TEST_FRAMEWORK := $(wildcard $(VENDOR)/Unity/*.c)

CC := clang
CFLAGS := -Wall -Wextra -pedantic -std=c99 -g -gcodeview
LFLAGS :=

.PHONY: all clean test runtests

all: $(OUT)

$(OUT): $(OBJS)
	llvm-ar rc $@ $^ $(LFLAGS)

$(BUILD_DIR)/objs/%.o: $(SRC_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

test: $(TEST_BINS)

runtests: test
	$(foreach var,$(TEST_BINS),$(CALL) "$(var)"$(AND))

$(TEST)/tests/%.exe: $(TEST)/%.c $(OUT) $(TEST_FRAMEWORK)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	$(DELETE) $(OUT)
	$(DELETE) $(BUILD_DIR)/objs/*
	$(DELETE) $(TEST)/tests/*
