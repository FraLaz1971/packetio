#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "packets.h"
int main() {
    unsigned short int *word;
    unsigned long long int j,npkt;
    size_t res;
    char fname[1024];
    unsigned char mybyte;
    puts("insert the filename to read");
    scanf("%s",fname);
    printf("going to read file %s\n",fname);
   // Open the binary file for reading
   word=(unsigned short*)malloc(MAXWORD*sizeof(unsigned short));
   FILE *wf = fopen(fname, "rb");
   if(debug) printf("open file for reading\n");
   
   // Check if file open successfully
   if (!wf) { 
       printf("Cannot open file!\n");
       return 1;
   }

   // Read packet data from the file
   struct Packet rpkt[NMAX];
   npkt=0;
while (npkt<ULONG_MAX){
	if(debug) printf("reading packet n.%lld\n",npkt);
   for (j = 0; j < 3; j++){
       res = fread(&word[j], 2, 1, wf);
              if(!res) break;
//       if(debug) printf("j:%lld res:%lu\n",j,res);
       // decode the i word
       swapword(&word[j]);
	switch (j) {
       case 0:
          rpkt[npkt%NMAX].ver =  word[j]>>13;
          if(debug) printf("ver: %hu\n",rpkt[npkt%NMAX].ver);
          rpkt[npkt%NMAX].type = (word[j]>>12)&1;
          if(debug) printf("type: %hu\n",rpkt[npkt%NMAX].type);
          rpkt[npkt%NMAX].shf =  (word[j]>>11)&1;
          if(debug) printf("shf: %hu\n",rpkt[npkt%NMAX].shf);          
          rpkt[npkt%NMAX].apid = word[j]&2047;
          if(debug) printf("apid: %hu\n",rpkt[npkt%NMAX].apid);
          break;
       case 1:
              rpkt[npkt%NMAX].sf =  word[j]>>14;
			  if(debug) printf("sf: %hu\n",rpkt[npkt%NMAX].sf);
              rpkt[npkt%NMAX].ssc = word[j]&16383;
			  if(debug) printf("ssc: %hu\n",rpkt[npkt%NMAX].ssc);
                  break;
       case 2:
                 rpkt[npkt%NMAX].len = word[j];
		if (debug) printf("length:%hu\n",rpkt[npkt%NMAX].len);
		break;
       default:
              printf("illegal value of j: %lld\n",j);
        }
   }
   rpkt[npkt%NMAX].data=(unsigned short*)malloc(MAXDATA*sizeof(unsigned short));
   if(debug) printf("max word: %hu\n",3+(rpkt[npkt%NMAX].len+1)/2);
   for (j=3; j<3+(rpkt[npkt%NMAX].len+1)/2;j++){
       res = fread(&word[j], 2, 1, wf);
       if(debug) printf("read word: %lld\n",j);
       if(!res)  break;
       if(debug) printf("j:%lld res:%lu\n",j,res);
       // decode the i word
       swapword(&word[j]);
       rpkt[npkt%NMAX].data[(j-3)%MAXDATA]=word[j];
   }
   if(fmod((rpkt[npkt%NMAX].len+1),2) != 0.0)  {
       res = fread(&mybyte, 1, 1, wf);
       rpkt[npkt%NMAX].lastbyte=mybyte;
	   }
   if(!res) break;
       printf("%lld version: %hu\n",npkt, rpkt[npkt%NMAX].ver); // packet version
       printf("%lld type: %hu\n",npkt, rpkt[npkt%NMAX].type); // packet type 0,1
       printf("%lld shf:  %hu\n",npkt, rpkt[npkt%NMAX].shf);  //secondary header flag
       printf("%lld apid: %hu\n",npkt, rpkt[npkt%NMAX].apid); // application ID
 // end 1st 16 bits word
       printf("%lld sf: %hu\n",npkt, rpkt[npkt%NMAX].sf); // sequence flag
       printf("%lld ssc: %hu\n",npkt, rpkt[npkt%NMAX].ssc); // source sequence counter
 // end 2nd 16 bits word
       printf("%lld len: %hu\n",npkt, rpkt[npkt%NMAX].len); // packet length (bytes-1 more)
 // end 3rd 16 bits word
   for (j=3; j<3+(rpkt[npkt%NMAX].len+1)/2;j++){
           printf("%lld d%lld: %hu\n",npkt, j-3,rpkt[npkt%NMAX].data[(j-3)%MAXDATA]); // j data word
      }
 // end all the 16 bit words
   if(fmod((rpkt[npkt%NMAX].len+1),2) != 0){
	   printf("%lld lastbyte: %hu\n",npkt,rpkt[npkt%NMAX].lastbyte); // lastbyte
 // end last byte if odd number of bytes
	   }
       printf("\n");
   free(rpkt[npkt%NMAX].data);
   npkt++;
}
   // Close the file after reading
   fclose(wf);
   free(word);
	printf("read %lld packets\n",npkt);
   return 0;
}
