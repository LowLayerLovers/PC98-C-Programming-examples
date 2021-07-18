#include <stdio.h>

int main(void) {
  puts("[Data Type Ranges(Bytes)]");
  printf("char   = %d\n" ,sizeof(char));
  printf("int    = %d\n" ,sizeof(int));
  printf("short  = %d\n" ,sizeof(short));
  printf("long   = %d\n" ,sizeof(long));
  printf("long long = %d\n" ,sizeof(long long));
  printf("long long int = %d\n\n" ,sizeof(long long int));
  
  printf("float  = %d\n" ,sizeof(float));
  printf("double = %d\n\n" ,sizeof(double));

  int *p;
  printf("pointer = %d\n" ,sizeof(p));

  return 0;
}