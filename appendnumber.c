/**************************************************************************
PROGRAM NAME : appendnumber (source appendnumber.c)
PURPOSE      : Append an incrementing number to each line of a file
PARAMETERS   : Filename
OUTPUT       : File with ".out" extension containing the numbers appended
AUTHOR       : Dominic Shields 16/12/2011 13:58
VERSION      : V1
**************************************************************************/

#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAXLINE 1024		/* check maximum length of input records */

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
int exit(int);

main(argc,argv)
int argc;
char *argv[];
{
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
	fgets(buf,MAXLINE,infil_ptr);
	readcnt = 1;

	while(feof(infil_ptr) == 0)
	{
		
		sprintf(num,"%d",readcnt);
		
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
