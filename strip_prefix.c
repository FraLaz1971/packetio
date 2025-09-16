#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  int i,cnt;
  unsigned char prefix[18];
  unsigned char packet[4096];
  unsigned char b;
  int prefix_length;
  int ccsds_length;
  unsigned int w1,w2,w;
  unsigned int len;
  int res;
  FILE *rf;
  FILE *wf;
  char *fname = argv[1];
  char *ofname = argv[3];
   // Open the binary file for reading
  rf = fopen(fname, "rb");
  wf = fopen(ofname, "wb");
   
   // Check if file open successfully
   if (!rf) { 
       fprintf(stderr,"Cannot open input file!\n");
       return 1;
   }
   if(!wf){
       fprintf(stderr,"Cannot open output file!\n");
       return 1;
   }

  prefix_length=atoi(argv[2]);
  cnt=0;
  if (rf > 0) {
    do {
      for(i=0;i<prefix_length;i++){
          res = fread(&b,1,1,rf);
          if(res){
          prefix[i]=b;
	  fprintf(stderr,"byte %d = %d\n",i,prefix[i]);
         }
     	}
          w1=(prefix[8]<<8)+prefix[9];
          w2=(prefix[10]<<8)+prefix[11];
          len=(w1<<16)+w2;
      if (res) {
        for(i=0;i<len;i++){
          res = fread(&b,1,1,rf);
          packet[i]=b;
	  fwrite(&packet[i], 1,1,wf);
	  fprintf(stderr,"byte %d = %d\n",i,packet[i]);
        }
	  fprintf(stderr, "len %i %8.8X\n", len, len);
      }
	cnt++;
    } while (res > 0);
  }
  fclose(rf);
  fclose(wf);
  fprintf(stderr,"processed %d packets\n",cnt-1);
  return 0;
}
