.PHONY: clean, mrproper
CC = gcc
CFLAGS = -g -Wall -lm

all: framebuffer

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

framebuffer: framebuffer.o
	$(CC) $(CFLAGS) -o $@ $+

clean: mrproper
	rm -f *.o core.*

mrproper: 
	rm -f framebuffer

