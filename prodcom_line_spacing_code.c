/**************************************************************************
PROGRAM NAME : prodcom (source prodcom.c)
PURPOSE      : To produce output that can be imported into the new Prodcom Contributor Comments
PARAMETERS   : Input File Name
OUTPUT       : Reformatted output files (4 at present)
COMMENTS     : This takes the Ingres "Notepad" file supplied by Dom Ford and reformats it into up to 4 files depending on the periods.
							It assumes that any colons in the input have been replaced by semi-colons and that any trailing spaces have been trimmed
							by using the function Format <Trim Training Spaces> in Ultraedit.
							There is rather odd-looking code which tries to format the individual comments in a more readable way, the reason for the
							kludgy space padding is that newlines have no effect in the target field and the even more kludgy calculations about how
							many spaces to pad with are to do with how Notes treats variable width fonts.
							Some of the code isn't very elegant but the plan is that it is only used for real once, plus I always bear in mind the
							advice of ace Ingres Consultant Neil Pitman - "A C program that produces correct output is an efficient C program".
AUTHOR       : Dominic Shields 25/11/2014 14:26:24
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <dos.h>
#include <dir.h>
#include <time.h>

#define MAXLINE 256    /* check maximum length of input records */
#define MAXRECLEN 160  /* Comment line max length */
#define PADDING 140    /* Comment line padding */

FILE *infil_ptr;
FILE *outfil_ptr2011;
FILE *outfil_ptr2012;
FILE *outfil_ptr2013;
FILE *outfil_ptr2014;

char *input;
char *output2011;
char *output2012;
char *output2013;
char *output2014;
char *p1;
char *p2;
char *p3;
char *p4;
char buf[MAXLINE];
char buf2[MAXLINE];
char ruref[12];     /* Byte 1 to 11  */
char period[7];     /* Byte 20 to 26 */
char datetime[11];  /* Byte 41 to 51 Date only after reflection */
char title[41];     /* Byte 67 to 107 */
char comment[5000];  /* Starts at byte 115  */
char spacer[500];     /* pad out each comment in the manner of FI CC */
char commentsqueeze[MAXLINE];
char timebuff[100];

long readcnt, i, writecnt_2011, writecnt_2012, writecnt_2013, writecnt_2014, x;
long reclen;

int remainder;
int padout;
int openinput(void);
int openoutput(void);
int processfile(void);
int readfile(void);
int writefile(void);
int squeezespaces(char* , char);
time_t tim;

main(argc,argv)
int argc;
char *argv[];
{
   /*time_t now = time (0); */
    tim = time(NULL);
    strftime (timebuff, 100, "%Y-%m-%d %H:%M:%S", localtime (&tim));
    fprintf(stdout,"%s starts at %s\n",argv[0],timebuff);

	if(argc < 2)
	{
		fprintf(stderr,"Error, usage is prodcom <inputfile>\n");
		exit(1);
	}

	readcnt = 0;
	writecnt_2011 = writecnt_2012 = writecnt_2013, writecnt_2014 = 0;
	input = argv[1];            /* input filename assigned to pointer */
	openinput();
	openoutput();
	processfile();
	fclose(infil_ptr);
	fclose(outfil_ptr2011);
	fclose(outfil_ptr2012);
	fclose(outfil_ptr2013);
	fclose(outfil_ptr2014);

  tim = time(NULL);
	strftime (timebuff, 100, "%Y-%m-%d %H:%M:%S", localtime (&tim));
	fprintf(stdout,"%d Records Read\n%d 2011 Records written\n%d 2012 Records written\n%d 2013 Records written\n%d 2014 Records written\nEnds %s\n",readcnt,writecnt_2011,writecnt_2012,writecnt_2013,writecnt_2014,timebuff);
	return(0);
}

