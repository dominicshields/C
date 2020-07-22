/* Finding all 10 digit numbers that include the all the numbers 
0 1 2 3 4 5 6 7 8 9 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

char numhold[11];
double num;
int y, x;
int match = 0;
time_t first, second;

main(argc,argv)
int argc;
char *argv[];
{
first = time(NULL);
fprintf(stdout,"\nProgram Begins  %s\n",ctime(&first));

num = 1234567890;
while (num < 10000000000)
{
	sprintf(numhold,"%f",num);
	for(x=0;x<10;x++)
	{
		for(y=0;y<10;y++)
		{
			if(numhold[x]==numhold[y])
			match++;
		}
		if(match > 10)
		{
			break;
		}
	}
	if (match == 10)
	{
		fprintf(stdout,"%.10s\n",numhold);
		fflush(stdout);
	}
	
	match = 0;
	num++;
}
second = time(NULL);
fprintf(stdout,"\nProgram Ends  %s\n",ctime(&second));
return 0;
}
