/*-------------------------------------------------------------------------------
Program Name   : rand.c
Program Author : Dominic Shields
Date           : 22/02/2000  
Purpose        : Christine Valenzuela's Assessed Coursework, random number
				 generator
-------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

FILE *outfil;
long max,cnt;
time_t tim;

void main(argc,argv)
int argc;
char *argv[];
{

if(argc < 2)
{
	printf("%s: Error, usage is %s Number required\n",argv[0],argv[0]);
	exit(1);
}

max = atol(argv[1]);

if((outfil = fopen("random.txt","w")) == NULL)

{
	printf("Error on creating file random.txt\n");
	exit(1);
}

cnt=0;
srand( (unsigned)time( NULL ) );

while(cnt<max)
{
	fprintf(outfil,"%d\n",rand()%9999);
	cnt++; 	
}

fclose(outfil);

}
