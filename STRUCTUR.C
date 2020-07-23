#include <stdio.h>
#include <string.h>

typedef struct Z {
int j;
struct Z *bert; 
} Z;

struct Z *a;

main(argc,argv)
int argc;
char *argv[];
{
	c = &b;
	b.i = 10;
	c->i = 10;
	strcpy(c->bert,"Wibble, wibble");

	fprintf(stdout,"%d %s %s\n",b.i,c->i,c->bert);
}

