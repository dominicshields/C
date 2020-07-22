/* Calculate checkdigit according to VAT rules:-
 Multiply each digit by a position-dependent value (2 for
 least-significant, 3 for next least-significant, and so on); add
 the products; then divide the sum by 97. Checkdigit is 97 minus
 remainder.

 Example:- 1062764
 (1*8) + (0*7) + (6*6) + (2*5) + (7*4) + (6*3) + (4*2)
 = 8   +   0   +  36   +  10   +  28   +  18   +   8 
 = 108
 108 % 97 = 11
 Checkdigit is (97 - 11) i.e. 86

$Based on ycrvatcheck
$Author: mike $
$Date: 93/02/16 12:10:17 $
*/

#include <stdio.h>

iro_pr_calc_vatcheck(vatref,checkdigit)
char *vatref;
int *checkdigit;
{
 int res=0;
 int n;                                              /* For intermediate calculations */
 long r;                                             /* Ref no. (converted to int) */
 int x;                                              /* Digit multiplier */
 
 /* Initialise n, x, & r; then while r > 0 */
 
 for (n = 0, x = 2, r = atol (vatref);r; )
 {
                                                      /* Process current least-significant digit (remainder from
                                                       mod 10 division); increment multiplier */
  n += ((r % 10) * (x++));
  r /= 10;                                             /* Shift right (integer divide by 10) */
 }
 
 *checkdigit = (97 - (n % 97));
 
 return (0);

}