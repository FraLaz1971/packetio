CC=gcc
LD=gcc
RM=rm -f
CFLAGS=-c
LDFLAGS=
LIBS=-lm
.PHONY: all clean packets
all: writepackets readpackets writeDDSpackets readDDSpackets strip_prefix \
     countpackets testcopypkt sortpackets swappacket writeagilepackets \
     readagilepackets

packets.o: packets.c packets.h
	$(CC) $(CFLAGS) $<

readpackets.o: readpackets.c packets.h
	$(CC) $(CFLAGS) $<

readpackets: readpackets.o packets.o
	$(LD) $^ -o $@ $(LDFLAGS) $(LIBS)

writepackets.o: writepackets.c packets.h
	$(CC) $(CFLAGS) $<

writepackets: writepackets.o packets.o
	$(LD) $^ -o $@ $(LDFLAGS) $(LIBS)

writeagilepackets.o: writeagilepackets.c packets.h
	$(CC) $(CFLAGS) $<

writeagilepackets: writeagilepackets.o packets.o
	$(LD) $^ -o $@ $(LDFLAGS) $(LIBS)

readagilepackets.o: readagilepackets.c packets.h
	$(CC) $(CFLAGS) $<

readagilepackets: readagilepackets.o packets.o
	$(LD) $^ -o $@ $(LDFLAGS) $(LIBS)

readDDSpackets.o: readDDSpackets.c packets.h
	$(CC) $(CFLAGS) $<

readDDSpackets: readDDSpackets.o packets.o
	$(LD) $^ -o $@ $(LDFLAGS) $(LIBS)

writeDDSpackets.o: writeDDSpackets.c packets.h
	$(CC) $(CFLAGS) $<

writeDDSpackets: writeDDSpackets.o packets.o
	$(LD) $^ -o $@ $(LDFLAGS) $(LIBS)

strip_prefix.o: strip_prefix.c packets.h
	$(CC) $(CFLAGS) $<

strip_prefix: strip_prefix.o packets.o
	$(LD) $^ -o $@ $(LDFLAGS) $(LIBS)

countpackets.o: countpackets.c packets.h
	$(CC) $(CFLAGS) $<

countpackets: countpackets.o packets.o
	$(LD) $^ -o $@ $(LDFLAGS) $(LIBS)

testcopypkt.o: testcopypkt.c packets.h
	$(CC) $(CFLAGS) $<

testcopypkt: testcopypkt.o packets.o
	$(LD) $^ -o $@ $(LDFLAGS) $(LIBS)

swappacket.o: swappacket.c packets.h
	$(CC) $(CFLAGS) $<

swappacket: swappacket.o packets.o
	$(LD) $^ -o $@ $(LDFLAGS) $(LIBS)

sortpackets.o: sortpackets.c packets.h
	$(CC) $(CFLAGS) $<

sortpackets: sortpackets.o packets.o
	$(LD) $^ -o $@ $(LDFLAGS) $(LIBS)

packets: packets.ccsds

packets.ccsds:
	./writepackets <<< "3"
clean:
	$(RM) *.o writepackets readpackets readDDSpackets writeDDSpackets strip_prefix countpackets testcopypkt sortpackets swappacket packets.ccsds *.log
