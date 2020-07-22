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
char hold[RECLEN];
char *v;
char *listcmp;
char *hold1;
char *hold2;
char **list;
char **extlist;
long a,b,i,j,upper,lower,arrcnt,t;
long qsortcalls, partitioncalls, exchangecalls, comparisons;
time_t tim;

void quicksort(char ***list, int lower,int upper);
int partition(char *list [],int lower,int upper);
int exchange(char *list [],int a,int b);
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

	tim = time(NULL);
	printf("\nQsort starts at %s",ctime(&tim));
	
while(feof(infil) == 0)
{
/*
    //Expand array to allow one additional item in the array
    myArray = (char **)realloc(myArray,++arrayIndexs * sizeof(char*));

    //Allocate memory for the new string item in the array
    myArray[arrayIndexs] = malloc(254 * sizeof(char*));

    //Populate a new value in the array
    strcpy(myArray[arrayIndexs], "string 3"); //
*/	

/*	list = (char **) realloc (list, (arrcnt+1) * sizeof(char *)); 
	list[arrcnt+1] = malloc(1000 * sizeof(char*));  */
	list = realloc(list, 50 * sizeof(char *)); 
	strcpy(hold,p1);
  list[arrcnt+1] = hold; 
/*  strcpy(list[arrcnt+1],buf); */
  printf("list[%d] = %s",arrcnt+1,list[arrcnt+1]); 
	arrcnt++;	
	readin();  
}
printf("Test loop\n");
 	for (t = 1; t < 5;t++)
 	{
 		printf("list[%d] = %s",t,list[t]);
 	}

printf("Sorting %d records\n",arrcnt);

quicksort(&list,1,arrcnt);

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

void quicksort(char ***list,int lower ,int upper)
{
 	int c;
  printf("1.In quicksort\n");
 	
 	for (c = 1; c < 5;c++)
 	{
 		printf("list[%d] = %s",c,(*list)[c]);
 	}
	qsortcalls++;
	if(upper > lower)
	{
		j = partition(*list,lower,upper);
		quicksort(list,lower,j - 1);	
		quicksort(list,j + 1,upper);
	}
}

int partition(char *list[],int lower,int upper)
{
 printf("2.In partition\n"); 
	partitioncalls++;
	i=lower;
	j=upper + 1;

	strcpy(v,list[lower]);
	printf("lower = %ld, v = %s",lower,v);
printf("2B.In partition\n"); 
	do
	{
		do
		{ 
			i = i+1;
			printf("i = %ld\n",i);
			listcmp = list[i];
			comparisons++;
		} while( strcmp(listcmp,v) < 0 );

		do
		{
			 j = j-1;
			 listcmp = list[j];
			 comparisons++;
		} while(strcmp(listcmp,v) > 0);

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

int exchange(char *list[],int a,int b)
{
  printf("3.In exchange\n"); 
	exchangecalls++;
	
	hold1 = list[a];
	hold2 = list[b];
/*	strcpy(hold1,list[a]); 
	strcpy(hold2,list[b]); */

/*	strcpy(list[a],hold2);
	strcpy(list[b],hold1); */
	list[a] = hold2;
	list[b] = hold1;

	return(0);
}
