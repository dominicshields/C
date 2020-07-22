/**************************************************************************
PROGRAM NAME : appendinqperiod (source appendinqperiod.c)
PURPOSE      : decompresses all files in the directory it is placed in
               then reads the directory and creates a file of all the filenames
               it finds. Reads this file and excluding any files with an .exe or
               compressed extension assumes all other files are files with the
               naming convention gainloss001_200403 where 001 is inquiry code
               and 200403 is YYYYMM. It writes all records encountered to one file
               detailed below. Finally it tidies up by copying all decompressed
               gainloss files to a backup directory and deleting all compressed
               and decompressed gainloss files.
PARAMETERS   : None at present
OUTPUT       : The program creates one output file inqcode_period_gainloss.txt
               which contains all the records from all the files it has read
               suffixed with inqcode,period. This gets overwritten each time the
               program runs.
COMMENTS     : Makes external system call to GZIP http://www.gzip.org/ which
               needs to be in the same directory and does the uncompression of
               the compressed .Z unix format.
               Assumes that this all takes place in a dedicated directory.
AUTHOR       : Dominic Shields Feb 2004               
**************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <dos.h>
#include <dir.h>
#include <time.h>

#define MAXLINE 256		/* check maximum length of input records */

FILE *inp;			/* printing input file */
FILE *outp;			/* printing output file */
FILE *infil_ptr;		/* read DIR file */
FILE *gainloss_ptr;		/* read gainloss file */
FILE *outfil_ptr;

char *input;
char *output;

char *filename;
char *p1;
char *p2;
char *p3;
char *p4;
char *s;
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
char *outfilename;
time_t tim;

int flags, len, j, linelen, x, z, cnt, filecnt;

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
	fprintf(stdout,"%d files read %d records written to %s\n%s\n",filecnt,cnt,outfilename,ctime(&tim));
	return(0);
}

decompress()
{
/* The wzunzip command requires that the Winzip command line tools are installed
and that it is in the PATH of the machine. For Unix syle gzipped files, the extension is .z */
	sprintf(syscall,"gzip -ad *.z");
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
	sprintf(syscall,"DIR *.* /A:-D /B /L > c:\\@@@@@@@@.@@@");
	system(syscall);
	if ((infil_ptr = fopen("c:\\@@@@@@@@.@@@","r")) == NULL)
	{
		fprintf(stdout,"Error opening c:\\@@@@@@@@.@@@\n");
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
		if((strncmp(p3,"exe",3) != 0) && (strncmp(p3,"txt",3) != 0)
		&& (strncmp(p3,"z",1) != 0))
		{
/* LAYOUT IS gainloss001_200403 */
			len = strlen(filenamebuf);
			p1 += (len - 11);
			strncpy(inqcode,p1,3);
			inqcode[3] = '\0';
			p1 += 4;
			strncpy(period,p1,6);
			period[6] = '\0';
			opengl();
			processgl();
			fclose(gainloss_ptr);
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
	do
	{
		readgl();
		p2 = buf;
		p4 = p2;
		linelen = strlen(p2);
		linelen--;
		p2 = p2 + linelen;
		p2[0] = '\0';
		strncat(p2,":",1);
		strcat(p2,inqcode);
		p2 += 3;
		strncat(p2,":",1);
		p2++;
		strcat(p2,period);
		p2 += 6;
		fprintf(outfil_ptr,"%s\n",p4);
		cnt++;
	} while (feof(gainloss_ptr) == 0);

	return(0);
}

opengl()
{
	x = strlen(filenamebuf);
	filenamebuf[x-1] = '\0';
	if ((gainloss_ptr = fopen(filenamebuf,"r+")) == NULL)
	{
		fprintf(stdout,"Error opening %s\n",filenamebuf);
		exit(1);
	}
	filecnt++;
	return(0);
}

openoutput()
{
	outfilename = "inqcode_period_gainloss.txt";
	if ((outfil_ptr = fopen(outfilename,"w")) == NULL)
	{
		fprintf(stdout,"Error creating %s\n",outfilename);
		exit(1);
	}
	return(0);
}

readgl()
{
	fgets(buf,MAXLINE,gainloss_ptr);
	return(0);
}

tidyup()
{
	fclose(infil_ptr);
	fclose(gainloss_ptr);
	fclose(outfil_ptr);

	remove("c:\\@@@@@@@@.@@@");
	strcpy(syscall,"");
	sprintf(syscall,"copy gainloss* backup");
	system(syscall);
	strcpy(syscall,"");
	sprintf(syscall,"del gainloss*");
	system(syscall);

	return(0);
}