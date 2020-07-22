/*-------------------------------------------------------------------------------
Program Name   : filprim.c
Program Author : Dominic Shields
Date           : 24/10/1999  
Purpose        : Generates prime number
-------------------------------------------------------------------------------*/

#include <stdio.h>
#include <math.h>
#include <string.h>

FILE *output;

int main(argc,argv)
int argc;
char *argv[];
{
int i=2;
int flag;

if ((output = fopen(argv[1],"w")) == NULL)
{
	fprintf(stderr,"Error opening %s\n",output);
	exit(1);
}

while(i<100000)
{
	int j=2;
	flag = 0;

	while(j <= sqrt(i))
	{
		if(i%j==0)
		{
			flag = 1;	
		}
		j++;
	}

	if(flag == 0)
	{
		fprintf(output,"%d\n",i);
	}

	i++;
}
	
return 0;
}
