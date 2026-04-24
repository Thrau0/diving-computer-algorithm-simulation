# Makefile -- dive decompression simulation
#
# Usage:
#   make            build compare
#   make run        build and run
#   make clean      remove compiled binary and output files
CC     = gcc
CFLAGS = -O2 -Wall

all: comp

comp: compare.c profile.h buehlmann.h rgbm.h
	$(CC) $(CFLAGS) -o comp compare.c -lm

run: all
	./comp

clean:
	rm -f comp *.txt *.dat