/* pentium.c - test for Pentium floating point bug */

#include <stdio.h>

main()
{

printf("Result of calculation 1.0/824633702449.0*824633702449.0 = %lf\n",(1.0/824633702449.0)*824633702449.0);

return(0);
}