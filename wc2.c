/*-------------------------------------------------------------------------------
Program Name   : wc2.c
Program Author : Dominic Shields
Date           : 11/12/2002  
Purpose        : counts words in a file
-------------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

FILE *infil;
char *input;
char *p1;
char *p2;
char buf[500];

void main(argc,argv)
int argc;
char *argv[];
{
int i=0;
int spaceflag = 0;
long words=0;
int readin();

if(argc < 2)
{
	printf("Error, usage is %s filename\n",argv[0]);
	exit(1);
}

input = argv[1];

if((infil = fopen(input,"r")) == NULL)

{
	printf("Error, failure to open %s\n",input);
	exit(1);
}

readin();
spaceflag = 0;

while(feof(infil) == 0)
{
	for(i=0;i<strlen(buf);i++)
	{
		if(p1[0] == ' ' || p1[0] == '\n' || p1[0] == '\t')
		{
			spaceflag = 1;
		}
		
		if(spaceflag > 0 && p2[0] != ' ' && p2[0] != '\t')
		{
			words++;
		}
		spaceflag=0;
		p2 = p1;		
		p1++;
	}
	
	readin();
}

fclose(infil);

printf("Words in file %s = %ld\n",argv[1],words);

}

readin()
{
	int j;
	for(j=0;j<500;j++)
	{
		buf[j] = '\0';
	}

	fgets(buf,500,infil);
	p1=buf;

	return 0;
}
