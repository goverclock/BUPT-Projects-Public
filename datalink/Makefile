CC=gcc
CFLAGS=-O2 -Wall

datalink: datalink.o protocol.o lprintf.o crc32.o
	gcc datalink.o protocol.o lprintf.o crc32.o -o datalink -lm

SR: SR.o protocol.o lprintf.o crc32.o
	gcc SR.o protocol.o lprintf.o crc32.o -o SR -lm

GBN: GBN.o protocol.o lprintf.o crc32.o
	gcc GBN.o protocol.o lprintf.o crc32.o -o GBN -lm

clean:
	${RM} *.o  *.log datalink SR GBN
