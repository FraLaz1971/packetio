#include "packets.h"
int main(int argc, char **argv){
  unsigned int i, res;
//  unsigned char *m1bv, *m2bv;
  struct Packet p1, p2;
  /* fill p1 */
  p1.ver = 0;
  p1.type = 0;
  p1.shf = 1;
  p1.apid = 804;
  p1.pid = 50;
  p1.cat = 4;
  p1.sf = 2;
  p1.ssc = 1023;
  p1.len = 9;
  p1.bv = (unsigned char *) malloc((p1.len+1)*sizeof(unsigned char));
  for(i=0; i<p1.len+1; i++)
      p1.bv[i]=(unsigned char)i%256;
  /* fill p2 */
  p2.ver = 4;
  p2.type = 0;
  p2.shf = 1;
  p2.apid = 844;
  p2.pid = 52;
  p2.cat = 12;
  p2.sf = 3;
  p2.ssc = 1024;
  p2.len = 65533;
  p2.bv = (unsigned char *) malloc((p2.len+1)*sizeof(unsigned char));
  for(i=0; i<p2.len+1; i++)
      p2.bv[i]=(i+1)%256;
  printf("swappacket: before the swap\n");
  printf("*****************\n");
  printf("*** packet p1 ***\n");
  printf("*****************\n");
  showpacket(&p1);
  printf("*****************\n");
  printf("*** packet p2 ***\n");
  printf("*****************\n");
  showpacket(&p2);
  printf("swappacket: now swapping\n");
  p1.bv = (unsigned char *)realloc(p1.bv,(p2.len+1)*sizeof(unsigned char));
  p2.bv = (unsigned char *)realloc(p2.bv,(p1.len+1)*sizeof(unsigned char));
  res = swappacket(&p1, &p2);
  printf("swappacket: after the swap\n");
  printf("*****************\n");
  printf("*** packet p1 ***\n");
  printf("*****************\n");
  showpacket(&p1);
  printf("*****************\n");
  printf("*** packet p2 ***\n");
  printf("*****************\n");
  showpacket(&p2);
  return 0;
}
