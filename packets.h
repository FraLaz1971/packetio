#ifndef PACKETS_H
#define PACKETS_H
/*
 * packets.h
 * 
 * Copyright 2025 francesco <francesco.lazzarotto@inaf.it>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
#include <stdio.h>
#include <limits.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NMAX 512
#define MAXDATA 32768
#define MAXWORD 32771
#define TIMEOFFS 935280002.649755
static int debug=0;

struct AGL_DFH{
	unsigned int syncmarkMSW; /* 16 bits 0xFFDB */
	unsigned int syncmarkLSW; /* 16 bits 0x9255 */
	unsigned char CRCflag;/* 2 bits 3 */
	unsigned char type; /* 6 bits service type [32,63] */
	unsigned char subtype; /* 8 bit service subtype [0,255] */
	unsigned int MMpageID; /* 16 bits mass storage page identifier */
	unsigned int MMpktcnt; /* 16 bits mass storage packet counter */
	unsigned char century; /* 8 bits current century 0x20  */
	unsigned char APIDseqcnt; /* 8 bits APID sequence counter  */
	unsigned char OBTsb0; /* 8 bits OBT s byte 0 */
	unsigned char OBTsb1; /* 8 bits OBT s byte 1 */
	unsigned char OBTsb2; /* 8 bits OBT s byte 2 */
	unsigned char OBTsb3; /* 8 bits OBT s byte 3 */
	unsigned char OBTsb4; /* 8 bits OBT s byte 4 */
	unsigned char OBTusbn; /* 4 bits OBT us  2^19-2^16 */
	unsigned char OBTusb1; /* 8 bits OBT us  2^15-2^8 */
	unsigned char OBTusb2; /* 8 bits OBT us 2^7-2^0  */
	unsigned char mode; /* 8 bits  P/L operational mode in which the Source Packet was generated */
	unsigned char PktStructID; /* 4 bits Packet Structure ID. val=1 for the first version */
	unsigned char RedundID; /* 4 bits Redundancy ID value=1 */
	double OBTtime; /* OBT time reconstructed from all the fields elapsed time since 00:00:00, January 6, 1980 */
};


struct BC_DFH{
	unsigned char spare0; /* 1 bit 0b0 */
	unsigned char pus_ver; /* 3 bit PUS Versioni 0b001  */
	unsigned char spare1; /* 4 bit 0b0000 */
	unsigned char type; /* 8 bit service type */
	unsigned char subtype; /* 8 bit service subtype */
	unsigned char dest; /* 8 bit destination ID */
	unsigned int sec_msw; /* 16 bit seconds MSW */
	unsigned int sec_lsw; /* 16 bit seconds LSW */
	unsigned int subsec; /* 16 bit subseconds s*(1/2^16) */	
	double time; /* time reconstructed from all the fields */
};
struct DDSheader {
	/* SCET */
	unsigned int sec_msw; /* 16 bit */
	unsigned int sec_lsw; /* 16 bit */
	unsigned int usec_msw; /* 16 bit */
	unsigned int usec_lsw; /* 16 bit */
	unsigned long int pkt_len; /*32 bit n. of bytes in the data packet excluding the dds header */
	unsigned short gr_st_id; /* 16 bit ground station ID*/
	unsigned short vc_id; /* 16 bit virtual channel ID*/
	unsigned char sle_serv; /* 8 bit sle service channel and data type */
	unsigned char time_qual; /*8 bit, 0 = good, 1 = inaccurate, 2 = bad */
	double time;
};

// Define the structure to store ccsds telemetry packets
struct Packet {
// first 16 bits
    unsigned char ver; // version number (3bit)
    unsigned char type;// packet type (1bit)
    unsigned char shf;// secondary header flag (1bit)
    unsigned short int apid;// (11bit)
/* adding PID and CAT P(ID<<4)+CAT == APID */
    unsigned short pid; /* process ID*/
    unsigned char cat; /* category */
// second 16 bits
    unsigned char sf;// sequence flag (2bit)
    unsigned short int ssc;// sequence counter (14bit)
// third 16 bits
    unsigned short int len;// packet length (16bit)
    struct BC_DFH dfh;
    struct AGL_DFH adfh;
// following words
    unsigned short int  *data;// data field filled with n*16bit
// last byte
	unsigned char lastbyte;// last byte of the datafield if composed by
						  // an odd number of bytes
	unsigned char *bv;
	unsigned int CRC; /* Packet Error Control (16 bits): CRC checksum */
};

int swapword(unsigned short int *w);

void leftpad(char *s,char c);

// yyyy-MM-ddThh:mm:ss
void getBC_UTC(char *bcstrt, double ts);

int countpackets(char *fname);

int copypacket(struct Packet *dest, struct Packet *src );

int swappacket(struct Packet *p1, struct Packet *p2 );

int sortpackets(struct Packet *pv, unsigned long dim);

void showpacket(struct Packet *p);

#endif //PACKETS_H
