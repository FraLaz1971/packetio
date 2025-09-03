#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "packets.h"
int main() {
	unsigned short int word[1027];
    int i,j,npkt;
    int debug=0;
    size_t res;
    char fname[32];
    puts("insert the filename to read");
    scanf("%s",fname);
    printf("going to read file %s\n",fname);
   // Open the binary file for reading
   FILE *wf = fopen(fname, "rb");
   
   // Check if file open successfully
   if (!wf) { 
       printf("Cannot open file!\n");
       return 1;
   }

   // Read packet data from the file
   struct Packet rpkt[NMAX];
   npkt=0;
while (npkt<NMAX){
	if(debug) printf("reading packet n.%d\n",npkt);
   for (j = 0; j < 7; j++){
       res = fread(&word[j], 2, 1, wf);
       if(!res) break;
       if(debug) printf("j:%d res:%lu\n",j,res);
       // decode the i word
       swapword(&word[j]);
       switch (j) {
       case 0:
          rpkt[npkt].ver =  word[j]>>13;
          rpkt[npkt].type = (word[j]>>12)&1;
          rpkt[npkt].shf =  (word[j]>>11)&1;
          rpkt[npkt].apid = word[j]&2047;
		  break;
	   case 1:
	      rpkt[npkt].sf =  word[j]>>14;
          rpkt[npkt].ssc = word[j]&16383;
		  break;
	   case 2:
		  rpkt[npkt].len = word[j];
			break;
	   case 3:
			rpkt[npkt].data[0]=word[j];
			break;
	   case 4:
			rpkt[npkt].data[1]=word[j];
			break;
	   case 5:
			rpkt[npkt].data[2]=word[j];
			break;
	   case 6:
			rpkt[npkt].data[3]=word[j];
			break;
	  default:
			printf("illegal value of j\n");
	}
   }
   if(!res) break;
   npkt++;
}
   // Close the file after reading
   fclose(wf);

   // Display student details
   printf("Packets Details:\n");
   for (i = 0; i < npkt; i++) {
       printf("%d version: %d\n",i, rpkt[i].ver); // packet version
       printf("%d type: %d\n",i, rpkt[i].type); // packet type 0,1
       printf("%d shf:  %d\n",i, rpkt[i].shf);  //secondary header flag
       printf("%d apid: %d\n",i, rpkt[i].apid); // application ID
 // end 1st 16 bits word
       printf("%d sf: %d\n",i, rpkt[i].sf); // sequence flag
       printf("%d ssc: %d\n",i, rpkt[i].ssc); // source sequence counter
 // end 2nd 16 bits word
       printf("%d len: %d\n",i, rpkt[i].len); // packet length (bytes-1 more)
 // end 3rd 16 bits word
       printf("%d d1: %d\n",i, rpkt[i].data[0]); // first data word
       printf("%d d2: %d\n",i, rpkt[i].data[1]); // second data word
       printf("%d d3: %d\n",i, rpkt[i].data[2]); // third data word
       printf("%d d4: %d\n",i, rpkt[i].data[3]); // fourth data word
       printf("\n");
   }
	printf("read %d packets\n",npkt);
   return 0;
}
