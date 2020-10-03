# 2020-robot-code Makefile
#
# @author Connor Henley, @thatging3rkid

# include defaults from core
include ./core/defaults.mk

# setup directories
OBJ_DIR=./obj
TEST_DIR=./test
TEST_OBJ_DIR=$(TEST_DIR)/obj

# allow GCC to find local includes
CFLAGS += -iquote.

all: $(OBJ_DIR)/robot

$(OBJ_DIR)/robot.o: robot.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/robot: $(OBJ_DIR)/robot.o
	@mkdir -p $(OBJ_DIR)
	$(CXX) -o $@ $^ $(LD_FLAGS)

.PHONY: run
run: $(OBJ_DIR)/robot
	@./$<

### Required recipes

ci-build: all
ci-test:  ; # do nothing
