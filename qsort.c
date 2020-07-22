/*-------------------------------------------------------------------------------
Program Name   : qsort.c
Program Author : Dominic Shields
Date           : 22/02/2000  
Purpose        : Christine Valenzuela's Assessed Coursework
-------------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#define MAX 10000
#define RECLEN 500

FILE *infil;
FILE *outfil;
char *input;
char *p1;
char buf[RECLEN];
char v[RECLEN];
char hold1[RECLEN];
char hold2[RECLEN];
char list[MAX][RECLEN];
int a,b,i,j,upper,lower,arrcnt;
long qsortcalls, partitioncalls, exchangecalls, comparisons;
time_t tim;

void quicksort(char list[] [RECLEN], int lower,int upper);
int partition(char list[] [RECLEN],int lower,int upper);
int exchange(char list[] [RECLEN],int a,int b);
int readin(void);

void main(argc,argv)
int argc;
char *argv[];
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

if((outfil = fopen("sorted.txt","w")) == NULL)

{
	printf("Error on creating file sorted.txt\n");
	exit(1);
}

readin();
arrcnt=0;
qsortcalls=0;
partitioncalls=0;
exchangecalls=0;
comparisons=0;

while(feof(infil) == 0)
{
 	strcpy(list[arrcnt],buf); 
	arrcnt++;	
	readin();
}

tim = time(NULL);
printf("\nQsort starts at %s",ctime(&tim));
printf("Sorting %d records\n",arrcnt);

quicksort(list,0,arrcnt-1);

tim = time(NULL);
printf("Qsort ends at %s\n",ctime(&tim));
printf("Qsort function called %ld times\n",qsortcalls);
printf("Partition function called %ld times\n",partitioncalls);
printf("Exchange function called %ld times\n",exchangecalls);
printf("Comparisons = %ld, nlogn = %f, n squared = %f\n",comparisons,(float)arrcnt*(log(arrcnt)),(float)arrcnt*arrcnt);

fclose(infil);

for(i=0;i<arrcnt;i++)
{
	fprintf(outfil,"%s",list[i]);
}

fclose(outfil);
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

void quicksort(char list[] [RECLEN],int lower ,int upper)
{

	qsortcalls++;
	if(upper > lower)
	{
		j = partition(list,lower,upper);
		quicksort(list,lower,j - 1);	
		quicksort(list,j + 1,upper);
	}
}

int partition(char list[] [RECLEN],int lower,int upper)
{

	partitioncalls++;
	i=lower;
	j=upper + 1;

	strcpy(v,list[lower]);

	do
	{
		do
		{ 
			i = i+1;
			comparisons++;
		} while(strcmp(list[i],v) < 0);

		do
		{
			 j = j-1;
			 comparisons++;
		} while(strcmp(list[j],v) > 0);

		if(i<j)
		{
			a=i;
			b=j;
			exchange(list,i,j);
		}
	}
	while(i<j);
	exchange(list,lower,j);
	return(j);
	
}

int exchange(char list[] [RECLEN],int a,int b)
{

	exchangecalls++;
	
	strcpy(hold1,list[a]); 
	strcpy(hold2,list[b]); 

	strcpy(list[a],hold2);
	strcpy(list[b],hold1);

	return(0);
}
