# piconzero/Makefile
#
# @author Connor Henley, @thatging3rkid

# include defaults from core
include ../core/defaults.mk

# setup directories
OBJ_DIR=../obj
TEST_DIR=../test
TEST_OBJ_DIR=$(TEST_DIR)/obj

# allow GCC to find local includes
CFLAGS += -iquote..

all: $(OBJ_DIR)/piconzero.o

$(OBJ_DIR)/piconzero.o: piconzero.c piconzero.h
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

### Required recipes

ci-build: all
ci-test:  ; # do nothing
