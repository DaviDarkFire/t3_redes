CC = gcc
CFLAGS = -Wall -pedantic -std=c99
THREADFLAGS = -pthread
COMM = communication.c
SRCCODES = linked_list.c xarpd.c

xarpd: linked_list.c $(COMM) xarpd.c
	$(CC) $(CFLAGS) $(THREADFLAGS) $(COMM) $(SRCCODES) -o $@

clean:
	-rm -f xarpd
