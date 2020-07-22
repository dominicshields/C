/*-------------------------------------------------------------------------------
Program Name   : domqsortbypos.c
Program Author : Dominic Shields
Date           : 11/11/2016 15:09:43
Purpose        : Implementation of QuickSort https://en.wikipedia.org/wiki/Quicksort
                 using realloc/malloc and pointer to pointer array
Usage          : domqsort unsorted_file startpos Y (optional flag to delete duplicates)
Output         : input filename.srt
-------------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define RECLEN 2000   /* Longest input record - may need to increase */

FILE *infil;
FILE *outfil;
char *input;
char *output;
char *hold1;
char *p1;
char *p2;
char **initlist;
char **initrealloclist;
char **list;
char **realloclist;
char buf[RECLEN];
char dups[1];
char startpos[5];
char arraycounter[10];
char initrec[RECLEN];
int startposint;
int i;
time_t tim;

void quicksort(char **list,long lower,long upper);
int partition(char **list,long lower,long upper);
int exchange(char **list,long a,long b);
int readin(void);

int main(int argc,char *argv[])
{
long x,arrcnt,writecnt;
double ramused;
ramused = 0;

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

if (argc > 2)
{
		strcpy(startpos,argv[2]); /* Sort starts at this position */
}
else
{
	strcpy(startpos,"0");
}

if (argc > 3)
{
		strcpy(dups,argv[3]); /*If set to Y, duplicate records should be discarded */
}
else
{
	*dups = 'N';
}

output = argv[1];
strcat(output,".srt");
if((outfil = fopen(output,"w")) == NULL)
{
	printf("Error on creating file %s\n",output);
	exit(1);
}

arrcnt=0;
writecnt=0;
tim = time(NULL);
printf("\ndomqsort starts at %s",ctime(&tim));
printf("Suppress duplicates flag = %c\n",*dups);
readin();

startposint = atoi(startpos);

while(feof(infil) == 0)
{
	realloclist = realloc(list, (arrcnt+1) * sizeof(char *));  /* Allocates memory for a new pointer in list (one ahead) */
  if (realloclist != NULL)                                   /* Use expendable pointer to pointer in case of failure although in this usage it doesn't really matter */
  {
  	list = realloclist;                                      /* Set list to newly realloc'd space */
  	p1 = buf;
  	if (startposint > 0)                                     /* If there is a sort start pos */
  	{
  		sprintf(arraycounter,"__%d",arrcnt);
/*  		printf("arraycounter = %s\n",arraycounter); */
  		p2 = buf;
  		p2 += strlen(buf) -1;                                  /* Find newline char  */
  		p2[0] = '\0';                                          /* Replace newline char with null*/
  		strcat(buf,arraycounter);                              /* Append initial array pos identifier  */
  		strcat(buf,"\n");
  		p1 = buf;
  		p1 += startposint;                                     /* Offset pointer to start pos   */

  		/* Store the first part of each record */
  		initrealloclist = realloc(initlist, (arrcnt+1) * sizeof(char *));
  		if (initrealloclist == NULL)
  		{
  			printf("initrealloclist failed\n");
  		}
  		initlist = initrealloclist;
  		p2 = buf;
			for (i = 0; i < RECLEN; i++)
			{
				initrec[i] = '\0';
			}
  		strncpy(initrec,p2,startposint);
      initlist[arrcnt] = malloc(strlen(initrec)+1);
  		strcpy(initlist[arrcnt], initrec);
   	}
  	list[arrcnt] = malloc(strlen(p1)+1);                     /* Allocates memory for what the new pointer points to (plus 1 for newline) */
  	strcpy(list[arrcnt], p1);
		ramused += strlen(list[arrcnt]);                         /* Harmless fun, seeing how expensive this is on RAM */
		arrcnt++;
		readin();
	}
	else
	{
		printf("Error allocating memory, exiting\n");            /* Bomb out on memory alloc error which is why the realloclist variable is just good practice rather than strictly necessary */
		exit (1);
	}
}

printf("Sorting %d records\n",arrcnt);

quicksort(list,0,arrcnt-1);

fclose(infil);

/* find the record from the sorted array in the initlist[arrcnt] using the array index at the end of the record, remove this and write both in sync  */

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
	if(x>0)
	{
		free(list[x-1]);                 /* Again, like the memory testing above, not really required in this case, only included for completeness  */
	}
}
fclose(outfil);
printf("Records Written %ld to file %s\n",writecnt,output);
printf("RAM Used %0.2lf MB\n",ramused/1024/1024);
tim = time(NULL);
printf("domqsort ends at %s\n",ctime(&tim));
return EXIT_SUCCESS;
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
