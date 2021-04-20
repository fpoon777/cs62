# Makefile for Q3 and Q5

PROG = rsa cbcctr q3hw2
OBJS = rsa.o cbcctr.o q3hw2.o
LIBS = -lm

CFLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc

.PHONY: clean all

all: rsa cbcctr

rsa: rsa.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

cbcctr: cbcctr.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

q3hw2: q3hw2.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

clean:
	rm -f *~ *.o
	rm -f $(PROG)
	rm -rf *.dSYM
