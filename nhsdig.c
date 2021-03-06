/*-----------------------------------------------------------------------      
nhs.c   a tweak of calc_ed_let.h written by Mike Jones 

Calculate check letters according to NHS rules:-
1. Multiply each digit by a position-dependent value (2 for least-significant, 3 for next least-significant, and so on)
2. Add the results of each multiplication together 
3. Divide the total by 11 and establish the remainder 
4. subtract the remainder from 11 to give the check digit 

There are two occasions where the check digit calculation process must be modified slightly:

if the result of step 4 is 11 then a check digit of 0 is used
if the result of step 4 is 10 then the number is invalid and not used. 
-----------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <time.h>

main(argc,argv)
int argc;
char *argv[];
{
	int n; /* Checksum */
	int x; /* Digit multiplier */
	int r; /* For current digit. (converted to int) */
	long valid = 0;
	long invalid = 0;
	long number = 1;
	char strnum[10];
	time_t tim;
	
	tim = time(NULL);
	fprintf(stderr,"\nProgram Begins  %s\n",ctime(&tim));
		
	while (number < 100)
	{
		sprintf(strnum,"%09d",number);
		printf("Strnum = %s\n",strnum);
		for(n = 0, x = (strlen (strnum) +1); x > 1; )
		{
			r = (int) *(argv[1]++) - '0';
			n += r * (x--);
		}
	
		if ((11-(n%11)) == 10)
		{
			invalid++;
		}
		else
		{
			valid++;
		}
		
		number++;
	}
	
	fprintf(stdout,"Valid Numbers   = %ld\n",valid);
	fprintf(stdout,"Invalid Numbers = %ld\n",invalid);
	
	tim = time(NULL);
	fprintf(stderr,"Program Ends OK  %s\n",ctime(&tim));
	return 0;
}
