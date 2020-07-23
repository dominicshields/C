/**************************************************************************
PROGRAM NAME : appendinqperiod (source appendinqperiod.c)
PURPOSE      : decompresses all files in the directory it is placed in
               then reads the directory and creates a file of all the filenames
               it finds. Reads this file and excluding any files with an .exe or
               compressed extension assumes all other files are files with the
               naming convention gainloss1_001_200403
               where 1 is phase and can be 1 or 2
               001 is inquiry code
               200403 is YYYYMM. It writes all records encountered to one file
               detailed below. Finally it tidies up by copying all decompressed
               gainloss files to a backup directory and deleting all compressed
               and decompressed gainloss files.
PARAMETERS   : None at present
OUTPUT       : The program creates three output files inqcode_period_gainloss.txt
               which contains all the records from all the files it has read except
               for the inquiry codes defined in the constant ADMININQ suffixed with 
               inqcode,period.
               A separate file for inquiry codes defined in the constant ADMININQ 
               called admin_period_gainloss.txt
               And a separate file critsic_period_gainloss.txt inquiry codes defined 
               in the constant CRITICALINQ.
               These get overwritten each time the program runs.
COMMENTS     : Makes external system call to GZIP http://www.gzip.org/ which
               needs to be in the same directory and does the uncompression of
               the compressed .Z unix format.
               Assumes that this all takes place in a dedicated directory.
AUTHOR       : Dominic Shields Feb 2004
VERSION      : V1.5 27/04/2010 14:55 - Change to remove Admin file and redefine Critical file
	             V1.4 07/10/2005 14:11 - New output file for critical SICs
	             V1.3 27-05-2004 - Remove empty output files.
	             V1.2 26-05-2004 - Added code to deal with a new range of
               inquiry codes to be dealt with differently.
               833:834:842:871:872:873:884:885:888:889
               V1.1 06-05-2004 - Added code to deal with inqcode 861 differently
               by writing this to a different file.
**************************************************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dos.h>
#include <dir.h>
#include <time.h>

#define MAXLINE 512		/* check maximum length of input records */
/*#define ADMININQ "833:834:842:861:871:872:873:884:885:888:889"   Inquiry codes of exception inquiries  */
/*#define CRITICALINQ "827:860"   Inquiry codes of critical sics 07/10/2005 14:16 */ 
#define CRITICALINQ "827:833:860:862:872"  /* DEFINITIONS CHANGED, THOSE ABOVE SUPERSEDED 27/04/2010 14:53 DJS*/

FILE *inp;			/* printing input file */
FILE *outp;			/* printing output file */
FILE *infil_ptr;		/* read DIR file */
FILE *gainloss_ptr;		/* read gainloss file */
FILE *outfil_ptr;
/*FILE *adminfil_ptr;*/
FILE *criticalfil_ptr;

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
/* char *adminfilename;   27/04/2010 15:00 */
char *criticalfilename;
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

main(argc,argv)
int argc;
char *argv[];
{
	filename=argv[argc-1];
	cnt = 0;
/*	cntadmin = 0;  27/04/2010 15:00 */
	cntcritsic = 0;
	filecnt = 0;

	decompress();
	dirlook();
	openoutput();
	processdir();
	tidyup();

	tim = time(NULL);
	fprintf(stdout,"Version 1.5 : %d files read\n%d records written to %s\n%d records written to %s\n%s\n",filecnt,cnt,outfilename,cntcritsic,criticalfilename,ctime(&tim));
	return(0);
}

decompress()
{
/* Call freeware compress/decompress utility gzip
For Unix syle gzipped files, the extension is .z */
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
	sprintf(syscall,"DIR gainloss*.* /A:-D /B /L > c:\\@@@@@@@@.@@@");
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
/* LAYOUT IS gainloss1_001_200403 */
			len = strlen(filenamebuf);
			p1 += (len - 13);
			strncpy(phase,p1,1);
			phase[1] = '\0';
			p1 += 2;
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
	readgl();
	while (feof(gainloss_ptr) == 0)
	{
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
		strncat(p2,":",1);
		p2++;
		strcat(p2,phase);

/*		retval = strstr(ADMININQ,inqcode);   27/04/2010 14:57
 		if(retval != NULL)
		{
			fprintf(adminfil_ptr,"%s\n",p4);
			cntadmin++;
		}
			else
			{ */
		retval = strstr(CRITICALINQ,inqcode);  /* NEW TEST 07/10/2005 14:17 DJS */
 	 	if(retval != NULL)
	 	{
	    fprintf(criticalfil_ptr,"%s\n",p4);
	    cntcritsic++;
	 	}
	 	else
		{
			fprintf(outfil_ptr,"%s\n",p4);
			cnt++;
		}
	/*		}   27/04/2010 14:57 */

		readgl();
	}

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

/*	adminfilename = "admin_period_gainloss.txt";   27/04/2010 14:58
	if ((adminfil_ptr = fopen(adminfilename,"w")) == NULL)
	{
		fprintf(stdout,"Error creating %s\n",adminfilename);
		exit(1);
	} */
	
	criticalfilename = "critsic_period_gainloss.txt";    /* added 07/10/2005 14:13 DJS */
	if ((criticalfil_ptr = fopen(criticalfilename,"w")) == NULL)
	{
		fprintf(stdout,"Error creating %s\n",criticalfilename);
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
/*fclose(adminfil_ptr);    27/04/2010 14:59 */
	fclose(criticalfil_ptr);

	/* remove("c:\\@@@@@@@@.@@@"); commented out 06/07/2005 - Lucia*/
	strcpy(syscall,"");
	sprintf(syscall,"copy gainloss* backup");
	system(syscall);
	strcpy(syscall,"");
	sprintf(syscall,"del gainloss*");
	system(syscall);
/* ADDED 27-05-2004 - REMOVE EMPTY OUTPUT FILES */	
/*	if(cntadmin == 0)   27/04/2010 14:59
	{
		remove(adminfilename);
	}    */
	if(cnt == 0)
	{
		remove(outfilename);
	}
	if(cntcritsic == 0)
	{
		remove(criticalfilename);
	}

	return(0);
}