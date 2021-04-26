# Makefile for gradfun portion

PROG = gradFun
OBJS = gradFun.o
LIBS = -lm

CFLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc

.PHONY: clean all

all: gradFun

gradFun: gradFun.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@


clean:
	rm -f *~ *.o
	rm -f $(PROG)
	rm -rf *.dSYM
