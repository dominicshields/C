/*-------------------------------------------------------------------------------
Program Name   : allprime.c
Program Author : Dominic Shields
Date           : 24/10/1999  
Purpose        : Generates prime number
-------------------------------------------------------------------------------*/

#include <stdio.h>
#include <math.h>
#include <string.h>

int main()
{
int i=2;
int flag;

while(i<1000)
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
		printf("%d\n",i);
	}

	i++;
}
	
return 0;
}
