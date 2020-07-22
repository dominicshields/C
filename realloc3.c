#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initialize(double ***A, int r, int c)
{
	int i;
	int j;
   *A = (double **)malloc(sizeof(double *)*r);
   for( i = 0; i< r; i++) {
        (*A)[i] = (double *)malloc(sizeof(double) *c);
        for(j = 0; j < c; j++) {
            (*A)[i][j] = i*j;
            printf("(*A)[%d][%d] = %lf\n",i,j,(*A)[i][j]);
        }
   }
}
int main()
{
    double **A;
    initialize(&A, 10, 10);
    return 0;
}