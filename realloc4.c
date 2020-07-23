#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RECLEN 500
FILE *infil;
char *input;
char *p1;
char buf[RECLEN];
 
int readin(void);

int main(int argc, char *argv[])
{
 char **a;

 int x;
    
if(argc < 2)
{
	printf("%s: No filename specified\n",argv[0]);
	exit(1);
}

input = argv[1]; 

if((infil = fopen(input,"r")) == NULL)

{
	printf("Error on opening file %s\n",input);
	exit(1);
}

for(x = 0; x < 5; x++) 
{
    readin();
    printf("Diag 1 strlen(buf) * sizeof(char) + 1 = %d \n",strlen(buf) * sizeof(char) + 1);
    /* acquire space for string just read */
/*    a[x] = (char *)malloc(strlen(buf) * sizeof(char) + 1); */
    a[x] = malloc(100 * sizeof(char*));
    printf("Diag 2, buf = %s",buf);
    strcpy(a[x], buf); 
    printf("Diag 3, a[%d] = %s",x,a[x]);
    /* expand the pointer list */
    a = (char **) realloc(a, x * (sizeof(char*)));
    printf("Diag 4\n");
}

return 0;
}

int readin()
{
	int j;
	for(j=0;j<RECLEN;j++)
	{
		buf[j] = '\0';
	}

	fgets(buf,RECLEN,infil);
	p1=buf;

	return 0;
}