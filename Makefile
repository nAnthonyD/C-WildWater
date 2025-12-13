all: c-wildwater

c-wildwater:
	gcc -std=c99 -o c-wildwater codeC/*.c

clean:
	rm -f c-wildwater

.PHONY: all clean
