/***************************************************
memfind  (source memfind.c)
***************************************************/
#include <stdio.h>
#include <stdlib.h>

main()
{
	char *p;
	long l;

	l=0;

	do {
		p = malloc(1000);
		if(p) l += 1000;
	   } while(p);

	fprintf(stdout,"Total mem = %ld\n",l);

	return(0);

}
