#include "packets.h"
int main(int argc, char **argv){
  struct Packet *pv; /* packets vector */
  FILE *ifp, *ofp;
  unsigned short int w,w0,w1,len;
  unsigned char b;
  unsigned long long cnt,i,j;
  int res=1,reso=1, ress;
  if(argc<3){
    printf("usage:%s <inTMfile> <outTMfile>\n",argv[0]);
    return 1;
  }
  char *fname = argv[1], *ofname = argv[2];
  cnt = countpackets(fname);
  printf("read %lld packets\n",cnt);
  printf("size of a packet struct: %lu\n",sizeof(struct Packet));
  pv=(struct Packet *) malloc(cnt*sizeof(struct Packet));
  ifp=fopen(fname,"rb");
  if(!ifp){
    fprintf(stderr,"error in opening input file %s\n",fname);
    return 1;
  }
  ress=sortpackets(pv, cnt);
  /* loop on all the packets */
  cnt=0;
  while(res){
    pv[cnt].data = (unsigned short int *) malloc (3*sizeof(unsigned short));
    res=fread(&pv[cnt].data[0],2,1,ifp);
    if(!res) break;
    res=fread(&pv[cnt].data[1],2,1,ifp);
    if(!res) break;
    res=fread(&pv[cnt].data[2],2,1,ifp);
    if(!res) break;
    len = pv[cnt].data[2];
    swapword(&len);
    pv[cnt].len = len;
    pv[cnt].bv = (unsigned char *) malloc((len+1)*sizeof(unsigned char));
    for(i=0;i<len+1;i++){
      res=fread(&pv[cnt].bv[i],1,1,ifp);
      if(!res) break;
      }
    cnt++;
  }
  ofp=fopen(ofname,"wb");
  if(!ofp){
    fprintf(stderr,"error in opening output file %s\n",fname);
    return 1;
  }
  // write all packets in the output file
  for(i=0; i<cnt; i++){ // for loop on all packets
  // write header
    if (debug) printf("writing packet %llu\n", i);
    for(j=0; j<3; j++){
    if (debug) printf("writing packet %llu hw %llu\n", i, j);
      reso=fwrite(&pv[i].data[j],2,1,ofp);
      if(!reso) break;
    }// end loop on the 3 16 bit words of the header
   free(pv[i].data);
 // write data field
    for(j=0;j<pv[i].len+1;j++){
    if (debug) printf("writing packet %llu df byte %llu\n", i, j);
      reso=fwrite(&pv[i].bv[j],1,1,ofp);
  } // end loop on all bytes of the data field
  free(pv[i].bv);
} // and of while loop on all packets
  fclose(ifp);
  free(pv);
  return 0;
}
