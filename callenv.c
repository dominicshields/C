#include <stdio.h>
#include <stdlib.h>

char * main(argc,argv)
int argc;
char *argv[];
{
	char * env = environment(argv[1]);
  fprintf(stdout,"%s\n",env);
  
}
