all:
	rm *.o
	$(CC) $(CFLAGS) antiopt.c -c -o antiopt.o
	$(CC) $(CFLAGS) uclibc.c -c -o uclibc.o
	$(CC) $(CFLAGS) mine.c -c -o mine.o
	$(CC) $(CFLAGS) musl.c -c -o musl.o
	$(CC) $(CFLAGS) fast.c -c -o fast.o
	$(CC) $(CFLAGS) diet.c -c -o diet.o
	$(CC) $(CFLAGS) bsd.c -c -o bsd.o
	$(CC) $(CFLAGS) $(LDFLAGS) bench.c *.o -o bench
