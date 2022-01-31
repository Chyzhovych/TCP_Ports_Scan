CC=gcc
CFLAGS=-c -Wall

all: scan

scan: scanport.o
	$(CC) scanport.o -o scan

scanport.o: scanport.c
	$(CC) $(CFLAGS) scanport.c

clean:
	rm -rf *.o scanport scan