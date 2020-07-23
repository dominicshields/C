#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

int main(int argc,char *argv[])
{
long n ;
long d;
long cnt = 0;
long max;
long composite_flag; /*--Flag that's set equal to 1 if d divides n --*/
time_t tim;

tim = time(NULL);

/*printf("Enter the number below which you want to list the prime numbers: ");
scanf("%ld" , &max ); */

max = atol(argv[1]);

for (n = 1 ; n < max ; n++)
{
composite_flag = 0;
d = 2;
      while( (d <= sqrt(n)) && (composite_flag == 0))
      {
          if( n % d == 0)
          {
              composite_flag = 1;
              break;
          }
          d ++;
      }

   		if(composite_flag == 0)
   		{
   		  if ( n != 1 )
        {
       	 		printf("%ld\n" , n );
   					cnt++;
   		  }
      }
}

printf("\nFound %ld primes up to %ld\n" , cnt,max );

fprintf(stderr,"\nProgram Begins  %s\n",ctime(&tim));
tim = time(NULL);
fprintf(stderr,"Program Ends OK  %s\n",ctime(&tim));
   
return EXIT_SUCCESS;
}
