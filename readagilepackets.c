#include "packets.h"
int main() {
    struct Packet rpkt[NMAX];
    unsigned short int *word;
    unsigned long long int j,npkt;
    size_t res;
    char sword[5];
    char fname[1024];
    FILE *wf;
    unsigned char mybyte;
    puts("insert the filename to read");
    scanf("%s",fname);
    printf("going to read file %s\n",fname);
   // Open the binary file for reading
   word=(unsigned short*)malloc(MAXWORD*sizeof(unsigned short));
   wf = fopen(fname, "rb");
   if(debug) printf("open file for reading\n");
   
   // Check if file open successfully
   if (!wf) { 
       printf("Cannot open file!\n");
       return 1;
   }

   // Read packet data from the file
   npkt=0;
while (npkt<ULONG_MAX){
	if(debug) printf("reading packet n.%lld\n",npkt);
   for (j = 0; j < 14; j++){
       res = fread(&word[j], 2, 1, wf);
              if(!res) break;
//       if(debug) printf("j:%lld res:%lu\n",j,res);
       // decode the word
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
          rpkt[npkt%NMAX].pid = (word[j]&2032)>>4;
          if(debug) printf("pid: %hu\n",rpkt[npkt%NMAX].pid);
          rpkt[npkt%NMAX].cat = word[j]&15;
          if(debug) printf("cat: %hu\n",rpkt[npkt%NMAX].cat);
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
       case 3:
           rpkt[npkt%NMAX].adfh.syncmarkMSW = word[j];
           if (debug) printf("syncmarkMSW:%hu\n",rpkt[npkt%NMAX].adfh.syncmarkMSW);
       break;
       case 4:
           rpkt[npkt%NMAX].adfh.syncmarkLSW = word[j];
           if (debug) printf("syncmarkLSW:%hu\n",rpkt[npkt%NMAX].adfh.syncmarkLSW);
       break;
       case 5:
           rpkt[npkt%NMAX].adfh.CRCflag = word[j]>>14;
           rpkt[npkt%NMAX].adfh.type = (word[j]&16128)>>8;
           rpkt[npkt%NMAX].adfh.subtype = word[j]&255;
           if (debug) printf("CRCflag:%u\n",rpkt[npkt%NMAX].adfh.CRCflag);
           if (debug) printf("dfh.type:%u\n",rpkt[npkt%NMAX].adfh.type);
           if (debug) printf("dfh.subtype:%u\n",rpkt[npkt%NMAX].adfh.subtype);
       break;
       case 6:
           rpkt[npkt%NMAX].adfh.MMpageID = word[j];
           if (debug) printf("MMpageID:%hu\n",rpkt[npkt%NMAX].adfh.MMpageID);
       break;
       case 7:
           rpkt[npkt%NMAX].adfh.MMpktcnt = word[j];
           if (debug) printf("MMpktcnt:%u\n",rpkt[npkt%NMAX].adfh.MMpktcnt);
       break;
       case 8:
           rpkt[npkt%NMAX].adfh.century = word[j]>>8;
           if (debug) printf("century:%u\n",rpkt[npkt%NMAX].adfh.century);
           rpkt[npkt%NMAX].adfh.APIDseqcnt = word[j]&255;
           if (debug) printf("APIDseqcnt:%u\n",rpkt[npkt%NMAX].adfh.APIDseqcnt);
       break;
       case 9:
           rpkt[npkt%NMAX].adfh.OBTsb0 = word[j]>>8;
           rpkt[npkt%NMAX].adfh.OBTsb1 = word[j]&255;
           if (debug) printf("OBTsb0:%llu\n",rpkt[npkt%NMAX].adfh.OBTsb0);
           if (debug) printf("OBTsb1:%lu\n",rpkt[npkt%NMAX].adfh.OBTsb1);
       break;
       case 10:
           rpkt[npkt%NMAX].adfh.OBTsb2 = word[j]>>8;
           rpkt[npkt%NMAX].adfh.OBTsb3 = word[j]&255;
           if (debug) printf("OBTsb2:%hu\n",rpkt[npkt%NMAX].adfh.OBTsb2);
           if (debug) printf("OBTsb3:%hu\n",rpkt[npkt%NMAX].adfh.OBTsb3);
       break;
       case 11:
           rpkt[npkt%NMAX].adfh.OBTsb4=word[j]>>8;
           rpkt[npkt%NMAX].adfh.OBTsecs=(rpkt[npkt%NMAX].adfh.OBTsb0<<32)+ \
           (rpkt[npkt%NMAX].adfh.OBTsb1<<24)+(rpkt[npkt%NMAX].adfh.OBTsb2<<16)+ \
           (rpkt[npkt%NMAX].adfh.OBTsb3<<8)+(rpkt[npkt%NMAX].adfh.OBTsb4);
           if (debug) printf("OBTsb4:%u\n",rpkt[npkt%NMAX].adfh.OBTsb4);
           if (debug) printf("OBTsecs:%llu\n",rpkt[npkt%NMAX].adfh.OBTsecs);
           rpkt[npkt%NMAX].adfh.OBTusbn=word[j]&15;
           if (debug) printf("OBTusbn:%u\n",rpkt[npkt%NMAX].adfh.OBTusbn);
       break;
       case 12:
           rpkt[npkt%NMAX].adfh.OBTusb1 = word[j]>>8;
           rpkt[npkt%NMAX].adfh.OBTusb2 = word[j]&255;
           rpkt[npkt%NMAX].adfh.OBTusecs = (rpkt[npkt%NMAX].adfh.OBTusbn<<16)+ \
           (rpkt[npkt%NMAX].adfh.OBTusb1<<8)+rpkt[npkt%NMAX].adfh.OBTusb2;
           rpkt[npkt%NMAX].adfh.OBTtime=(double)rpkt[npkt%NMAX].adfh.OBTsecs+ \
           (double)(rpkt[npkt%NMAX].adfh.OBTusecs)/1000000.0;
           if (debug) printf("OBTusb1:%hu\n",rpkt[npkt%NMAX].adfh.OBTusb1);
           if (debug) printf("OBTusb2:%hu\n",rpkt[npkt%NMAX].adfh.OBTusb2);
           if (debug) printf("OBTusecs:%u\n",rpkt[npkt%NMAX].adfh.OBTusecs);
           if (debug) printf("OBTTime:%lf\n",rpkt[npkt%NMAX].adfh.OBTtime);
       break;
       case 13:
           rpkt[npkt%NMAX].CRC = word[j];
           if (debug) printf("CRC:%hu\n",rpkt[npkt%NMAX].CRC);
       break;
       default:
              printf("illegal value of j: %lld\n",j);
        }
   }
   rpkt[npkt%NMAX].data=(unsigned short*)malloc(MAXDATA*sizeof(unsigned short));
   if(debug) printf("max word: %hu\n",14+(rpkt[npkt%NMAX].len+1)/2);
   for (j=14; j<3+(rpkt[npkt%NMAX].len+1)/2;j++){
       res = fread(&word[j], 2, 1, wf);
       if(debug) printf("read word: %lld\n",j);
       if(!res)  break;
       if(debug) printf("j:%lld res:%lu\n",j,res);
       // decode the i word
       swapword(&word[j]);
       rpkt[npkt%NMAX].data[(j-14)%MAXDATA]=word[j];
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
       printf("%lld pid: %hu\n",npkt, rpkt[npkt%NMAX].pid); // application ID
       printf("%lld cat: %hu\n",npkt, rpkt[npkt%NMAX].cat); // application ID
 // end 1st 16 bits word
       printf("%lld sf: %hu\n",npkt, rpkt[npkt%NMAX].sf); // sequence flag
       printf("%lld ssc: %hu\n",npkt, rpkt[npkt%NMAX].ssc); // source sequence counter
 // end 2nd 16 bits word
       printf("%lld len: %hu\n",npkt, rpkt[npkt%NMAX].len); // packet length (bytes-1 more)
 // end 3rd 16 bits word
       printf("%lld syncmarkMSW: %hu\n",npkt,rpkt[npkt%NMAX].adfh.syncmarkMSW);
       printf("%lld syncmarkLSW: %hu\n",npkt,rpkt[npkt%NMAX].adfh.syncmarkLSW);
       printf("%lld CRCflag: %u\n",npkt,rpkt[npkt%NMAX].adfh.CRCflag);
       printf("%lld dfh.type: %u\n",npkt,rpkt[npkt%NMAX].adfh.type);
       printf("%lld dfh.subtype: %u\n",npkt,rpkt[npkt%NMAX].adfh.subtype);
       printf("%lld MMpageID: %hu\n",npkt,rpkt[npkt%NMAX].adfh.MMpageID);
       printf("%lld MMpktcnt: %u\n",npkt,rpkt[npkt%NMAX].adfh.MMpktcnt);
       printf("%lld century: %u\n",npkt,rpkt[npkt%NMAX].adfh.century);
       printf("%lld APIDseqcnt: %u\n",npkt,rpkt[npkt%NMAX].adfh.APIDseqcnt);
       printf("%lld OBTsb0: %llu\n",npkt,rpkt[npkt%NMAX].adfh.OBTsb0);
       printf("%lld OBTsb1: %lu\n",npkt,rpkt[npkt%NMAX].adfh.OBTsb1);
       printf("%lld OBTsb2: %hu\n",npkt,rpkt[npkt%NMAX].adfh.OBTsb2);
       printf("%lld OBTsb3: %hu\n",npkt,rpkt[npkt%NMAX].adfh.OBTsb3);
       printf("%lld OBTsb4: %u\n",npkt,rpkt[npkt%NMAX].adfh.OBTsb4);
       printf("%lld OBTsecs: %llu\n",npkt,rpkt[npkt%NMAX].adfh.OBTsecs);
       printf("%lld OBTusbn: %u\n",npkt,rpkt[npkt%NMAX].adfh.OBTusbn);
       printf("%lld OBTusb1: %hu\n",npkt,rpkt[npkt%NMAX].adfh.OBTusb1);
       printf("%lld OBTusb2: %hu\n",npkt,rpkt[npkt%NMAX].adfh.OBTusb2);
       printf("%lld OBTusecs: %u\n",npkt,rpkt[npkt%NMAX].adfh.OBTusecs);
       printf("%lld OBTTime: %lf\n",npkt,rpkt[npkt%NMAX].adfh.OBTtime);
   for (j=14; j<3+(rpkt[npkt%NMAX].len+1)/2;j++){
           snprintf(sword,5,"%4X",rpkt[npkt%NMAX].data[(j-14)%MAXDATA]); // j data word
           leftpad(sword,'0');
           printf("%lld d%lld: %s\n",npkt, j-14,sword);
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
