/* Recursion Test */

#include <stdio.h>

int i=1;

main(argc,argv)
int argc;
char *argv[];
{
	recurseme();
	return 0;
}

recurseme()
{
	printf("Recursion level = %d\n",i);
	fflush(stdout);
	i++;
	if(i > 9)
	{
		return 0;
	}
	recurseme();
	return 0;
}