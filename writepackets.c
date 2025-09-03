#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "packets.h"
int main() {
   unsigned short int word[1027];
   int i,j,npkt;
   char fname[32];
   // decide the number of packets
   puts("enter n. of packets");
   scanf("%d",&npkt);
   // decide the file name
   puts("enter file name");
   scanf("%s",fname);
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
   wpkt[i].len = 7;
   wpkt[i].data[0] = 65535;
   wpkt[i].data[1] = 65535-i-1;
   wpkt[i].data[2] = 65535-i-2;
   wpkt[i].data[3] = 65535-i-3;
  }

// 57344
// 4096
// 2048
// 2047
//
// 49152
// 16383

   for (int i = 0; i < npkt; i++){
   printf("%d+%d+%d+%d\n",wpkt[i].ver<<13,wpkt[i].type<<12,wpkt[i].shf<<11,wpkt[i].apid);
   word[0]=(wpkt[i].ver<<13)+(wpkt[i].type<<12)+(wpkt[i].shf<<11)+wpkt[i].apid;
   printf("%d+%d\n",wpkt[i].sf<<14,wpkt[i].ssc);
   word[1]=(wpkt[i].sf<<14)+wpkt[i].ssc;
   printf("%d\n",wpkt[i].len);
   word[2]=wpkt[i].len;
   word[3]=wpkt[i].data[0];
   word[4]=wpkt[i].data[1];
   word[5]=wpkt[i].data[2];
   word[6]=wpkt[i].data[3];

   // Write pakets data to the file
  for(j=0;j<7;j++){
       swapword(&word[j]);
       fwrite(&word[j], 2, 1, wf);
  }
}
   // Close the file after writing
   fclose(wf);


   // Display packets details
   printf("Packets Details:\n");
   for (i = 0; i < npkt; i++) {
	   printf("packet n.%d\n",i);
       printf("version n.: %d\n", wpkt[i].ver);
       printf("pkt type: %d\n", wpkt[i].type);
       printf("sec. head. flag: %d\n", wpkt[i].shf);
       printf("apid: %d\n", wpkt[i].apid);
       printf("sequence flag: %d\n", wpkt[i].sf);
       printf("sequence counter: %d\n", wpkt[i].ssc);
       printf("pkt length: %d\n", wpkt[i].len);
       printf("\n");
   }

   return 0;
}
