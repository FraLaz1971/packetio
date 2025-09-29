#include <stdio.h>
int main(int argc, char **argv){
  FILE *ifp,*ofp;
  unsigned char b;
  int i,res,ores,cnt;
  if(argc<3){
    printf("usage:%s <infile> <outfile>",argv[0]);
    return 1;
  }
  char *ifname=argv[1],ofname[32];;
  
  ifp=fopen(ifname, "rb");
  cnt=1;res=1;
  while (res){
    for(i=0;i<12;i++)
      res=fread(&b,1,1,ifp);
    snprintf(ofname,32,"%s_%d.bin",argv[2],cnt);
    ofp=fopen(ofname, "wb");
    for(i=0;i<2258;i++){
      res=fread(&b,1,1,ifp);
      ores=fwrite(&b,1,1,ofp);
    }
    cnt++;
    fclose(ofp);
  }
  fclose(ifp);
  printf("extracted %d frames",cnt);
  return 0;
}
