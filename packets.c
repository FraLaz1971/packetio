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
#include "packets.h"


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

int countpackets(char *fname){
  FILE *ifp;
  unsigned short int w,len;
  unsigned char b;
  unsigned long long cnt;
  int i,res=1;
  ifp=fopen(fname,"rb");
  if(!ifp){
    fprintf(stderr,"error in opening file %s\n",fname);
    return 1;
  }

  /* loop on all the packets */
  cnt=0;
  while(res){
    res=fread(&w,2,1,ifp);
    if(!res) break;
    res=fread(&w,2,1,ifp);
    if(!res) break;
    res=fread(&len,2,1,ifp);
    if(!res) break;
    swapword(&len);
    for(i=0;i<len+1;i++){
      res=fread(&b,1,1,ifp);
      if(!res) break;
    }
    cnt++;
  }
  if(debug) printf("read %lld packets\n",cnt);
  fclose(ifp);
  return cnt;
}

int copypacket(struct Packet *dest, struct Packet *src ){
    if (debug) printf("copypacket() start");
    unsigned long int i;
// first 16 bits
    dest->ver = src->ver;
    dest->type = src->type;
    dest->shf = src->shf;
    dest->apid = src->apid;
//(
    dest->pid = src->pid;
    dest->cat = src->cat;
//)
// second 16 bits
    dest->sf = src->sf;
    dest->ssc = src->ssc;
// third 16 bits
    dest->len = src->len;// packet length (16bit)
    // following bytes
    if (debug) printf("copypacket() now coying data field bytes");
    for(i=0; i<dest->len+1; i++){
      dest->bv[i] = src->bv[i];
    }
    if (debug) printf("copypacket() end");
  return 0;
}

int swappacket(struct Packet *p1, struct Packet *p2 ){
  struct Packet temp;
  temp.bv = (unsigned char *) malloc(65536*sizeof(unsigned char));
  if (debug) printf("swappacket() start");
  int res;
  if (debug) printf("swappacket() going to exec copypacket(&temp, p2)");
  res = copypacket(&temp, p2);
  if (debug) printf("swappacket() going to exec copypacket(p2,p1)");
  res = copypacket(p2,p1);
  if (debug) printf("swappacket() going to exec copypacket(p1,&temp)");
  res = copypacket(p1,&temp);
  if (debug) printf("swappacket() end");
  free(temp.bv);
  return 0;
}

int sortpackets(struct Packet *pv, unsigned long dim){
 unsigned long long cnt,i,j;
 return 0;
}
void showpacket(struct Packet *p){
  unsigned long int i;
  printf("ver: %u\n",p->ver);
  printf("type: %u\n",p->type);
  printf("shf: %u\n",p->shf);
  printf("apid: %hu\n",p->apid);
  printf("pid: %u\n",p->pid);
  printf("cat: %u\n",p->cat);
  printf("sf: %u\n",p->sf);
  printf("ssc: %u\n",p->ssc);
  printf("len: %hu\n",p->len);
  for(i=0; i<p->len+1; i++){
    printf("bv[%lu]: %u\n",i,p->bv[i]);
}
 return ;
}
