CC = gcc
CFLAGS = -std=99 -g

all: dbishop

dbishop: dbishop.o
	$(CC) $(FLAGS) dbishop.c -o dbishop

dbishop.o: dbishop.c
	$(CC) $(FLAGS) -c dbishop.c

clean:
	rm -rf *.o
