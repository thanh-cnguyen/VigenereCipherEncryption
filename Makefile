#This is a Makefile

CC = gcc
CFLAGS = -g -Wall
PROGS = Logger

all:
	$(CC) $(CFLAGS) $(PROGS).c -o $(PROGS)
run:
	./$(PROGS) $(PROGS).txt
clr:
	rm -f $(PROGS) *.txt
