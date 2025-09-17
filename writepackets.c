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
   word=(unsigned short *)malloc(MAXWORD*sizeof(unsigned short));
   // Initialize packet data
   struct Packet wpkt[NMAX];
 for(i=0;i<npkt;i++){
   wpkt[i%NMAX].ver = 0;
   wpkt[i%NMAX].type = 0;
   wpkt[i%NMAX].shf = 1;
   wpkt[i%NMAX].apid = 1032;
   wpkt[i%NMAX].sf = 2;
   wpkt[i%NMAX].ssc = 100+i;
   wpkt[i%NMAX].len = length;
   wpkt[i%NMAX].data=(unsigned short*)malloc(((length+1)/2)*sizeof(unsigned short));
   for(j=0;j<(length+1)/2;j++){
   wpkt[i%NMAX].data[j] = 65535-j;
  }
  if(fmod((length+1),2) != 0.0){
      wpkt[i%NMAX].lastbyte = 255;
 } 
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
}   
	free(word);

   // Close the file after writing
   fclose(wf);

   return 0;

}
