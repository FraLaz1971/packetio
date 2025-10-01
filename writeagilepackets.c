#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "packets.h"
int main() {
   unsigned short int *word;
   unsigned long long int i,j,npkt;
   unsigned short int length;// length in bytes-1
   char fname[32];
   unsigned char mybyte;
   // decide the number of packets
   do{
      puts("enter n. of packets");
      scanf("%lld",&npkt);
    } while ((npkt>ULONG_MAX)||(npkt<1));
   // decide the file name
   puts("enter file name");
   scanf("%s",fname);
   puts("enter packet length-1");
   scanf("%hd",&length);
   // Open the binary file for writing
   FILE *wf = fopen(fname, "wb");

   // Check if file open successfully
   if (!wf) { 
       printf("Cannot open file!\n");
       return 1;
   }
   // Initialize packet data
   struct Packet wpkt[NMAX];
 for(i=0;i<npkt;i++){
   wpkt[i%NMAX].ver = 4;
   wpkt[i%NMAX].type = 0;
   wpkt[i%NMAX].shf = 1;
   wpkt[i%NMAX].apid = 767;
   wpkt[i%NMAX].sf = 3;
   wpkt[i%NMAX].ssc = 100+i;
   wpkt[i%NMAX].len = length;
   // AGILE DFH
   wpkt[i%NMAX].adfh.syncmarkMSW = 0xFFDB;
   wpkt[i%NMAX].adfh.syncmarkLSW = 0x9255;
   wpkt[i%NMAX].adfh.CRCflag = 3;
   wpkt[i%NMAX].adfh.type = 32;
   wpkt[i%NMAX].adfh.subtype = 1;
   wpkt[i%NMAX].adfh.MMpageID = 128;
   wpkt[i%NMAX].adfh.MMpktcnt = 255;
   wpkt[i%NMAX].adfh.century = 32;
   wpkt[i%NMAX].adfh.APIDseqcnt = 50+i;
   wpkt[i%NMAX].adfh.OBTsb0 = 0;
   wpkt[i%NMAX].adfh.OBTsb1 = 0;
   wpkt[i%NMAX].adfh.OBTsb2 = 0;
   wpkt[i%NMAX].adfh.OBTsb3 = 0;
   wpkt[i%NMAX].adfh.OBTsb4 = i;
   wpkt[i%NMAX].adfh.OBTusbn = 0;
   wpkt[i%NMAX].adfh.OBTusb1 = 0;
   wpkt[i%NMAX].adfh.OBTusb2 = i*8;
   wpkt[i%NMAX].adfh.mode = 2;
   wpkt[i%NMAX].adfh.PktStructID = 1;
   wpkt[i%NMAX].adfh.RedundID = 1;


   wpkt[i%NMAX].data=(unsigned short*)malloc(((length+1)/2)*sizeof(unsigned short));
   for(j=0;j<(length+1)/2;j++){
     switch(j){
       case 0:
         wpkt[i%NMAX].data[j]=wpkt[i%NMAX].adfh.syncmarkMSW;
       break;
       case 1:
         wpkt[i%NMAX].data[j]=wpkt[i%NMAX].adfh.syncmarkLSW;
       break;
       case 2:
         wpkt[i%NMAX].data[j]=(wpkt[i%NMAX].adfh.CRCflag<<14)+(wpkt[i%NMAX].adfh.type<<8)+wpkt[i%NMAX].adfh.subtype;
       break;
       case 3:
         wpkt[i%NMAX].data[j]=wpkt[i%NMAX].adfh.MMpageID;
       break;
       case 4:
         wpkt[i%NMAX].data[j]=wpkt[i%NMAX].adfh.MMpktcnt;
       break;
       case 5:
         wpkt[i%NMAX].data[j]=(wpkt[i%NMAX].adfh.century<<8)+wpkt[i%NMAX].adfh.APIDseqcnt;
       break;
       case 6:
         wpkt[i%NMAX].data[j]=(wpkt[i%NMAX].adfh.OBTsb0<<8+wpkt[i%NMAX].adfh.OBTsb1);
       break;
       case 7:
         wpkt[i%NMAX].data[j]=(wpkt[i%NMAX].adfh.OBTsb2<<8)+wpkt[i%NMAX].adfh.OBTsb3;
       break;
       case 8:
         wpkt[i%NMAX].data[j]=(wpkt[i%NMAX].adfh.OBTsb4<<8)+wpkt[i%NMAX].adfh.OBTusbn;
       break;
       case 9:
         wpkt[i%NMAX].data[j]=(wpkt[i%NMAX].adfh.OBTusb1<<8)+wpkt[i%NMAX].adfh.OBTusb2;
       break;
       case 10:
         wpkt[i%NMAX].data[j]=(wpkt[i%NMAX].adfh.mode<<8)+(wpkt[i%NMAX].adfh.PktStructID<<4)+wpkt[i%NMAX].adfh.RedundID;
       break;
       default:
        wpkt[i%NMAX].data[j] = 65535-j;
    }
   }
   if(fmod((length+1),2) != 0.0){
      wpkt[i%NMAX].lastbyte = 255;
   }
   word=(unsigned short *)malloc(MAXWORD*sizeof(unsigned short));
   if (debug) printf("%d+%d+%d+%d\n",wpkt[i%NMAX].ver<<13,wpkt[i%NMAX].type<<12,wpkt[i%NMAX].shf<<11,wpkt[i%NMAX].apid);
   word[0]=(wpkt[i%NMAX].ver<<13)+(wpkt[i%NMAX].type<<12)+(wpkt[i%NMAX].shf<<11)+wpkt[i%NMAX].apid;
   if (debug) printf("%d+%d\n",wpkt[i%NMAX].sf<<14,wpkt[i%NMAX].ssc);
   word[1]=(wpkt[i%NMAX].sf<<14)+wpkt[i%NMAX].ssc;
   if (debug) printf("%d\n",wpkt[i%NMAX].len);
   word[2]=wpkt[i%NMAX].len;
   for(j=3;j<(length+7)/2; j++){
     word[j]=wpkt[i%NMAX].data[j-3];
   }

   // Write pakets data to the file
  for(j=0;j<(length+7)/2;j++){
       swapword(&word[j]);
       fwrite(&word[j], 2, 1, wf);
  }
  if (fmod((length+7),2) != 0.0){
     mybyte=wpkt[i%NMAX].lastbyte;
      fwrite(&mybyte, 1, 1, wf);
 } 

   // Display packets details
	   if(debug) printf("Packets Details:\n");
	   if(debug) printf("packet n.%lld\n",i);
       if(debug) printf("version n.: %d\n", wpkt[i%NMAX].ver);
       if(debug) printf("pkt type: %d\n", wpkt[i%NMAX].type);
       if(debug) printf("sec. head. flag: %d\n", wpkt[i%NMAX].shf);
       if(debug) printf("apid: %d\n", wpkt[i%NMAX].apid);
       if(debug) printf("sequence flag: %d\n", wpkt[i%NMAX].sf);
       if(debug) printf("sequence counter: %d\n", wpkt[i%NMAX].ssc);
       if(debug) printf("pkt length: %d\n", wpkt[i%NMAX].len);
       if(debug) {
		   for(j=3;j<(length+7)/2;j++){
			printf("d%lld: %d\n",j, wpkt[i%NMAX].data[j-3]);
		  }
       }
       if (fmod((length+7),2) != 0.0){
		if(debug) printf("lb: %hhu\n", wpkt[i%NMAX].lastbyte);
       }
       if (debug) printf("\n");
      free(wpkt[i%NMAX].data);
      free(word);
}   

   // Close the file after writing
   fclose(wf);

   return 0;

}
