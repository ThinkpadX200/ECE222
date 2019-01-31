CC = gcc
CFLAGS = -g -Wall
COMP_LIBS = -lncurses

all: $(patsubst %.c, %.out, $(wildcard *.c))

%.out: %.c Makefile
	$(CC) $(CFLAGS) $< -o $@ $(COMP_LIBS)

clean:
	rm -f *.out

