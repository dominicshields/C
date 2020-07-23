/*      checklet.c   an interactive copy of calc_ed_let.h written
		     by Mike Jones and shamelessly blagged by Dominic.
		     My "contribution" was the argc and argv[].

	Calculate check letters according to ED rules:-
	Multiply each digit by a position-dependent value (2 for
	least-significant, 3 for next least-significant, and so on); add
	the products; then divide the sum by 11. Map the remainder on to
	11 (non-consecutive!) letters as follows:-

	0 = A; 1 = B; 2 = C; 3 = D; 4 = F; 5 = H; 6 = J; 7 = K; 8 = L;
	9 = S; 10 = T

	Example:- 01532257
	(0*9) +(1*8) +(5*7) +(3*6) +(2*5) +(2*4) +(5*3) +(7*2) = 108
	108 % 11 = 9 (remainder)
	9 maps on to S; so check letter is S.
*************************************************************************/
#include <stdio.h>
#include <stdlib.h>

main(argc,argv)
int argc;
char *argv[];
{
	int n; /* Checksum */
	int x; /* Digit multiplier */
	int r; /* For current digit. (converted to int) */

	for(n = 0, x = (strlen (argv[1]) +1); x > 1; )
	{
		r = (int) *(argv[1]++) - '0';
		n += r * (x--);
	}
	fprintf(stderr,"%c\n",("ABCDFHJKLST" [n % 11]));
}
