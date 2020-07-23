/**************************************************************************
PROGRAM NAME : appendrandomnumber (source appendrandomnumber.c)
PURPOSE      : Append a pseudorandom number to each line of a file
PARAMETERS   : Filename
OUTPUT       : File with ".out" extension containing the numbers appended
AUTHOR       : Dominic Shields 16/12/2011 18:03
VERSION      : V1
NOTE         : RAND_MAX max value is defined in <stdlib.h> so is compiler-specific
**************************************************************************/

#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAXLINE 1024	   /* check maximum length of input records */
#define RAND_MAX 32767   /* range of random numbers is 1 to the number RAND_MAX is set to */

FILE *infil_ptr;
FILE *outfil_ptr;

char *filename;
char *p1;
char outfilename[100];
char buf[MAXLINE];
char num[10];
int readcnt, len;
time_t tim;

int process(void);
int openfiles(void);
long srand(long);
long rand(void);
int exit(int);

main(argc,argv)
int argc;
char *argv[];
{
	if(argc == 1)
	{
		fprintf(stdout,"Error, usage is %s <Filename>\n",argv[0]);
		exit(1);
	}
	filename=argv[1];
	strcpy(outfilename,filename);
	strcat(outfilename,".out");

	openfiles();
	process();

	fclose(infil_ptr);
	fclose(outfil_ptr);

	tim = time(NULL);
	fprintf(stdout,"Version 1.0 : %d records read\n%d records written to %s %s\n",readcnt,readcnt,outfilename,ctime(&tim));
	return(0);
}


process()
{
	srand((unsigned)time(NULL));   /* Seed the random number generator with current time */

	fgets(buf,MAXLINE,infil_ptr);
	readcnt = 0;

	while(feof(infil_ptr) == 0)
	{

		sprintf(num,"%d",rand()%RAND_MAX);  /* Return a random number between 1 and RAND_MAX into num */

		p1 = buf;
		len = strlen(buf);

		p1 += len;
		p1--;
		p1[0] = '\0';
		strcat(p1,num);

		fprintf(outfil_ptr,"%s\n",buf);

		fgets(buf,MAXLINE,infil_ptr);
		readcnt++;
	}
	return(0);
}

openfiles()
{
	if ((infil_ptr = fopen(filename,"r")) == NULL)
	{
		fprintf(stdout,"Error opening %s\n",filename);
		exit(1);
	}

	if ((outfil_ptr = fopen(outfilename,"w")) == NULL)
	{
		fprintf(stdout,"Error creating %s\n",outfilename);
		exit(1);
	}

	return(0);
}
