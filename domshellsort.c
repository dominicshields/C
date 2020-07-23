/*-------------------------------------------------------------------------------
Program Name   : domshellsort.c
Program Author : Dominic Shields
Date           : 26/07/2016 11:29:27
Purpose        : Implementation of ShellSort https://en.wikipedia.org/wiki/Shellsort
                 using realloc/malloc and pointer to pointer array
Usage          : domshellsort unsorted_file Y (optional flag to delete duplicates)
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
char **list;
char **realloclist;
char buf[RECLEN];
char dups[1];
time_t tim;

void shell_sort (char **list, long upper);
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

if (argc == 3)
{
		strcpy(dups,argv[2]); /*If set to Y, duplicate records should be discarded */
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
printf("\ndomshellsort starts at %s",ctime(&tim));
printf("Suppress duplicates flag = %c\n",*dups);
readin();
while(feof(infil) == 0)
{
	realloclist = realloc(list, (arrcnt+1) * sizeof(char *));  /* Allocates memory for a new pointer in list (one ahead) */
  if (realloclist != NULL)                                   /* Use expendable pointer to pointer in case of failure although in this usage it doesn't really matter */
  {
  	list = realloclist;                                      /* Set list to newly realloc'd space */
  	list[arrcnt] = malloc(strlen(buf)+1);                    /* Allocates memory for what the new pointer points to (plus 1 for newline) */
  	strcpy(list[arrcnt], buf);
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

/*shell_sort(list,arrcnt-1); */

shell_sort(list,arrcnt); 

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
	if(x>0)
	{
		free(list[x-1]);                 /* Again, like the memory testing above, not really required in this case, only included for completeness  */
	}
}
fclose(outfil);
printf("Records Written %ld to file %s\n",writecnt,output);
printf("RAM Used %0.2lf MB\n",ramused/1024/1024);
tim = time(NULL);
printf("domshellsort ends at %s\n",ctime(&tim));
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

void shell_sort (char **list, long upper) {
    long h, i, j ;
    for (h = upper; h /= 2;) {
        for (i = h; i < upper; i++) {
            hold1 = list[i];
            for (j = i; j >= h && strcmp(hold1,list[j - h]) < 0; j -= h) {
                list[j] = list[j - h];
            }
            list[j] = hold1;
        }
    }
}
