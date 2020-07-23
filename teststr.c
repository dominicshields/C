#include <stdio.h>
#include <string.h>

#define ADMININQ "861:999:123:456"  

char *retval;

main(argc,argv)
int argc;
char *argv[];
{

 retval = strstr(ADMININQ,"9:12");
 if(retval != NULL)
		{
			fprintf(stdout,"ADMININQ contains test, retval = %s\n",retval);
		}
		else
		{
		fprintf(stdout,"ADMININQ does NOT contain test retval = %s\n",retval);
		}
		
		return 0;
}