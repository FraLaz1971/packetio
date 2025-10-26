#include <stdio.h>
#include <stdlib.h>
int main(){
  int *arr1, *arr2, i;
  int len1=512, len2=2048;
  arr1 = malloc(len1*sizeof(int));
  arr2 = malloc(len2*sizeof(int));
  for(i=0; i<len1; i++){
    arr1[i] = i;
  }
  for(i=0; i<len2; i++){
    arr2[i] = i*i;
  }

  printf("*** arr1 ***\n");
  for(i=0; i<len1; i++){
    printf("arr1[%d]: %d\n",i, arr1[i]);
  }
  printf("*** arr2 ***\n");
  for(i=0; i<len2; i++){
    printf("arr2[%d]: %d\n",i, arr2[i]);
  }
  arr1 = realloc(arr1, len2*sizeof(int));
  arr2 = realloc(arr2, len1*sizeof(int));

  printf("*** arr1 ***\n");
  for(i=0; i<len2; i++){
    printf("arr1[%d]: %d\n",i, arr1[i]);
  }
  printf("*** arr2 ***\n");
  for(i=0; i<len1; i++){
    printf("arr2[%d]: %d\n",i, arr2[i]);
  }

  return 0;
}
