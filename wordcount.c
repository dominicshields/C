/*-------------------------------------------------------------------------------
Program Name   : wordcount.c
Program Author : Dominic Shields
Date           : 27/10/1999  
Purpose        : Fifth C Programming exercise to count words in a file
-------------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

FILE *infil;
char *input;
char *p1;
char buf[500];

void main(argc,argv)
int argc;
char *argv[];
{
int i=0;
int punctflag = 0;
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
punctflag = 0;

while(feof(infil) == 0)
{
	for(i=0;i<strlen(buf);i++)
	{
		if(ispunct(p1[0]) || p1[0] == ' ' || p1[0] == '\n')
		{
			if(punctflag == 0 && (p1[0] != '\'' && p1[0] != '-'))
			{
				words++;
				punctflag=1;
			}
		}
		else
		{
			punctflag=0;
		}
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
