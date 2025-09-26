#include <stdio.h>
#include <stdlib.h>
int debug = 0;
int main(int argc, char **argv)
{
  int i,cnt;
  unsigned char prefix[256];
  unsigned char packet[65542];
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
  if(argc < 4){
    fprintf(stderr,"usage: %s <infile> <nbytes> <outfile>\n", argv[0]);
    return 1;
  }
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
	  if (debug) fprintf(stderr,"byte %d = %d\n",i,prefix[i]);
         }
     	}
      if (res) {
	len=56535;
        for(i=0;i<len+7;i++){
          res = fread(&b,1,1,rf);
          packet[i]=b;
	  if(i==5){
            len=(packet[4]<<8)+packet[5];
	  debug=1;
	  if (debug) fprintf(stderr,"len[%hu] = %hu\n",cnt,len);
	  debug=0;
	  }
	  fwrite(&packet[i], 1,1,wf);
	  if (debug) fprintf(stderr,"byte %d = %d\n",i,packet[i]);
        }
	  if (debug) fprintf(stderr, "len %i %8.8X\n", len, len);
      }
	cnt++;
    } while (res > 0);
  }
  fclose(rf);
  fclose(wf);
  fprintf(stderr,"processed %d packets\n",cnt);
  return 0;
}
