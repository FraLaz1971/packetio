/* reads a real number and gives back timestamp */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "packets.h"
int main(int argc, char**argv, double ts){
	if(argc<2) {
		fprintf(stderr,"usage: %s <time>\n",argv[0]);
		return 1;
		}
	char bcstrt[128]; 
	double mytime=atof(argv[1]);
	getBC_UTC(bcstrt, mytime);
	printf("main(): BC Tm str: %s\n",bcstrt);
	return 0;
}
