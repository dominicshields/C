/**************************************************************************
PROGRAM NAME : genericstarter.c
DJS 06/12/2019 13:27:04
A generic starter program that
opens a named file in param 1
opens an output file of a literal name
accepts a second param which in this instance is converted to an int
reads the named input file
has a placeholder for processing
writes output records
clears output buffer
closes files
**************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define MAXLINE 512		/* check maximum length of input records */
#define OUTFILENAME "someoutput.txt"

FILE *infil_ptr;		  /* input file pointer */
FILE *outfil_ptr;     /* output file pointer */

char *filename;
char *outfilename;
char buf[MAXLINE];
char bufout[MAXLINE];
time_t tim;
int i, cnt, linecnt, fields;

int openoutput(void);
int openinput(void);
int process(void);
int read(void);
int clearbuf(void);
int tidyup(void);

int main(int argc,char *argv[])
{
	if (argc < 3)
	{
		fprintf(stderr,"Error, this program requires 2 parameters\n");
		exit(1);
	}

	filename=argv[1];
	fields=atoi(argv[2]);  /* converts to an integer */
	cnt = 0;
	openinput();
	openoutput();
	process();
	tidyup();

	tim = time(NULL);
	fprintf(stdout,"%d records written %s\n",cnt,ctime(&tim));
	return(0);
}

process()
{
	read();
	while (feof(infil_ptr) == 0)
	{
				/* DO SOME PROCESSING */

		fprintf(outfil_ptr,"%s\n",bufout);
		cnt++;
		clearbuf();
		read();
	}
	return(0);
}

openinput()
{
	if ((infil_ptr = fopen(filename,"r")) == NULL)
	{
		fprintf(stdout,"Error opening %s\n",filename);
		exit(1);
	}
	linecnt = 0;
	return(0);
}

openoutput()
{
	if ((outfil_ptr = fopen(OUTFILENAME,"w")) == NULL)
	{
		fprintf(stdout,"Error creating %s\n",OUTFILENAME);
		exit(1);
	}
	return(0);
}

read()
{
	fgets(buf,MAXLINE,infil_ptr);
	linecnt++;
	return(0);
}

tidyup()
{
	fclose(infil_ptr);
	fclose(outfil_ptr);
	return(0);
}

clearbuf()
{
	for(i = 0;i < MAXLINE;i++)
	{
		bufout[i] = '\0';
	}
	return(0);
}