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
#include <time.h>
#define NMAX 512
#define MAXDATA 32768
#define MAXWORD 32771
#define TIMEOFFS 935280002.649755
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

void leftpad(char *s,char c){
  int i,slen;
  slen=strlen(s);
  for(i=0;(i<slen)&&(s[i]==' ');++s){
    s[i]=c;
  }
  return ;
}

// yyyy-MM-ddThh:mm:ss
void getBC_UTC(char *bcstrt, double ts){
	int year,mon,day,hour,min,sec;
    char smon[3],sday[3],shour[3],smin[3],ssec[3];
	char *dt;
	double mytime=ts;
	time_t mytimet=(time_t)mytime; /* input time seconds */
        double bcoffset = TIMEOFFS;
	double bctime=mytime+bcoffset;
	time_t bctimet=(time_t)bctime;
	time_t bt;
	double mytimeud; /* input time microseconds */
	unsigned int mytimeu;
	mytimeud = (mytime-(double)mytimet)*1000000.0;
	mytimeu = (unsigned int)round(mytimeud);
	if (debug) printf("read input time: %f\n",mytime);
	if (debug) printf("input time seconds: %ld\n",mytimet);
	if (debug) printf("input time microseconds (double): %f\n",mytimeud);
	if (debug) printf("input time microseconds (uint): %u\n",mytimeu);
	// Structure to store local time
    struct tm* ptr;
    // Variable to store current time
    time_t t;
    ptr = gmtime(&bctimet); 
    if (debug) printf("UTC: %s\n", asctime(ptr));    
  year=ptr->tm_year+1900;
  mon=ptr->tm_mon+1;
  snprintf(smon,3,"%2d",mon);
  snprintf(sday,3,"%2d",ptr->tm_mday);
  snprintf(shour,3,"%2d",ptr->tm_hour);
  snprintf(smin,3,"%2d",ptr->tm_min);
  snprintf(ssec,3,"%2d",ptr->tm_sec);
 
  leftpad(smon,'0'); 
  leftpad(sday,'0');
  leftpad(shour,'0');
  leftpad(smin,'0');
  leftpad(ssec,'0');
  
  snprintf(bcstrt,32,"%d-%2s-%2s:%2s:%2s:%2s",year,smon,\
  sday,shour,smin,ssec);
  if (debug) printf("getBC_UTC():BC Tm str: %s\n",bcstrt);
	return;
}
