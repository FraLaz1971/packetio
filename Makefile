CC=gcc -Wall
LD=gcc
RM=rm -f
CFLAGS=-c
LDFLAGS=-o

.PHONY: all clean packets
all: writepackets readpackets

readpackets.o: readpackets.c packets.h
	$(CC) $(CFLAGS) $<

readpackets: readpackets.o
	$(LD) $< $(LDFLAGS) $@

writepackets.o: writepackets.c packets.h
	$(CC) $(CFLAGS) $<

writepackets: writepackets.o
	$(LD) $< $(LDFLAGS) $@

packets: packets.ccsds

packets.ccsds:
	./writepackets <<< "3"
clean:
	$(RM) *.o writepackets readpackets packets.ccsds *.log
