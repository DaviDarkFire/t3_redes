CC = gcc
CFLAGS = -Wall -pedantic -std=c99 -g
COMM = ../communication.c

xarp: $(COMM) xarp.c
	$(CC) $(CFLAGS) $? -o $@

clean:
	-rm -f xarp
