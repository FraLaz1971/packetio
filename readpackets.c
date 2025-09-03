#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "packets.h"
int main() {
	unsigned short int word[32763];
    unsigned long long int i,j,npkt;
    int debug=0;
    size_t res;
    char fname[32];
    unsigned char mybyte;
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
	if(debug) printf("reading packet n.%lld\n",npkt);
   for (j = 0; j < 3; j++){
       res = fread(&word[j], 2, 1, wf);
              if(!res) break;
//       if(debug) printf("j:%lld res:%lu\n",j,res);
       // decode the i word
       swapword(&word[j]);
	switch (j) {
       case 0:
          rpkt[npkt].ver =  word[j]>>13;
          if(debug) printf("ver: %hu\n",rpkt[npkt].ver);
          rpkt[npkt].type = (word[j]>>12)&1;
          if(debug) printf("type: %hu\n",rpkt[npkt].type);
          rpkt[npkt].shf =  (word[j]>>11)&1;
          if(debug) printf("shf: %hu\n",rpkt[npkt].shf);          
          rpkt[npkt].apid = word[j]&2047;
          if(debug) printf("apid: %hu\n",rpkt[npkt].apid);
          break;
       case 1:
              rpkt[npkt].sf =  word[j]>>14;
			  if(debug) printf("sf: %hu\n",rpkt[npkt].sf);
              rpkt[npkt].ssc = word[j]&16383;
			  if(debug) printf("ssc: %hu\n",rpkt[npkt].ssc);
                  break;
       case 2:
                 rpkt[npkt].len = word[j];
		if (debug) printf("length:%hu\n",rpkt[npkt].len);
		break;
       default:
              printf("illegal value of j: %lld\n",j);
        }
   }
   if(debug) printf("max word: %hu\n",3+(rpkt[npkt].len+1)/2);
   for (j=3; j<3+(rpkt[npkt].len+1)/2;j++){
       res = fread(&word[j], 2, 1, wf);
       if(debug) printf("read word: %lld\n",j);
       if(!res)  break;
//       if(debug) printf("j:%lld res:%lu\n",j,res);
       // decode the i word
       swapword(&word[j]);
       rpkt[npkt].data[j-3]=word[j];
   }
   if(fmod((rpkt[npkt].len+1),2) != 0.0)  {
       res = fread(&mybyte, 1, 1, wf);
//       printf("read word: %d",word[j]);
//       swapword(&word[j]);
//       printf("after swap word: %d",word[j]);
       rpkt[npkt].lastbyte=mybyte;
	   }
   if(!res) break;
   npkt++;
}
   // Close the file after reading
   fclose(wf);

   // Display student details
   printf("Packets Details:\n");
   for (i = 0; i < npkt; i++) {
       printf("%lld version: %hu\n",i, rpkt[i].ver); // packet version
       printf("%lld type: %hu\n",i, rpkt[i].type); // packet type 0,1
       printf("%lld shf:  %hu\n",i, rpkt[i].shf);  //secondary header flag
       printf("%lld apid: %hu\n",i, rpkt[i].apid); // application ID
 // end 1st 16 bits word
       printf("%lld sf: %hu\n",i, rpkt[i].sf); // sequence flag
       printf("%lld ssc: %hu\n",i, rpkt[i].ssc); // source sequence counter
 // end 2nd 16 bits word
       printf("%lld len: %hu\n",i, rpkt[i].len); // packet length (bytes-1 more)
 // end 3rd 16 bits word
   for (j=3; j<3+(rpkt[i].len+1)/2;j++){
           printf("%lld d%lld: %hu\n",i, j-3,rpkt[i].data[j-3]); // j data word
      }
 // end all the 16 bit words
   if(fmod((rpkt[i].len+1),2) != 0){
	   printf("%lld lastbyte: %hu\n",i,rpkt[i].lastbyte); // lastbyte
 // end last byte if odd number of bytes
	   }
       printf("\n");
}
	printf("read %lld packets\n",npkt);
   return 0;
}
