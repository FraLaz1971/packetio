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
#define MAXDATA 32762
#define MAXWORD 32768
int debug=0;


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
// second 16 bits
    unsigned char sf;// sequence flag (2bit)
    unsigned short int ssc;// sequence counter (14bit)
// third 16 bits
    unsigned short int len;// packet length (16bit)
// following words
    unsigned short int  *data;// data field filled with n*16bit
// last byte
	unsigned char lastbyte;// last byte of the datafield if composed by
						  // an odd number of bytes
};

int swapword(unsigned short int *w){
  int debug = 0;
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
