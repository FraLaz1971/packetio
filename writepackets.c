#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "packets.h"
int main() {
   unsigned short int word[1027];
   int i,j,npkt;
   // decide the number of packets
   puts("enter n. of packets");
   scanf("%d",&npkt);
   // Open the binary file for writing
   FILE *wf = fopen("packets.ccsds", "wb");

   // Check if file open successfully
   if (!wf) { 
       printf("Cannot open file!\n");
       return 1;
   }

   // Initialize packet data
   struct Packet wpkt[NMAX];
   /*
   wpkt[0].ver = 3;
   wpkt[1].ver = 3;
   wpkt[2].ver = 3;
   wpkt[0].type = 1;
   wpkt[1].type = 1;
   wpkt[2].type = 1;
   wpkt[0].shf = 1;
   wpkt[1].shf = 1;
   wpkt[2].shf = 1;
   wpkt[0].apid = 1032;
   wpkt[1].apid = 1032;
   wpkt[2].apid = 1032;
   wpkt[0].sf = 0;
   wpkt[1].sf = 0;
   wpkt[2].sf = 0;
   wpkt[0].ssc = 100;
   wpkt[1].ssc = 101;
   wpkt[2].ssc = 102;
   wpkt[0].len = 5;
   wpkt[1].len = 5;
   wpkt[2].len = 5;
   wpkt[0].data[0] = 65535;
   wpkt[0].data[1] = 65535;
   wpkt[0].data[2] = 65535;
   wpkt[1].data[0] = 65535;
   wpkt[1].data[1] = 65535;
   wpkt[1].data[2] = 65535;
   wpkt[2].data[0] = 65535;
   wpkt[2].data[1] = 65535;
   wpkt[2].data[2] = 65535;
  */
 for(i=0;i<npkt;i++){
   wpkt[i].ver = 3;
   wpkt[i].type = 1;
   wpkt[i].shf = 1;
   wpkt[i].apid = 1032;
   wpkt[i].sf = 0;
   wpkt[i].ssc = 100+i;
   wpkt[i].len = 5;
   wpkt[i].data[0] = 65535-i;
   wpkt[i].data[1] = 65535-i-1;
   wpkt[i].data[2] = 65535-i-2;
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

   // Write pakets data to the file
  for(j=0;j<6;j++){
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
