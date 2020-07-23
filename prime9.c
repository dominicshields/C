#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <math.h>
#include <time.h>
#include <sys\timeb.h> 

static char* get_timestamp(char* buf) {
    struct timeb start;
    char append[100];
    if (buf) {
        ftime(&start);
        strftime(buf, 100, "%H:%M:%S", localtime(&start.time));

        /* append milliseconds */
        sprintf(append, ".%03u", start.millitm);
        strcat(buf, append);
    }
    return buf;
}

int main(int argc, char* argv[]) {
  long n = argc > 1 ? atol(argv[1]) : 100;
  long* S;
  long count;
  long sz = n * sizeof(*S);
  long i, j;
  long sqrt_n;
  char start[100];
	char end[100];
	
	printf("%s\n", get_timestamp(start));

  sqrt_n = sqrt(n) + 1;

  S = malloc(sz);
  memset(S, 0, sz);

  for (i = 2; i < sqrt_n; ++i)
    if (S[i] == 0)
      for (j = i*i; j < n; j+=i)
        S[j] = 1;

  count = 0;
  for (i = 2; i < n; ++i)
    if (S[i] == 0)
      count++;

  printf("Found %d Primes\n", count);

  free(S);
  
  printf("%s\n", get_timestamp(end));
  
  return 0;
}