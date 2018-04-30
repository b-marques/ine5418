CPP_FILES := $(wildcard src/*.cpp)
INCLUDE_DIR := include/
BIN_NAME := process
COMPILE_FLAGS := -o $(BIN_NAME) -std=c++14 -Wall -Werror -I$(INCLUDE_DIR)
COMPILER := g++ 

all:
	$(COMPILER) $(CPP_FILES) $(COMPILE_FLAGS)