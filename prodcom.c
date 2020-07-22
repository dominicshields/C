/**************************************************************************
PROGRAM NAME : prodcom (source prodcom.c)
PURPOSE      :
PARAMETERS   : Input File Name
OUTPUT       : Reformatted output file
COMMENTS     :
AUTHOR       : Dominic Shields 29/11/2013 21:27:03
**************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <dos.h>
#include <dir.h>
#include <time.h>

#define MAXLINE 256 /* check maximum length of input records */

FILE *infil_ptr;
FILE *outfil_ptr;

char *input;
char *output;
char *p1;
char *p2;
char *p3;
char *p4;
char buf[MAXLINE];
char buf2[MAXLINE];
char ruref[12];
char ref[7];
char comment[5000];
char commentsqueeze[MAXLINE];
char timebuff[100];

long readcnt, writecnt, i;

int openinput(void);
int openoutput(void);
int processfile(void);
int readfile(void);
int writefile(void);
int exit(int);
int squeezespaces(char* , char);

main(argc,argv)
int argc;
char *argv[];
{
    time_t now = time (0);
    strftime (timebuff, 100, "%Y-%m-%d %H:%M:%S", localtime (&now));
    fprintf(stdout,"\n%s starts at %s\n",argv[0],timebuff);
	
	if(argc < 2)
	{
		fprintf(stderr,"Error, usage is prodcom <inputfile>\n");
		exit(1);
	}

	readcnt = 0;
	writecnt = 0;
	input = argv[1];            /* input filename assigned to pointer */
	openinput();
	openoutput();
	processfile();
	fclose(infil_ptr);
	fclose(outfil_ptr);
	strftime (timebuff, 100, "%Y-%m-%d %H:%M:%S", localtime (&now));
	fprintf(stdout,"%d Records Read, %d Records written to %s %s\n",readcnt,writecnt,output,timebuff);
	return(0);
}

/*
INPUT FILE LAYOUT (FIXED LENGTH)
49900000583	       279922	     1	toilet bags have now been more accurately coded therefore 174025901
49900000583	       279922	     2	is correctly not filled in.
49900000583	       279922	     3	deb
49900000583	       279923	     1	for decrease in question 365012301.  Mr Boswell confirmed this decrease is
49900000583	       279923	     2	due to this line being discontinnued.
49900000583	       279924	     1	Boswell but he was not in the office.  I left a message with his colleague
49900000583	       279924	     2	to say I am chasing up the return of the form for 9512 and for Mr Boswell to
49900000583	       279924	     3	phone if there are any problems.
49900000583	       279924	     4	JULIA COLES
49900000583	       279924	     5	&
49900000583	       279925	     1	with Mr.Boswell and also explained that I'd calculated the the volumes and
49900000583	       279925	     2	confirmed with him that there were no changes in any of the unit values.
49900000583	       279925	     3	Geraint Davies
*/
processfile()
{
	p4 = buf2;
	do
	{
		readfile();

		p2 = buf;
		if(strncmp(p2,p4,28) != 0)    /* Test for change of ruref and 6 digit ref  */
		{
				if(strlen(ruref) > 0)     /* Prevent first record writing */
				{
					writefile();
				}
				for (i = 0; i < 5000; i++)
				{
					comment[i] = '\0';
				}
				strncpy(ruref,p2,11);
				ruref[11] = '\0';
				p2 += 19;
				strncpy(ref,p2,6);
				ref[6] = '\0';
				p2 += 14;
				squeezespaces(p2,'\n');
				strcpy(comment,commentsqueeze);
		}
		else
		{
				p2 += 33;
				squeezespaces(p2,'\n');
				strcat(comment,commentsqueeze);
		}
		strcpy(buf2,buf);
		p4 = buf2;
	} while (feof(infil_ptr) == 0);

	writefile();  /* flush last record */
	return(0);
}

openinput()
{
   if ((infil_ptr = fopen(input,"r")) == NULL)
   {
      fprintf(stderr,"Error opening %s does it exist?\n",input);
      exit(1);
   }
	return(0);
}

openoutput()
{
	output = "prodcomout.txt";
	if ((outfil_ptr = fopen(output,"w")) == NULL)
	{
		fprintf(stdout,"Error creating %s\n",output);
		exit(1);
	}
	return(0);
}

readfile()
{
	fgets(buf,MAXLINE,infil_ptr);
	readcnt++;
	return(0);
}

writefile()
{
	fprintf(outfil_ptr,"%s~%s~%s\n",ruref,ref,comment);
	writecnt++;
	return(0);
}

squeezespaces(char* row, char separator)
{
	for (i = 0; i < MAXLINE; i++)
	{
		commentsqueeze[i] = '\0';
	}

  for(p1 = row; p1[0]!=separator; p1++)
  {
    if(strncmp(p1,"  ",2) != 0 && strncmp(p1,"\r",1) != 0 && strncmp(p1,"\n",1) != 0)
    {
			strncat(commentsqueeze,p1,1);
  	}
  }
	return(0);
}