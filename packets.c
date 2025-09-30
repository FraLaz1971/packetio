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
#include <stdlib.h>
#include <string.h>
#include <math.h> 
#include "packets.h"
#define NMAX 512
#define MAXDATA 32768
#define MAXWORD 32771
#define TIMEOFFS 935280002.649755


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
