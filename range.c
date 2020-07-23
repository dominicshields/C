/*-------------------------------------------------------------------------------
Program Name   : range.c
Program Author : Dominic Shields
Date           : 19/10/1999  
Purpose        : Second C Programming exercise to calculate range of a gun
-------------------------------------------------------------------------------*/

#include <stdio.h>
#include <math.h>

int main()
{
	int u, angle;

	lines();
	printf("* Speed(m/s) * Angle(degrees) * Range(m)    *\n");
	lines();
	
	for(u=10;u<=40;u+=10)
	{
		for(angle=15;angle<=75;angle+=15)
		{
			printf("*    %d      *     %4d       * %8.2lf    *\n",u,angle,u*u*sin(2*(angle/180.000)*3.1416)/9.8);
		}
	}

	lines();
	return 0;
}

int lines()
{
	printf("*********************************************\n");
	return 0;
}
