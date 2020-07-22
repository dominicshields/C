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
#include <stdlib.h>
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
	long max;
	char strnum[10];
	time_t tim;
	
	if(argc < 2)
	{
		printf("Error - Usage is %s <Max Number>\n",argv[0]);
		exit(1);
	}
	
	tim = time(NULL);
	fprintf(stderr,"\nProgram Begins  %s\n",ctime(&tim));
	fprintf(stderr,"\nFinding valid NHS numbers between 000000001 and %s\n",argv[1]);
	
	max = atol(argv[1]);
	while (number < max) 
	{
		sprintf(strnum,"%09ld",number);
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
		if(number%200000000 == 0)
		{
			tim = time(NULL);
			printf("Reached number %ld  at %s\n",number,ctime(&tim));
			fprintf(stdout,"Valid Numbers   = %ld\n",valid);
			fprintf(stdout,"Invalid Numbers = %ld\n\n",invalid);
		}			
	}
	
		
	tim = time(NULL);
	fprintf(stderr,"\n\n\nProgram Ends OK  %s\n",ctime(&tim));
	fprintf(stdout,"Valid Numbers   = %ld\n",valid);
	fprintf(stdout,"Invalid Numbers = %ld\n",invalid);
	return 0;
}
