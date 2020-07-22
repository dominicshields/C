#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    long number,i,j;
    float *primes;
    printf("Enter the number\n");
    scanf("%ld",&number);

    primes = malloc(sizeof(float) * (number+1));
		if(!primes)
		{
			printf("Prime allocation too big\n");
			return 0;
		}
	
    for(i = 2; i<=number; i++)
    {
        primes[i] = i;
    }

    i = 2;
    while ((i*i) <= number)
    {
        if (primes[i] != 0)
        {
            for(j=2; j<number; j++)
            {
                if (primes[i]*j > number)
                    break;
                else
                    primes[primes[i]*j]=-1;
            }
        }
        i++;
    }

    for(i = 2; i<=number; i++)
    {
        if (primes[i]!=-1)
        {
          printf("%.0f\n",primes[i]);
        }
    }

    free(primes);

    return 0;
}
