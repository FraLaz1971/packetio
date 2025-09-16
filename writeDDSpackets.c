#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "packets.h"
int main() {
   unsigned short int word[MAXWORD];
   unsigned short int hword[9];
   unsigned long long int i,j,k,npkt;
   unsigned short int length;// length in bytes-1
   char fname[1024];
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
   
   struct DDSheader dhd[NMAX];
   struct Packet wpkt[NMAX];
 for(i=0;i<npkt;i++){ /* start loop on the packets to write*/
   /* fill the dds header */  
   dhd[i%NMAX].sec_msw = 0;
   dhd[i%NMAX].sec_lsw = i+1;
   dhd[i%NMAX].usec_msw = 0;
   dhd[i%NMAX].usec_lsw = (i+1)*1000;
   dhd[i%NMAX].pkt_len=(unsigned int)length+7; // 32 bit n. of bytes in the data packet excluding the dds header 
   dhd[i%NMAX].gr_st_id=1; //16 bit ground station ID
   dhd[i%NMAX].vc_id=1; //16 bit virtual channel ID
   dhd[i%NMAX].sle_serv=8; //8 bit sle service channel and data type 
   dhd[i%NMAX].time_qual=0; //8 bit: 0 = good, 1 = inaccurate, 2 = bad
   dhd[i%NMAX].time=(double)((dhd[i%NMAX].sec_msw<<16)+dhd[i%NMAX].sec_lsw)+\
   ((dhd[i%NMAX].usec_msw<<16)+dhd[i%NMAX].usec_lsw)/1000000.0;
   if (debug) printf("%hu %hu %hu %hu\n",dhd[i%NMAX].sec_msw,dhd[i%NMAX].sec_lsw,dhd[i%NMAX].usec_msw,dhd[i%NMAX].usec_lsw);
   hword[0]=dhd[i%NMAX].sec_msw;hword[1]=dhd[i%NMAX].sec_lsw; hword[2]=dhd[i%NMAX].usec_msw; hword[3]=dhd[i%NMAX].usec_lsw;
   hword[4]=(dhd[i%NMAX].pkt_len&4294901760)>>16; hword[5]=dhd[i%NMAX].pkt_len&65535;
   if (debug) printf("%hu+%hu\n", hword[4],hword[5]);
   if (debug) printf("%hu %hu %hhu %hhu\n", dhd[i%NMAX].gr_st_id, dhd[i%NMAX].vc_id, dhd[i%NMAX].sle_serv, dhd[i%NMAX].time_qual);
   hword[6]=dhd[i%NMAX].gr_st_id; hword[7]=dhd[i%NMAX].vc_id;hword[8]=(dhd[i%NMAX].sle_serv<<8)+dhd[i%NMAX].time_qual; 
  // write dds header data to the file
  for(k=0; k<9; k++){
       swapword(&hword[k]);
       fwrite(&hword[k], 2, 1, wf);
  } 
   /* fill the ccsds packet */ 
   wpkt[i%NMAX].ver = 0;
   wpkt[i%NMAX].type = 0;
   wpkt[i%NMAX].shf = 1;
   wpkt[i%NMAX].apid = 1032;
   wpkt[i%NMAX].sf = 2;
   wpkt[i%NMAX].ssc = 100+i;
   wpkt[i%NMAX].len = length;
   if (debug) printf("%d+%d+%d+%d\n",wpkt[i%NMAX].ver<<13,wpkt[i%NMAX].type<<12,wpkt[i%NMAX].shf<<11,wpkt[i%NMAX].apid);
   word[0]=(wpkt[i%NMAX].ver<<13)+(wpkt[i%NMAX].type<<12)+(wpkt[i%NMAX].shf<<11)+wpkt[i%NMAX].apid;
   if (debug) printf("%d+%d\n",wpkt[i%NMAX].sf<<14,wpkt[i%NMAX].ssc);
   word[1]=(wpkt[i%NMAX].sf<<14)+wpkt[i%NMAX].ssc;
   if (debug) printf("%d\n",wpkt[i%NMAX].len);
   word[2]=wpkt[i%NMAX].len;
   wpkt[i%NMAX].data=(unsigned short*)malloc(((length+1)/2)*sizeof(unsigned short));
   for(j=3;j<(dhd[i%NMAX].pkt_len)/2; j++){
     wpkt[i%NMAX].data[j-3] = 65535-j+3;
     word[j]=wpkt[i%NMAX].data[j-3];
  }
   // Write ccsds packets data to the file
  for(j=0;j<dhd[i%NMAX].pkt_len/2;j++){
       swapword(&word[j]);
       fwrite(&word[j], 2, 1, wf);
  }
  if(fmod((length+1),2) != 0.0){
      wpkt[i%NMAX].lastbyte = 255;
 }  

  if (fmod((double)dhd[i%NMAX].pkt_len,2.0) != 0.0){
     mybyte=wpkt[i%NMAX].lastbyte;
      fwrite(&mybyte, 1, 1, wf);
 } 
	   if(debug) printf("Packets Details:\n");
	   if(debug) printf("packet n.%lld\n",i%NMAX);
	   if(debug) printf("DDS header\n");
	   if(debug) printf("scet_sec_msw: %d\n", dhd[i%NMAX].sec_msw);
	   if(debug) printf("scet_sec_lsw: %d\n", dhd[i%NMAX].sec_lsw);
	   if(debug) printf("scet_usec_msw: %d\n", dhd[i%NMAX].usec_msw);
	   if(debug) printf("scet_usec_lsw: %d\n", dhd[i%NMAX].usec_lsw);
	   if(debug) printf("scet: %f\n", dhd[i%NMAX].time);
	   if(debug) printf("packet length: %ld\n", dhd[i%NMAX].pkt_len);
	   if(debug) printf("ground station id: %d\n", dhd[i%NMAX].gr_st_id);
	   if(debug) printf("virtual channel id: %d\n", dhd[i%NMAX].vc_id);
	   if(debug) printf("SLE service: %d\n", dhd[i%NMAX].sle_serv);
	   if(debug) printf("Time Quality: %d\n", dhd[i%NMAX].time_qual);
	   if(debug) printf("CCSDS packet\n");
       if(debug) printf("version n.: %d\n", wpkt[i%NMAX].ver);
       if(debug) printf("pkt type: %d\n", wpkt[i%NMAX].type);
       if(debug) printf("sec. head. flag: %d\n", wpkt[i%NMAX].shf);
       if(debug) printf("apid: %d\n", wpkt[i%NMAX].apid);
       if(debug) printf("sequence flag: %d\n", wpkt[i%NMAX].sf);
       if(debug) printf("sequence counter: %d\n", wpkt[i%NMAX].ssc);
       if(debug) printf("pkt length: %d\n", wpkt[i%NMAX].len);
       if(debug) {
		   for(j=3;j<(dhd[i%NMAX].pkt_len)/2;j++){
			printf("d%llu: %d\n",j, wpkt[i%NMAX].data[j-3]);
		  }
       }
       if (fmod((double)dhd[i%NMAX].pkt_len,2.0) != 0.0){
		if(debug) printf("lb: %hhu\n", wpkt[i%NMAX].lastbyte);
       }
       if (debug) printf("\n");
      free(wpkt[i%NMAX].data);
} /* end loop on the packets*/

   // Close the file after writing
   fclose(wf);

   // Display packets details
  return 0;

}

