# Makefile for Q3 and Q5

PROG = q3hw2
OBJS = q3hw2.o
LIBS = -lm

CFLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc

.PHONY: clean all

all: q3hw2

q3hw2: q3hw2.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

clean:
	rm -f *~ *.o
	rm -f $(PROG)
	rm -rf *.dSYM
