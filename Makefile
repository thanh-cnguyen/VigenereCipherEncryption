#This is a Makefile

CC = gcc
CFLAGS = -g -Wall
PROGS = Logger Driver Encrypter

all: logger encrypter driver
logger:
	$(CC) $(CFLAGS) Logger.c -o $(PROGS)
encrypter:
	$(CC) $(CFLAGS) Encrypter.c -o $(PROGS)
driver:
	$(CC) $(CFLAGS) Driver.c -o $(PROGS)
run:
	./$(PROGS) $(PROGS).txt
clr:
	rm -f $(PROGS) *.txt
