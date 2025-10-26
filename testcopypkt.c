#include "packets.h"
int main(int argc, char **argv){
  int i, res;
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
      p1.bv[i]=i;
  /* fill p2 */
  p2.ver = 4;
  p2.type = 0;
  p2.shf = 1;
  p2.apid = 844;
  p2.pid = 52;
  p2.cat = 12;
  p2.sf = 3;
  p2.ssc = 1024;
  p2.len = 25;
  p2.bv = (unsigned char *) malloc((p2.len+1)*sizeof(unsigned char));
  for(i=0; i<p2.len+1; i++)
      p2.bv[i]=i*i;
  printf("testcopypkt: before the copy\n");
  printf("packet p1\n");
  showpacket(&p1);
  printf("packet p2\n");
  showpacket(&p2);
  printf("testcopypkt: now copying\n");
  copypacket(&p1, &p2);
  printf("testcopypkt: after the copy\n");
  printf("packet p1\n");
  showpacket(&p1);
  printf("packet p2\n");
  showpacket(&p2);
  free(p1.bv);
  free(p2.bv);
  return 0;
}
