/**************************************************************************
PROGRAM NAME : pdfreformat.c
DJS 02/12/2019 15:19:28
A Copy and paste from a pdf file gives text output where each field is on a new line
This code accepts the filename and number of fields per record as params
and outputs the concatenated fields as a record.
**************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define MAXLINE 512		/* check maximum length of input records */
#define OUTFILENAME "pdfreformat.csv"

FILE *infil_ptr;		/* read DIR file */
FILE *outfil_ptr;

char *filename;
char *outfilename;
char buf[MAXLINE];
char bufout[MAXLINE];
time_t tim;
int cnt, linecnt, i, fields;

int openoutput(void);
int openinp(void);
int process(void);
int read(void);
int clearbuf(void);
int tidyup(void);

main(argc,argv)
int argc;
char *argv[];
{
	if (argc < 3)
	{
		fprintf(stdout,"Error, you must input filename of the PDF and the number of fields per record\n");
		exit(1);
	}
	filename=argv[1];
	fields=atoi(argv[2]);
	cnt = 0;

	openinp();
	openoutput();
	process();

	tim = time(NULL);
	fprintf(stdout,"%d records written %s\n",cnt,ctime(&tim));
	return(0);
}


process()
{
	read();
	while (feof(infil_ptr) == 0)
	{
		strncat(bufout,buf,strlen(buf)-1);    /* miss off the newline */
		strcat(bufout,",");

		if (linecnt == fields)
		{
					fprintf(outfil_ptr,"%s\n",bufout);
					cnt++;
					clearbuf();
					linecnt = 0;
		}

		read();
	}

	return(0);
}

openinp()
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