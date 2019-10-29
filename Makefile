# 2020-robot-code Makefile
CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -pthread -lm

all: bin/robot

bin/robot: src/robot.c
	@mkdir -p bin
	$(CC) $(CFLAGS) src/robot.c -o bin/robot
