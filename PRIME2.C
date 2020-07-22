#include <stdlib.h>
#include <stdio.h>
#include <math.h>


int main()
{
long n ; /*--   Number that is to be tested for being prime  --*/
long d = 3 ;  /*--  Number that is used as potential factor of   n  --*/
long composite_flag = 0 ; /*--Flag that's set equal to 1 if d divides n --*/

printf("Enter an integer: ");
scanf("%ld" , &n );
  if ( n == 1 )
    printf("\nThe number 1 is neither prime nor composite.\n"  );
  else if ( (n == 2) || (n == 3) || (n == 5) || (n == 7) )
    printf("\nThe number %ld is prime.\n" , n );
  else if(n % 2 == 0)
     printf("\nThe number %ld is even and therefore composite.\n" , n );
  else
     {
      while( (d <= sqrt(n)) && (composite_flag == 0))
        {
          if( n % d == 0)
            {
              composite_flag = 1;
              printf("\nThe number %ld is composite with %ld as a factor.\n" ,
                                                                        n , d);
            }
         d += 2;
        }
      }

   if(composite_flag == 0)
       printf("\n\nThe number %ld is prime.\n" , n );

return EXIT_SUCCESS;
}

/*-----------------------------------------------------------------------------
Note: The flag could have been eliminated by concluding the program as follows:

  else
     {
      while( d <= sqrt(n) )
        {
          if( n % d == 0)
            {
              printf("\nThe number %ld is composite with %ld as a factor.\n" ,
                                                                        n , d);
              return EXIT_SUCCESS;
            }
         d += 2;
        }
       }

       printf("\n\nThe number %ld is prime.\n" , n );

return EXIT_SUCCESS;
}

-----------------------------------------------------------------------------*/
