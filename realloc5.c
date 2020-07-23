/* http://computer-programming-forum.com/47-c-language/5a89c447aff47e94.htm */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#define RECLEN 500
FILE *infil;
char *input;
char *p;
char **lines = NULL; 
char **copylines = NULL; 
char buf[RECLEN];
int c;
size_t nalloc = 0; 
size_t nlines = 0; 
 
int readin(void);

int main(int argc, char *argv[])
{
	
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
	
copylines = realloc(copylines, 5 * sizeof(char *)); 
lines = realloc(lines, 5 * sizeof(char *)); 
	
readin();	
while(feof(infil) == 0)
{ 
  /*  if(nlines >= nalloc) { 
        nalloc += 50; 
        lines = realloc(lines, nalloc * sizeof(char *)); 
        if(lines == NULL) { 
            fprintf(stderr, "out of memory"); 
            exit(1); 
        } 
    } */
    lines[nlines] = p; 
    printf("Assigned lines[%d] = %s",nlines,lines[nlines]);
    nlines++;
    readin();
 copylines[0] = "AAAA";
 copylines[1] = "BBBB";
 copylines[2] = "CCCC";
 copylines[3] = "DDDD";
 copylines[4] = "EEEE";
}
    
  for (c = 0; c < 5;c++)
 	{
 		printf("\nLines Array[%d] = %s",c,lines[c]);
 	}
 	
 	for (c = 0; c < 5;c++)
 	{
 		printf("\nCopylines Array[%d] = %s",c,copylines[c]);
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
	p=buf;

	return 0;
} 