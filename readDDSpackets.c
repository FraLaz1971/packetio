#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "packets.h"
int main() {
	unsigned short int *word;
	unsigned short int hword[9];
    unsigned long long int j,k,npkt;
    unsigned int w1,w2;
    size_t res;
    char fname[1024];
    unsigned char mybyte;
    debug=0;
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
   word = (unsigned short int*)malloc(MAXWORD*sizeof(unsigned short int));
   
   struct DDSheader dhd[NMAX];
   struct Packet rpkt[NMAX];
   
   npkt=0;
while (npkt<ULONG_MAX){
   if(debug) printf("reading packet n.%lld\n",npkt);
   if(debug) printf("reading dds header \n");
     for(k=0; k<9; k++){
       res = fread(&hword[k], 2, 1, wf);
              if(!res) break;
       swapword(&hword[k]);
       switch(k){
		   case 0:
		     dhd[npkt%NMAX].sec_msw=hword[k];
             if(debug) printf("sec_msw: %hu\n",dhd[npkt%NMAX].sec_msw);
		   break;
		   case 1:
		     dhd[npkt%NMAX].sec_lsw=hword[k];
             if(debug) printf("sec_lsw: %hu\n",dhd[npkt%NMAX].sec_lsw);
		   break;
		   case 2:
		     dhd[npkt%NMAX].usec_msw=hword[k];
             if(debug) printf("usec_msw: %hu\n",dhd[npkt%NMAX].usec_msw);
		   break;
		   case 3:
		     dhd[npkt%NMAX].usec_lsw=hword[k];
             if(debug) printf("usec_lsw: %hu\n",dhd[npkt%NMAX].usec_lsw);
             dhd[npkt%NMAX].time=(double)((dhd[npkt%NMAX].sec_msw<<16)+dhd[npkt%NMAX].sec_lsw)+\
             ((dhd[npkt%NMAX].usec_msw<<16)+dhd[npkt%NMAX].usec_lsw)/1000000.0;
             if(debug) printf("time: %f\n",dhd[npkt%NMAX].time);
		   break;
		   case 4:
				w1=hword[k];
		   break;
		   case 5:
				w2=hword[k];
				dhd[npkt%NMAX].pkt_len=(w1<<16)+w2;
             if(debug) printf("pkt_len: %lu\n",dhd[npkt%NMAX].pkt_len);
			 rpkt[npkt%NMAX].data=(unsigned short*)malloc(((dhd[npkt%NMAX].pkt_len)/2)*sizeof(unsigned short));
		   break;
		   case 6:
				dhd[npkt%NMAX].gr_st_id=hword[k];
                if(debug) printf("gr_st_id: %hu\n",dhd[npkt%NMAX].gr_st_id);
		   break;
		   case 7:
				dhd[npkt%NMAX].vc_id=hword[k];
                if(debug) printf("vc_id: %hu\n",dhd[npkt%NMAX].vc_id);
		   break;
		   case 8:
				dhd[npkt%NMAX].sle_serv=(hword[k]&65280)>>8;
				dhd[npkt%NMAX].time_qual=hword[k]&255;
                if(debug) printf("sle_serv: %hu\n",dhd[npkt%NMAX].sle_serv);
                if(debug) printf("time_qual: %hu\n",dhd[npkt%NMAX].time_qual);
		   break;
		   default:
              printf("illegal value of k: %lld\n",k);
	   }
   }
   for (j = 0; j < 3; j++){
       res = fread(&word[j], 2, 1, wf);
              if(!res) break;
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
   if(debug) printf("max word: %hu\n",3+(rpkt[npkt%NMAX].len+1)/2);
   for (j=3; j<3+(rpkt[npkt%NMAX].len+1)/2;j++){
       res = fread(&word[j], 2, 1, wf);
       if(debug) printf("read word: %lld\n",j);
       if(!res)  break;
       if(debug) printf("j:%lld res:%lu\n",j,res);
       // decode the word
       swapword(&word[j]);
       rpkt[npkt%NMAX].data[(j-3)%MAXDATA]=word[j];
   }
   if(fmod((rpkt[npkt%NMAX].len+1),2) != 0.0)  {
       res = fread(&mybyte, 1, 1, wf);
       rpkt[npkt%NMAX].lastbyte=mybyte;
	   }
   if(!res) break;
       printf("%lld sec_msw: %hu\n",npkt, dhd[npkt%NMAX].sec_msw); // SCET seconds MSW
       printf("%lld sec_lsw: %hu\n",npkt, dhd[npkt%NMAX].sec_msw); // SCET seconds LSW
       printf("%lld usec_msw: %hu\n",npkt, dhd[npkt%NMAX].usec_msw); // SCET microseconds MSW 
       printf("%lld usec_lsw: %hu\n",npkt, dhd[npkt%NMAX].usec_lsw); // SCET microseconds LSW
       printf("%lld time: %f\n",npkt, dhd[npkt%NMAX].time); // SCET
       printf("%lld pkt_len: %lu\n",npkt, dhd[npkt%NMAX].pkt_len); // CCSDS packet length
       printf("%lld gr_st_id: %hu\n",npkt, dhd[npkt%NMAX].gr_st_id); // Ground Station ID
       printf("%lld vc_id: %hu\n",npkt, dhd[npkt%NMAX].vc_id); // Virtual Channel ID
       printf("%lld sle_serv: %hu\n",npkt, dhd[npkt%NMAX].sle_serv); // SLE Service
       printf("%lld time_qual: %hu\n",npkt, dhd[npkt%NMAX].time_qual); // Time Quality 0=good 1=inaccurate 2=bad
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
