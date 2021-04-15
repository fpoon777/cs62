# Makefile for Q3 and Q5

PROG = rsa cbcctr
OBJS = rsa.o cbcctr.o
LIBS = -lm

CFLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc

.PHONY: clean all

all: rsa cbcctr

rsa: rsa.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

cbcctr: cbcctr.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@


clean:
	rm -f *~ *.o
	rm -f $(PROG)
	rm -rf *.dSYM
