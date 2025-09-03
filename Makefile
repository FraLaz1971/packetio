CC=gcc -Wall
LD=gcc
RM=rm -f
CFLAGS=-c
LDFLAGS=-lm

.PHONY: all clean packets
all: writepackets readpackets

readpackets.o: readpackets.c packets.h
	$(CC) $(CFLAGS) $<

readpackets: readpackets.o
	$(LD) $< -o $@ $(LDFLAGS)

writepackets.o: writepackets.c packets.h
	$(CC) $(CFLAGS) $<

writepackets: writepackets.o
	$(LD) $< -o $@ $(LDFLAGS)

packets: packets.ccsds

packets.ccsds:
	./writepackets <<< "3"
clean:
	$(RM) *.o writepackets readpackets packets.ccsds *.log
