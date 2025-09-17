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
#include <limits.h>
#define NMAX 512
#define MAXDATA 32768
#define MAXWORD 32771
int debug=0;

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
// following words
    unsigned short int  *data;// data field filled with n*16bit
// last byte
	unsigned char lastbyte;// last byte of the datafield if composed by
						  // an odd number of bytes
};

int swapword(unsigned short int *w){
  int status = 0;
  unsigned short int b1,b2;
  if (debug) printf("input word:%d\n",*w);
  b1=(*w)&255;
  b2=(*w)>>8;
  if (debug) printf("MSB:%d\n",b2);
  if (debug) printf("LSB:%d\n",b1);
  if (debug) printf("after swap:\n");
  *w=(b1<<8)+b2;
  if (debug) printf("output word:%d\n",*w);
  return status;
}
