#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/*#define LIMIT  400000000 /size of integers array*/
/*#define PRIMES 20000000 /size of primes array*/

#define LIMIT  10000 /*size of integers array*/
#define PRIMES 1000 /*size of primes array*/

int main()
{
	long i;
	long j;
	long *primes;
	long *numbers;
	long cnt = 0;
	time_t tim;
	tim = time(NULL);

	primes = malloc(PRIMES * sizeof(long));
	if(!primes)
	{
		printf("Prime allocation too big\n");
		return 0;
	}
	numbers = malloc(LIMIT * sizeof(long));
	if(!numbers)
	{
		printf("Numbers allocation too big\n");
		return 0;
	}

   /*fill the array with natural numbers*/
    for (i=0;i < LIMIT;i++)
    {
        	numbers[i] = i+2;
    }
    
    /*sieve the non-primes*/
    for (i=0;i < LIMIT;i++)
    {
        if (numbers[i]!=-1)
        {
            for (j=2*numbers[i]-2;j < LIMIT;j += numbers[i])
            {
                numbers[j]=-1;
            }
        }
    }

    /*transfer the primes to their own array*/
    j = 0;
    for (i=0;i < LIMIT && j < PRIMES;i++)
    {
        if (numbers[i]!=-1)
        {
            primes[j++] = numbers[i];
   					cnt++;
        }
		}
    /*print*/
    for (i=0;i < PRIMES;i++)
    {
        printf("%ld\n",primes[i]);
   	}

   	free(primes);
   	
   	fprintf(stderr,"\nFound %ld primes up to %ld\n" , cnt,LIMIT );

		fprintf(stderr,"\nProgram Begins  %s\n",ctime(&tim));
		tim = time(NULL);
		fprintf(stderr,"Program Ends OK  %s\n",ctime(&tim));

return 0;
}