/*
INPUT FILE LAYOUT (FIXED LENGTH AND DELIMITED BY ^
1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
49900000118^       201200^           13^13/03/2013 13:02:59      ^201200 DATA VALIDATION                  ^     1^Proposal to reclass to SIC24530.  Contributor was always in this SIC, until
49900000118^       201200^           13^13/03/2013 13:02:59      ^201200 DATA VALIDATION                  ^     2^BRES reclassed to SIC29320 on the basis of "Drive shafts for motor vehicles"
49900000118^       201200^           13^13/03/2013 13:02:59      ^201200 DATA VALIDATION                  ^     4^I can see little point in reclassing back to SIC24530 as contributor is
49900000118^       201200^           13^13/03/2013 13:02:59      ^201200 DATA VALIDATION                  ^     5^unlikely to argue with this on the next BRES form.
49900000118^       201200^           13^13/03/2013 13:02:59      ^201200 DATA VALIDATION                  ^     6^Reclass rejected & form cleared.
49900000118^       201200^           13^13/03/2013 13:02:59      ^201200 DATA VALIDATION                  ^     7^Alan Hughes
*/
processfile()
{
	p4 = buf2;
	do
	{
		readfile();

		p2 = buf;
		if(strncmp(p2,p4,40) != 0)    /* Test for change of ruref and comment tuple-id ref  */
		{
				if(strlen(ruref) > 0)     /* Prevent first record writing */
				{
					writefile();
				}
				for (i = 0; i < 5000; i++)
				{
					comment[i] = '\0';
				}

/*
char ruref[12];      Byte 1 to 11
char period[7];      Byte 20 to 26
char datetime[11];   Byte 41 to 51
char title[41];      Byte 67 to 107
char comment[5000];  Starts at byte 114
*/
				strncpy(ruref,p2,11);
				ruref[11] = '\0';
				p2 += 19;
				strncpy(period,p2,6);
				period[6] = '\0';
				p2 += 21;
				strncpy(datetime,p2,10);
				datetime[11] = '\0';
				p2 += 26;
				strncpy(title,p2,40);
				title[41] = '\0';
				p2 += 48;
				squeezespaces(p2,'\n');
				strcpy(comment,commentsqueeze);
		}
		else
		{
				p2 += 114;
				squeezespaces(p2,'\n');
				strcat(comment," ");
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
	output2011 = "prodcomout_2011.txt";
	output2012 = "prodcomout_2012.txt";
	output2013 = "prodcomout_2013.txt";
	output2014 = "prodcomout_2014.txt";

	if ((outfil_ptr2011 = fopen(output2011,"w")) == NULL)
	{
		fprintf(stdout,"Error creating %s\n",output2011);
		exit(1);
	}
	if ((outfil_ptr2012 = fopen(output2012,"w")) == NULL)
	{
		fprintf(stdout,"Error creating %s\n",output2012);
		exit(1);
	}
	if ((outfil_ptr2013 = fopen(output2013,"w")) == NULL)
	{
		fprintf(stdout,"Error creating %s\n",output2013);
		exit(1);
	}
	if ((outfil_ptr2014 = fopen(output2014,"w")) == NULL)
	{
		fprintf(stdout,"Error creating %s\n",output2014);
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
	squeezespaces(title,'\0');
	strcpy(title,commentsqueeze);

	reclen = 0;
	reclen = strlen(period) + strlen(title) + strlen(comment) + strlen(datetime);
	remainder = reclen % MAXRECLEN;
	padout = MAXRECLEN - remainder + PADDING;
	strcpy(spacer,"");
	for (x = 0; x < padout-1; x++)
	{
		strcat(spacer," ");
	}
	strcat(spacer,"_");
	spacer[padout] = '\0';

	switch (atol(period))
	{
	case 201100 :
			fprintf(outfil_ptr2011,"%s^%s : %s %s : %s%s\n",ruref,period,title,comment,datetime,spacer);
			writecnt_2011++;
			break;
	case 201200 :
			fprintf(outfil_ptr2012,"%s^%s : %s %s : %s%s\n",ruref,period,title,comment,datetime,spacer);
			writecnt_2012++;
			break;
	case 201300 :
			fprintf(outfil_ptr2013,"%s^%s : %s %s : %s%s\n",ruref,period,title,comment,datetime,spacer);
			writecnt_2013++;
			break;
	case 201400 :
			fprintf(outfil_ptr2014,"%s^%s : %s %s : %s%s\n",ruref,period,title,comment,datetime,spacer);
			writecnt_2014++;
			break;
	default :
			fprintf(stdout,"Error with period %s\n",period);
	}

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