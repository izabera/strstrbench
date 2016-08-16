CC = gcc
CFLAGS = -Ofast -march=native -Wall -Wextra
LDFLAGS = -flto

.PHONY: all

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

all: bench

bench: antiopt.o uclibc.o plan9.o mine.o musl.o fast.o diet.o bsd.o bench.c Makefile
	$(CC) $(CFLAGS) $(LDFLAGS) bench.c *.o -o bench
