CC = gcc
# CFLAGS = -Wall -pedantic -std=c99
THREADFLAGS = -pthread
COMM = communication.c
SRCCODES = linked_list.c arp_protocol.c xarpd.c

xarpd: linked_list.c $(COMM) arp_protocol.c xarpd.c
	$(CC)  $(THREADFLAGS) $(COMM) $(SRCCODES) -o $@

clean:
	-rm -f xarpd
