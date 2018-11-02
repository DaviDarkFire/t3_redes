CC = gcc
CFLAGS = -Wall -pedantic -std=c99
THREADFLAGS = -pthread
COMM = communication.c

xarpd: linked_list.c $(COMM) xarpd.c
	$(CC) $(CFLAGS) $(THREADFLAGS) $? -o $@

clean:
	-rm -f xarpd
