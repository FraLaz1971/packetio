#include <stdio.h>
#include "packets.h"
int main(int argc, char **argv){
  FILE *ifp;
  unsigned short int w,len;
  unsigned char b;
  unsigned long long cnt;
  char *fname;
  int i,res=1;
  if(argc<2){
    printf("usage:%s <TMfile>\n",argv[0]);
    return 1;
  }
  fname = argv[1];
  ifp=fopen(fname,"rb");
  if(!ifp){
    fprintf(stderr,"error in opening file %s\n",fname);
    return 1;
  }
  
  /* loop on all the packets */
  cnt=0;
  while(res){
    for(i=0;i<9;i++){
	res=fread(&w,2,1,ifp);
        if(!res) break;
    }
    res=fread(&w,2,1,ifp);
    if(!res) break;
    res=fread(&w,2,1,ifp);
    if(!res) break;
    res=fread(&len,2,1,ifp);
    if(!res) break;
    swapword(&len);
    for(i=0;i<len+1;i++){
      res=fread(&b,1,1,ifp);
      if(!res) break;
    }
    cnt++;
  }
  printf("read %lld packets\n",cnt);
  fclose(ifp);
  return 0;
}
