#include <stdio.h>
int main(int argc, char **argv){
  FILE *ifp,*ofp;
  unsigned char b;
  int i;
  if(argc<3){
    printf("usage:%s <infile> <outfile>",argv[0]);
    return 1;
  }
  char *ifname=argv[1],*ofname=argv[2];
  
  ifp=fopen(ifname, "rb");
  ofp=fopen(ofname, "wb");
  for(i=0;i<12;i++)
    fread(&b,1,1,ifp);
  for(i=0;i<2258;i++){
    fread(&b,1,1,ifp);
    fwrite(&b,1,1,ofp);
  }
  fclose(ifp);
  fclose(ofp);
  return 0;
}
