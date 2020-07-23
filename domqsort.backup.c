/*-------------------------------------------------------------------------------
Program Name   : domqsort.c
Program Author : Dominic Shields
Date           : 23/07/2016 21:05:31
Purpose        : Implementation of QuickSort https://en.wikipedia.org/wiki/Quicksort
                 using realloc/malloc and pointer to pointer array
Usage          : domqsort unsorted_file Y (optional flag to delete duplicates
Output         : sorted.txt
-------------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#define RECLEN 1000   /* Longest input record - may need to increase */

FILE *infil;
FILE *outfil;
char *input;
char *hold1;
char **list;
char buf[RECLEN];
char dups[1];

long a,b,i,j,upper,lower,arrcnt,writecnt;
long qsortcalls, partitioncalls, exchangecalls, comparisons;
time_t tim;

void quicksort(char **list,int lower,int upper);
int partition(char **list,int lower,int upper);
int exchange(char **list,int a,int b);
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

if (argc == 3)
{
	*dups = *argv[2];  /* If set to Y, duplicate records should be discarded */
}
else
{
	*dups = 'N';
}

if((outfil = fopen("sorted.txt","w")) == NULL)

{
	printf("Error on creating file sorted.txt\n");
	exit(1);
}

arrcnt=0;
qsortcalls=0;
partitioncalls=0;
exchangecalls=0;
comparisons=0;
writecnt=0;
tim = time(NULL);
printf("\nQsort starts at %s",ctime(&tim));
printf("Suppress duplicates flag = %c\n",*dups);
readin();
while(feof(infil) == 0)
{
	list = realloc(list, (arrcnt+1) * sizeof(char *));  /* Allocates memory for a new pointer in list (one ahead) */
  list[arrcnt] = malloc(strlen(buf)+1);               /* Allocates memory for what the new pointer points to (plus 1 for newline) */
  strcpy(list[arrcnt], buf);
	arrcnt++;
	readin();
}

printf("Sorting %d records\n",arrcnt);

quicksort(list,0,arrcnt-1);

fclose(infil);

for(i=0;i<arrcnt;i++)
{
	if (*dups == 'Y')  
	{
		if(i>0)  /* Don't go out of bounds with the first record  */
		{
			if(strcmp(list[i-1],list[i]) != 0)
			{
				fprintf(outfil,"%s",list[i]);
				writecnt++;
			}
		}	
		else
		{
				fprintf(outfil,"%s",list[i]);
				writecnt++;
		}
	}
	else
	{
		fprintf(outfil,"%s",list[i]);
		writecnt++;
	}
}
fclose(outfil);
printf("Records Written %ld\n",writecnt);
tim = time(NULL);
printf("Qsort ends at %s\n",ctime(&tim));
/*printf("Qsort function called %ld times\n",qsortcalls);
printf("Partition function called %ld times\n",partitioncalls);
printf("Exchange function called %ld times\n",exchangecalls);
printf("Comparisons = %ld, nlogn = %f, n squared = %f\n",comparisons,(float)arrcnt*(log(arrcnt)),(float)arrcnt*arrcnt); */
}

int readin()
{
	int j;
	for(j=0;j<RECLEN;j++)
	{
		buf[j] = '\0';
	}

	fgets(buf,RECLEN,infil);

	return 0;
}

void quicksort(char **list,int lower ,int upper)
{
	qsortcalls++;
	if(upper > lower)
	{
		j = partition(list,lower,upper);
		quicksort(list,lower,j - 1);
		quicksort(list,j + 1,upper);
	}
}

int partition(char **list,int lower,int upper)
{
	partitioncalls++;
	i=lower;
	j=upper + 1;
	do
	{
		do
		{
			i = i+1;
			comparisons++;
		} while( strcmp(list[i],list[lower]) < 0 && i+1 < j);


		do
		{
			 j = j-1;
			 comparisons++;
		} while(strcmp(list[j],list[lower]) > 0);

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

int exchange(char **list,int a,int b)
{
	exchangecalls++;

	hold1 = list[a];
	list[a] = list[b];
	list[b] = hold1;
	return(0);
}
