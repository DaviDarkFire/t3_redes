CC = gcc
CFLAGS = -Wall -pedantic -std=c99 -g
MSC = ../misc.c

xarp: $(MSC) xarp.c
	$(CC) $(CFLAGS) $? -o $@

clean:
	-rm -f xarp
