# Makefile for bwcalc

CC ?= cc   		# use environment/compiler override if provided
CFLAGS ?= -Wall -Wextra -Wpedantic
LDFLAGS = -lm   # Math library

all: bwcalc

# This rule executes: cc bwcalc.c -o bwcalc -lm
bwcalc: bwcalc.c bwcalc.h
	$(CC) $(CFLAGS) bwcalc.c -o bwcalc $(LDFLAGS)
