#This is a Makefile

CC = gcc
CFLAGS = -g -Wall
PROGS = Logger Driver Encrypter

all: logger encrypter driver
logger:
	$(CC) $(CFLAGS) Logger.c -o Logger
encrypter:
	$(CC) $(CFLAGS) Encrypter.c -o Encrypter
driver:
	$(CC) $(CFLAGS) Driver.c -o Driver
run:
	./Driver output.txt
clr:
	rm -f $(PROGS) *.txt
