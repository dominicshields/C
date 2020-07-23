/*-------------------------------------------------------------------------------
Program Name   : speed.c
Program Author : Dominic Shields
Date           : 19/10/1999  
Purpose        : First C Programming exercise to calculate stopping distances 
-------------------------------------------------------------------------------*/

#include <stdio.h>

int main()
{
	int v;

	lines();
	printf("* Speed (m.p.h.) * Stopping Distance (feet) *\n");
	lines();
	
	for(v=10;v<=50;v+=5)
	{
		printf("*\t%d\t *\t  %6.2f\t    *\n",v,(v*v)/20.0+v); 
	}

	lines();
	return 0;
}

int lines()
{
	printf("*********************************************\n");
	return 0;
}
