#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "packets.h"
int main() {
   unsigned short int word[32763];
   unsigned long long int i,j,npkt;
   unsigned short int length;// length in bytes-1
   char fname[32];
   unsigned char mybyte;
   // decide the number of packets
   do{
      puts("enter n. of packets");
      scanf("%lld",&npkt);
    } while ((npkt>4000)||(npkt<1));
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
   wpkt[i].ver = 0;
   wpkt[i].type = 0;
   wpkt[i].shf = 1;
   wpkt[i].apid = 1032;
   wpkt[i].sf = 2;
   wpkt[i].ssc = 100+i;
   wpkt[i].len = length;
   for(j=0;j<(length+1)/2;j++){
   wpkt[i].data[j] = 65535-j;
  }
  if(fmod((length+1),2) != 0.0){
      wpkt[i].lastbyte = 255;
 }
}
// 57344
// 4096
// 2048
// 2047
//
// 49152
// 16383

   for (i = 0; i < npkt; i++){
   printf("%d+%d+%d+%d\n",wpkt[i].ver<<13,wpkt[i].type<<12,wpkt[i].shf<<11,wpkt[i].apid);
   word[0]=(wpkt[i].ver<<13)+(wpkt[i].type<<12)+(wpkt[i].shf<<11)+wpkt[i].apid;
   printf("%d+%d\n",wpkt[i].sf<<14,wpkt[i].ssc);
   word[1]=(wpkt[i].sf<<14)+wpkt[i].ssc;
   printf("%d\n",wpkt[i].len);
   word[2]=wpkt[i].len;
   for(j=3;j<(length+7)/2; j++){
     word[j]=wpkt[i].data[j-3];
   }

   // Write pakets data to the file
  for(j=0;j<(length+7)/2;j++){
       swapword(&word[j]);
       fwrite(&word[j], 2, 1, wf);
  }
  if (fmod((length+7),2) != 0.0){
     mybyte=wpkt[i].lastbyte;
//	 swapword(&word[j]);
//     fwrite(&word[j], 1, 1, wf);
      fwrite(&mybyte, 1, 1, wf);
 } 
}
   // Close the file after writing
   fclose(wf);


   // Display packets details
   printf("Packets Details:\n");
   for (i = 0; i < npkt; i++) {
	   printf("packet n.%lld\n",i);
       printf("version n.: %d\n", wpkt[i].ver);
       printf("pkt type: %d\n", wpkt[i].type);
       printf("sec. head. flag: %d\n", wpkt[i].shf);
       printf("apid: %d\n", wpkt[i].apid);
       printf("sequence flag: %d\n", wpkt[i].sf);
       printf("sequence counter: %d\n", wpkt[i].ssc);
       printf("pkt length: %d\n", wpkt[i].len);
       for(j=3;j<(length+7)/2;j++){
		printf("d%lld: %d\n",j, wpkt[i].data[j-3]);
       }
       if (fmod((length+7),2) != 0.0){
		printf("d%lld: %d\n",j, wpkt[i].lastbyte);
       }       
       printf("\n");
   }

   return 0;
}
