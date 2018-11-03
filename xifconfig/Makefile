CC = gcc
CFLAGS = -Wall -pedantic -g
COMM = ../communication.c

xifconfig: $(COMM) xifconfig.c
	$(CC) $(CFLAGS) $? -o $@

clean:
	-rm -f xifconfig
