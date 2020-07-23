/**************************************************************************
PROGRAM NAME : bresappendinqperiod (source bresappendinqperiod.c)
PURPOSE      : decompresses all files in the directory it is placed in
               then reads the directory and creates a file of all the filenames
               it finds. Reads this file and excluding any files with an .exe or
               compressed extension assumes all other files are files with the
               naming convention Somename001_200403
PARAMETERS   : None at present
OUTPUT       : The program creates
               These get overwritten each time the program runs.
COMMENTS     : Makes external system call to GZIP http://www.gzip.org/ which
               needs to be in the same directory and does the uncompression of
               the compressed .Z unix format.
               Assumes that this all takes place in a dedicated directory.
AUTHOR       : Dominic Shields 09/12/2015 17:43:47
VERSION      :
sample filename  rutrays242_201512
**************************************************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dos.h>
#include <dir.h>
#include <time.h>

#define MAXLINE 512		                               /* check maximum length of input records */
#define OUTPUTFILENAME "BRES_Update.csv"
#define DECOMPRESSCMD "gzip -ad *.z"                 /* Call freeware compress/decompress utility gzip For Unix syle gzipped files, the extension is .z */
#define DIRCMD "DIR rutrays*.* /A:-D /B /L > "
#define DUMMYFILE "d:\\@@@@@@@@.@@@"
#define WILDCARD "rutrays*"

FILE *inp;			/* printing input file */
FILE *outp;			/* printing output file */
FILE *infil_ptr;		/* read DIR file */
FILE *bres_ptr;		/* read bres file */
FILE *outfil_ptr;

char *input;
char *output;

char *filename;
char *p1;
char *p2;
char *p3;
char *p4;
char *s;
char *retval;
char buf[MAXLINE];
char bufin[MAXLINE];
char filenamebuf[81];
char arr[MAXLINE][80];
char syscall[MAXLINE];
char syscomm[MAXLINE];
char okprcall[MAXLINE];
char find[MAXLINE];
char drive[3];
char dir[MAXLINE];
char file[15];
char path[80];
char drive[3];
char ext[4];
char period[7];
char inqcode[4];
char phase[2];
char *outfilename;
time_t tim;

int flags, len, j, linelen, x, z, cnt, cntadmin, filecnt, cntcritsic;

int decompress(void);
int dirlook(void);
int openoutput(void);
int processdir(void);
int system(char*);
int opengl(void);
int processgl(void);
int readgl(void);
int tidyup(void);
int exit(int);
void removespaces(char*);
void colontocomma(char*);

main(argc,argv)
int argc;
char *argv[];
{
	filename=argv[argc-1];
	cnt = 0;
	filecnt = 0;

	decompress();
	dirlook();
	openoutput();
	processdir();
	tidyup();

	tim = time(NULL);
	fprintf(stdout,"Version 1 : %d files read\n%d records written to %s\n",filecnt,cnt,OUTPUTFILENAME,ctime(&tim));
	return(0);
}

decompress()
{
	sprintf(syscall,DECOMPRESSCMD);
	system(syscall);
	strcpy(syscall,"");
	return(0);
}

dirlook()
{
	s=filename;
	flags=fnsplit(s,drive,dir,file,ext);
	sprintf(path,"%s%s",drive,dir);
	printf("Drive %s\n",drive);
	printf("Dir %s\n",dir);
	sprintf(syscall,"%s%s",DIRCMD, DUMMYFILE);
	system(syscall);
	if ((infil_ptr = fopen(DUMMYFILE,"r")) == NULL)
	{
		fprintf(stdout,"Error opening %s\n",DUMMYFILE);
		exit(1);
	}
	strcpy(syscall,"");
	return(0);
}

processdir()
{
	fgets(filenamebuf,81,infil_ptr);
	while(feof(infil_ptr) == 0)
	{
		z=0;
		p3 = filenamebuf;
		p1 = filenamebuf;
		while(p3[0] != '.' && z<60)
		{
			p3++;
			z++;
		}
		p3++;
		if((strncmp(p3,"exe",3) != 0) && (strncmp(p3,"txt",3) != 0) && (strncmp(p3,"z",1) != 0) && (strncmp(p3,"csv",3) != 0))     /* Do not process files with these suffixes */ 
		{
/* filename layout rutrays242_201512  */
			len = strlen(filenamebuf);
			p1 += (len - 11);
			strncpy(inqcode,p1,3);
			inqcode[3] = '\0';
			p1 += 4;
			strncpy(period,p1,6);
			period[6] = '\0';
			opengl();
			processgl();
			fclose(bres_ptr);
		}
		for(j=0;j<80;j++)
		{
			filenamebuf[j]='\0';
		}
		fgets(filenamebuf,81,infil_ptr);
	}
	return(0);
}

processgl()
{
	readgl();
	while (feof(bres_ptr) == 0)
	{
		p2 = buf;
		p4 = p2;
		linelen = strlen(p2);
		linelen--;
		p2 = p2 + linelen;
		p2[0] = '\0';
		strncat(p2,",",1);
		strcat(p2,inqcode);
		p2 += 3;
		strncat(p2,",",1);
		p2++;
		strcat(p2,period);

		removespaces(p4);
		colontocomma(p4);

		fprintf(outfil_ptr,"%s\n",p4);
		cnt++;

		readgl();
	}

	return(0);
}

opengl()
{
	x = strlen(filenamebuf);
	filenamebuf[x-1] = '\0';
	if ((bres_ptr = fopen(filenamebuf,"r+")) == NULL)
	{
		fprintf(stdout,"Error opening %s\n",filenamebuf);
		exit(1);
	}
	filecnt++;
	return(0);
}

openoutput()
{
	if ((outfil_ptr = fopen(OUTPUTFILENAME,"w")) == NULL)
	{
		fprintf(stdout,"Error creating %s\n",OUTPUTFILENAME);
		exit(1);
	}

	return(0);
}

readgl()
{
	fgets(buf,MAXLINE,bres_ptr);
	return(0);
}

tidyup()
{
	fclose(infil_ptr);
	fclose(bres_ptr);
	fclose(outfil_ptr);

	strcpy(syscall,"");
	sprintf(syscall,"copy %s backup",WILDCARD);
	system(syscall);
	strcpy(syscall,"");
	sprintf(syscall,"del %s",WILDCARD);
	system(syscall);
/*	if(cnt == 0)
	{
		remove(OUTPUTFILENAME);
	} */
	return(0);
}
void removespaces(char* source)
{
  char* i = source;
  char* j = source;
  while(*j != 0)
  {
    *i = *j++;
    if(*i != ' ')
      i++;
  }
  *i = 0;
}
void colontocomma(char* source)
{
  char* i = source;
  char* j = source;
  while(*j != 0)
  {
    *i = *j++;
    if(*i == ':')
    	{
    		*i = ',';
    	}
      i++;
  }
  *i = 0;
}