/* A spigot algorithm for the Digits of \pi, Stanley Rabinowitz
   and Stan Wagon, Amer.Math.Monthly, March 1995, 195-203
*/

#include <stdio.h>

#define N 1000
#define LEN 10*N/3

main()
{
int i, j, k, q, x, nines, predigit;
long a[LEN];

for(j=0;j<LEN;j++) a[j]=2;
nines=0; predigit=0;
for(j=0;j<N;j++) {
        q=0;
        for(i=LEN;i;i--) {
           x = 10*a[i-1] + q * i;
           a[i-1] = x % (2*i-1);
           q = x / (2*i-1);
        }
        a[0] = q % 10; q = q / 10;
        if (q==9) { nines++; }
        else if (q==10) { printf("%d",predigit+1); predigit=0;
                          for(;nines;nines--) printf("%d",0);
                        }
        else { printf("%d",predigit); predigit = q;
               for(;nines;nines--) printf("%d",9);
             };
};
printf( "%d\n", predigit );
}
