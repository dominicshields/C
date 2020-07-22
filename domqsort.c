/*-------------------------------------------------------------------------------
Program Name   : domqsort.c
Program Author : Dominic Shields
Date           : 26/07/2016 11:29:27
Purpose        : Implementation of QuickSort https://en.wikipedia.org/wiki/Quicksort
                 using realloc/malloc and pointer to pointer array
Usage          : domqsort unsorted_file Y (optional flag to delete duplicates)
Output         : sorted.txt
-------------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define RECLEN 2000   /* Longest input record - may need to increase */

FILE *infil;
FILE *outfil;
char *input;
char *hold1;
char **list;
char buf[RECLEN];
char dups[1];
time_t tim;

void quicksort(char **list,long lower,long upper);
int partition(char **list,long lower,long upper);
int exchange(char **list,long a,long b);
int readin(void);

void main(int argc,char *argv[])
{
long x,arrcnt,writecnt;

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
writecnt=0;
tim = time(NULL);
printf("\ndomqsort starts at %s",ctime(&tim));
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

for(x=0;x<arrcnt;x++)
{
	if (*dups == 'Y')  
	{
		if(x>0)  /* Don't go out of bounds with the first record  */
		{
			if(strcmp(list[x-1],list[x]) != 0)
			{
				fprintf(outfil,"%s",list[x]);
				writecnt++;
			}
		}	
		else
		{
				fprintf(outfil,"%s",list[x]);
				writecnt++;
		}
	}
	else
	{
		fprintf(outfil,"%s",list[x]);
		writecnt++;
	}
}
fclose(outfil);
printf("Records Written %ld\n",writecnt);
tim = time(NULL);
printf("domqsort ends at %s\n",ctime(&tim));
}

int readin()
{
	long z;
	for(z=0;z<RECLEN;z++)
	{
		buf[z] = '\0';
	}

	fgets(buf,RECLEN,infil);

	return 0;
}

void quicksort(char **list,long lower ,long upper)
{
	long y;
	if(upper > lower)
	{
		y = partition(list,lower,upper);
		quicksort(list,lower,y - 1);
		quicksort(list,y + 1,upper);
	}
}

int partition(char **list,long lower,long upper)
{
  long i,j;
	i=lower;
	j=upper + 1;
	do
	{
		do
		{
			i = i+1;
		} while( strcmp(list[i],list[lower]) < 0 && i+1 < j);


		do
		{
			 j = j-1;
		} while(strcmp(list[j],list[lower]) > 0);

		if(i<j)
		{
			exchange(list,i,j);
		}
	}
	while(i<j);
	exchange(list,lower,j);
	return(j);
}

int exchange(char **list,long a,long b)
{
	hold1 = list[a];
	list[a] = list[b];
	list[b] = hold1;
	return 0;
}
