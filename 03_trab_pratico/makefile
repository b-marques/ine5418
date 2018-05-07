CPP_FILES   := $(wildcard src/*.cpp)
INCLUDE_DIR := include/
BIN_NAME    := process
CPP_FLAGS   := -o $(BIN_NAME) -std=c++14 -Wall -Werror -I$(INCLUDE_DIR)
L_FLAGS     := -lpthread
COMPILER    := g++ 

all:
	$(COMPILER) $(CPP_FILES) $(CPP_FLAGS) $(L_FLAGS)

doxygen:
	doxygen docs/config_doxygen
