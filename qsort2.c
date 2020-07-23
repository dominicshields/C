/* qsort example */
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* qsort */

char *values[] = { "Z12", "B10", "F00", "T90", "E20", "D25" };

int compare (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}

int main ()
{
  int n;
  qsort (values, 6, 3, compare);
  for (n=0; n<6; n++)
     printf ("%d ",values[n]);
  return 0;
}