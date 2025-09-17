/* reads a real number and gives back timestamp */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
int main(int argc, char**argv){
	if(argc<2) {
		fprintf(stderr,"usage: %s <time>\n",argv[0]);
		return 1;
		}
	int debug = 1;
	double mytime=atof(argv[1]);
	time_t mytimet=(time_t)mytime; /* input time seconds */
        double bcoffset = 935314068.790145;
	double bctime=mytime+bcoffset;
	time_t bctimet=(time_t)bctime;
	double mytimeud; /* input time microseconds */
	unsigned int mytimeu;
	mytimeud = (mytime-(double)mytimet)*1000000.0;
	mytimeu = (unsigned int)round(mytimeud);
	char *dt;
	if (debug) printf("read input time: %f\n",mytime);
	if (debug) printf("input time seconds: %ld\n",mytimet);
	if (debug) printf("input time microseconds (double): %f\n",mytimeud);
	if (debug) printf("input time microseconds (uint): %u\n",mytimeu);
	// Structure to store local time
    struct tm* ptr;
    
    // Variable to store current time
    time_t t;
    
    // Get current time
/*    t = time(NULL); */
      dt = ctime(&mytimet);
    // Convert it to local time
   /* ptr = gmtime(&t); */
    
    // Get the string of local time
/*    printf("UTC: %s", asctime(ptr)); */
    printf("TIME: %s\n", dt); 
    dt = ctime(&bctimet);
    printf("BC TIME: %s\n", dt); 
   
	return 0;
}
