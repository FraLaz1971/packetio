#include <stdio.h>
#include <stdlib.h>

main(int argc, char **argv)
{
  int i,cnt;
  unsigned char prefix[18];
  unsigned char packet[4096];
  int prefix_length;
  int ccsds_length;
  unsigned short int len;
  int res;
  FILE *rf;
  char *fname = argv[1];
   // Open the binary file for reading
  rf = fopen(fname, "rb");
   
   // Check if file open successfully
   if (!rf) { 
       fprintf(stderr,"Cannot open file!\n");
       return 1;
   }

  prefix_length=atoi(argv[2]);
  cnt=0;
  if (rf > 0) {
    do {
      res = fread(&prefix, prefix_length,1,rf);
      for(i=0;i<prefix_length;i++)
	  fprintf(stderr,"byte %d = %d\n",i,prefix[i]);
          res = fread(&packet,6,1,rf);
      if (res) {
	fwrite(&packet, 6,1,stdout);
	len = (unsigned short int)((packet[4]<<8)+packet[5]);
	fprintf(stderr, "len %i %8.8X\n", len, len);
	fread(&packet, len+1,1,rf);
	fwrite(&packet,len+1,1,stdout);
      }
	cnt++;
    } while (res > 0);
  }
  fclose(rf);
  fprintf(stderr,"processed %d packets\n",cnt-1);
}

