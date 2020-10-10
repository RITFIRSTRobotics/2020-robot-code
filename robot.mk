# 2020-robot-code Makefile
#
# @author Connor Henley, @thatging3rkid

# include defaults from core
include ./core/defaults.mk

# setup directories
OBJ_DIR=./obj
CORE_OBJ_DIR=./core/obj
TEST_DIR=./test
TEST_OBJ_DIR=$(TEST_DIR)/obj

# allow GCC to find local includes
CFLAGS += -iquote.
CXXFLAGS += -iquote.

all: $(OBJ_DIR)/robot

$(OBJ_DIR)/robot.o: robot.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/robot: $(OBJ_DIR)/robot.o $(OBJ_DIR)/llpiconzero.o $(OBJ_DIR)/piconzero.o $(CORE_OBJ_DIR)/i2cUtils.o
	@mkdir -p $(OBJ_DIR)
	$(CXX) -o $@ $^ $(LD_FLAGS) -lncurses

.PHONY: run
run: $(OBJ_DIR)/robot
	@./$<

### Dependency recipes

$(CORE_OBJ_DIR)/i2cUtils.o: ./core/i2c/i2cutils.c ./core/i2c/i2cutils.h
	make -C ./core/i2c $@

$(OBJ_DIR)/llpiconzero.o: ./piconzero/piconzero.c ./piconzero/piconzero.h
	make -C ./piconzero $@

$(OBJ_DIR)/piconzero.o: ./piconzero/PiconZero.cpp ./piconzero/PiconZero.hpp
	make -C ./piconzero $@

### Required recipes

ci-build: all
ci-test:  ; # do nothing
