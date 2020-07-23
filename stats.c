/*-------------------------------------------------------------------------------
Program Name   : stats.c
Program Author : Dominic Shields
Date           : 1/11/1999  
Purpose        : Sixth C Programming exercise to count letters in a file
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
int intarr[26];
int i=0;
int readin();

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

for(i=0;i<26;i++)
{
	intarr[i] = 0;
}

readin();

while(feof(infil) == 0)
{
	for(i=0;i<strlen(buf);i++)
	{
		if(isalpha(p1[0]))
		{
			intarr[toupper(p1[0]) - 65]++;
		}
		p1++;
	}
	
	readin();
}

fclose(infil);

printf("Letter\tFrequency\n");

for(i=0;i<26;i++)
{
	printf("  %c\t%6d\n",i+65,intarr[i]);
}

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

