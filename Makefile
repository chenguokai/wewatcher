CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

all: wewatcher

wewatcher: wewatcher.o
	$(CC) $(CFLAGS) -o wewatcher wewatcher.o

wewatcher.o: wewatcher.c
	$(CC) $(CFLAGS) -c wewatcher.c

clean:
	rm -f wewatcher *.o
