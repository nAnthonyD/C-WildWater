CC=gcc
CFLAGS=-std=c99 -Wall -Wextra

all: c-wildwater

c-wildwater:
	$(CC) $(CFLAGS) -o c-wildwater *.c

clean:
	rm -f c-wildwater

.PHONY: all clean
