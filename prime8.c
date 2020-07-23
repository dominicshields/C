#include <stdio.h>
#include <malloc.h>
void sieve(float *, float);

int main(int argc, char *argv[])
{
    float *array, n=10000;
    array =(float *)malloc(sizeof(float));

    sieve(array,n);
    return 0;
}

void sieve(float *a, float n)
{
    float i, j;
    for(i=2; i<=n; i++) {
        a[i] = 1;
    }

    for(i=2; i<=n; i++) {
        if(a[i] == 1) {
            for(j=i; (i*j)<=n; j++) {
                a[(i*j)] = 0;
            }
        }
    }

    printf("\nPrime numbers from 1 to %f are : ", n);
    for(i=2; i<=n; i++) {
        if(a[i] == 1)
            printf("%ld, ", i);
    }
    printf("\n\n");
}