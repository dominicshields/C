#include <stdio.h>

/*  Finds d for RSA algorithm when given n and e.  */

void main(void) {

   int n,d,e,phi,phiseg,x2test,edtest,a[2][3],factor;

   printf("\n Enter n: ");
   scanf("%d",&n);
   printf("\n Enter e: ");
   scanf("%d",&e);
   printf("\n Verify: \n n = %d \n e = %d \n\n Working ...",n,e);

/*  Find (d*e) candidates where 2^(d*e) mod n = 2  */
   phi=0;
   x2test=2;
   for (edtest=2;edtest<(n+1);edtest++){
      x2test=x2test+x2test;
      if (x2test>n){
         x2test=x2test-n;
      }
      if (x2test==2){
         phiseg=edtest-1;
         break;
      }
   }
   while (phi<n){
      phi=phi+phiseg;
   }
   phi=phi-phiseg;

/*  Calculate d  */
   a[0][0]=phi;
   a[0][1]=e;
   a[1][0]=0;
   a[1][1]=1;
   factor=(a[0][0])/(a[0][1]);
   a[0][2]=a[0][0]-factor*a[0][1];
   while (a[0][2]!=0){
      a[1][2]=a[1][0]-factor*a[1][1];
      a[0][0]=a[0][1];
      a[0][1]=a[0][2];
      a[1][0]=a[1][1];
      a[1][1]=a[1][2];
      factor=(a[0][0])/(a[0][1]);
      a[0][2]=a[0][0]-factor*a[0][1];
   }
   d=a[1][1];
   printf("\n\n d = %d\n",d);

   printf("\n phi = %d, phiseg = %d\n\n",phi,phiseg);
return (0);
}