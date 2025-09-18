/* reads a real number and gives back timestamp */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
int main(int argc, char**argv){
	if(argc<2) {
		fprintf(stderr,"usage: %s <time>\n",argv[0]);
		return 1;
		}
	int debug = 1;
        int year,mon;
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
        char bcstrt[128]; 
	if (debug) printf("read input time: %f\n",mytime);
	if (debug) printf("input time seconds: %ld\n",mytimet);
	if (debug) printf("input time microseconds (double): %f\n",mytimeud);
	if (debug) printf("input time microseconds (uint): %u\n",mytimeu);
	// Structure to store local time
    struct tm* ptr;
    // Variable to store current time
    time_t t;
    dt = ctime(&mytimet);
    printf("TIME: %s\n", dt); 
    dt = ctime(&bctimet);
    printf("BC TIME: %s\n", dt); 
    // Get current time
    t = time(NULL); 
    // Convert it to universal time
     ptr = gmtime(&t); 
    // Get the string of universal time
    printf("UTC: %s", asctime(ptr));
  year=ptr->tm_year+1900;
  mon=ptr->tm_mon+1; 
  snprintf(bcstrt,128,"%d-%2d-%2d:%2d:%2d:%2d",year,mon,\
  ptr->tm_mday,ptr->tm_hour,ptr->tm_min,ptr->tm_sec);
  printf("BC Tm str: %s\n",bcstrt);
  printf("Year: %d\n", ptr->tm_year + 1900);  // Add 1900 to get the actual year
  printf("Month: %d\n", ptr->tm_mon + 1);     // Months are numbered from 0 to 11, so add 1 to match real month numbers (1-12)
  printf("Day: %d\n", ptr->tm_mday);
  printf("Hour: %d\n", ptr->tm_hour);
  printf("Minute: %d\n", ptr->tm_min);
  printf("Second: %d\n", ptr->tm_sec);
  printf("%d-%2d-%2dT%2d:%2d:%2d\n",year,mon,\
  ptr->tm_mday,ptr->tm_hour,ptr->tm_min,ptr->tm_sec);
	return 0;
}
