#include <stdio.h>
#include <stdlib.h>
 
#define limit 1000 /*size of array*/
 
int main(){
    double i,j;
    double *primes;
 
    primes = malloc(sizeof(double) * limit);
 
    for (i = 2;i < limit; i++)
        primes[i] = 1;
 
    for (i = 2;i < limit; i++)
        if (primes[i])
            for (j = i;i * j < limit; j++)
                primes[i * j] = 0;
 
    printf("\nPrime numbers in range 1 to 100 are: \n");
    for (i = 2;i < limit; i++)
        if (primes[i])
            printf("%.0lf\n", i);
 
return 0;
}