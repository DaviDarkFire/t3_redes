CC = gcc
# CFLAGS = -Wall -pedantic -std=c99
THREADFLAGS = -pthread
MSC = misc.c
SRCCODES = linked_list.c arp_protocol.c xarpd.c

xarpd: linked_list.c $(MSC) arp_protocol.c xarpd.c
	$(CC)  $(THREADFLAGS) $(MSC) $(SRCCODES) -o $@

clean:
	-rm -f xarpd
