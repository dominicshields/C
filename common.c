/**************************************************************************
PROGRAM NAME :	common (source common.c)
PURPOSE      : 	Takes any file of Ingres batch take-on errors supplied by
		Common Software and re-formats it to look nice for ZMerge
		to stick it into Notes.
PARAMETERS   :	Input filename
OUTPUT       :	Notesinp.dat which goes into ZMerge.
COMMENTS     :	The format of the file from Commom Software is :
		inquiry code (letters):ruref:checkletter:Company name:
		SIC92:period:AO Name:Address Change/Comment marker(1, 2 or 5):
		5 sel.editing scores:Error text

CHANGE LOG
		PLEASE NOTE : THE ADDRESS CHANGE FIELD ACTUALLY IS
		1 = COMMENT PRESENT
		2 = NO COMMENT
		5 = TOUCHTONE
		I have changed the code below accordingly, I have left the
		fields misleadingly called add_err*	Dom 5/2/97

		COMPLETELY CHANGED 22/7/98
		Changed to work for any file output by common software.
		The format of the output file is
		ruref,period,inqcode,sic92,AO Name,comment,5 score fields,
		error text

		RELEASED AS VERSION 1 OF Common.c 31/7/98

		Added in 5 fields for Selective Editing.

	    Amended May 2002 JR
		To include a seperate Touchtone field (TDE), to hold Y or blank,
		between AO Name and Comment.

		Amended 04/09/2009 DJS
		New function checkscore added which is called after each score is assigned
		to replace numeric overflow (indicated by 15 asterisks) with a credible value.
		Output tested as identical to previous version of the code other than when the
		intended replacement of asterisks when necessary.

		Amended 03/02/2011 15:08 DJS
		New field to hold a single character key responder marker which is the 9th delimited field
		inquiry code (letters):
		ruref:
		checkletter:
		company name:
		SIC92:
		period:
		AO Name:
		TDE:
		KEY RESPONDER:
		Address Change/Comment marker(1, 2 or 5):
		editing score1:
		editing score2:
		editing score3:
		editing score4:
		editing score5:
		Error text

		Amended 19/07/2011 17:13 DJS
		New field to hold a three character Band Number which is the 10th delimited field
		inquiry code (letters):
		ruref:
		checkletter:
		company name:
		SIC92:
		period:
		AO Name:
		TDE:
		KEY RESPONDER:
		Band Number:         19/07/2011 17:14
		Address Change/Comment marker(1, 2 or 5):
		editing score1:
		editing score2:
		editing score3:
		editing score4:
		editing score5:
		Error text

WRITTEN BY   : Dominic
**************************************************************************/
#include <string.h>
#include <stdio.h>
#include <time.h>

#define LEN 5000

int exit(int);
void read1(void);
void reformat(void);
void checkscore(char *score);

       					/* file pointers */
FILE *in1not2;		/* file from common sw */
FILE *notes;		/* the merged file */

char *p1;
char *p2;
char *p3;
char *input1;
char *out1;

char buf1[LEN];		/* buffer area to read in file 1 */

char inq[4];
char ruref[12];
char period[7];
char sic92[6];
char coname[80];
char coname2[80];
char aoname[80];
char tde[2];         /*  Touchtone   */
char keyresp[2];     /*  Key Responder Mkr 03/02/2011 15:12   */
char bandno[4];      /*  Band Number 19/07/2011 17:13   */
char add_err[8];
char comment[2];	/* Selective editing fields */
char score1[16];
char score2[16];
char score3[16];
char score4[16];
char score5[16];
char error_text[LEN];
char error_arr[40][200];

long cnt1, cnt2;
time_t tim;
int reply, i, j, x, w, z, y, len, errlen, ptrctr;

int main(argc,argv)
int argc;
char *argv[];
{

	if(argc < 3)
	{
		fprintf(stdout,"Error, usage is common <input file> <output file>\n");
		exit(1);
	}

	tim = time(NULL);
	fprintf(stdout,"\nProgram Begins  %s\n",ctime(&tim));

	input1 = argv[1];
	out1 = argv[2];

	if ((in1not2 = fopen(input1,"r")) == NULL)
	{
		fprintf(stdout,"Error opening %s does it exist?\n",input1);
		exit(1);
	}

	if ((notes = fopen(out1,"w")) == NULL)
	{
		fprintf(stdout,"Error creating %s\n",out1);
		exit(1);
	}

	cnt1=cnt2=0;

	for(y=0;y<LEN;y++)
	{
		buf1[y] = '\0';
	}

  while (fgets(buf1, sizeof(buf1), in1not2) != NULL)
	{
	  cnt1++;
	  p1 = buf1;
	  reformat();
	
		fprintf(notes,"%s,%s,%s,%s,%s,%s,%s,%s,",ruref,period,inq,coname2,sic92,aoname,tde,keyresp);
/*		fprintf(notes,"%s,%s,%s,%s,%s,%s,",comment,score1,score2,score3,score4,score5); */
	  fprintf(notes,"%s,%s,%s,%s,%s,%s,%s,",bandno,comment,score1,score2,score3,score4,score5);
		fprintf(notes,"%s,\n",error_arr[0]);
		if(x>0)
		{
			for(w=1;w<(x-1);w++)
			{
				fprintf(notes,"%s\n",error_arr[w]);
			}
			fprintf(notes,"%s",error_arr[x]);

		}
		fprintf(notes,"###\n");
		cnt2++;
		for(y=0;y<LEN;y++)
	  {
			buf1[y] = '\0';
		}
	}
	
	fprintf(stdout,"End of file %s\n",input1);

	fclose(in1not2);
	fclose(notes);

	fprintf(stdout,"Records read from input file = %ld\n",cnt1);
	fprintf(stdout,"Records written              = %ld\n",cnt2);
	tim = time(NULL);
	fprintf(stdout,"Program Ends OK  %s\n",ctime(&tim));

	return(0);

}

/******************************** reformat ****************************/

void reformat()
{

	j=0;
	strncpy(inq,p1,3);		/* INQ CODE */
	inq[3]='\0';

	while(strncmp(p1,":",1) != 0)
	{
		p1++;
	}

	p1++;
	strncpy(ruref,p1,11);	/* RU REF */
	ruref[11]='\0';

	p1 += 14;

	while(strncmp(p1," ",1) == 0)
	{
		p1++;
	}

	len = strcspn(p1,":");

	strncpy(coname,p1,len);	/* COMPANY NAME */
	coname[len]='\0';

	for(z=0;z<len;z++)					/* THIS IS TO DEAL WITH */
	{							/* COMMAS IN THE COMPANY */
		if(coname[z] == ',')			/* NAME THROWING OUT */
		{						/* THE ZMERGE IMPORT */
			coname2[z] = ' ';
		}
		else
		{
			coname2[z] = coname[z];
		}

	}
	coname2[len]='\0';

	p1 += len;
	p1++;

	len = strcspn(p1,":");
	strncpy(sic92,p1,len);	/* SIC92 */
	sic92[len]='\0';

	p1 += len;
	p1++;

	strncpy(period,p1,6);	/* PERIOD */
	period[6]='\0';

	p1 += 4;
	if(strncmp(p1,"00",2) == 0)
	{
		period[4]='1';
		period[5]='2';
	}

	p1 += 3;

	len = strcspn(p1,":");
	strncpy(aoname,p1,len);	/* NAME OF AO */
	aoname[len]='\0';


	while(strncmp(p1,":",1) != 0)
	{
		p1++;
	}
	p1++;

	len = strcspn(p1,":");
	strncpy(tde,p1,len);        /*  TDE   */
    tde[len]='\0';
	p1 += len;
	p1++;

	/* Key Responder Marker 03/02/2011 15:13*/

	len = strcspn(p1,":");
	strncpy(keyresp,p1,len);        /*  Key Responder Marker   */
    keyresp[len]='\0';
	p1 += len;
	p1++;

	/* Band Number 19/07/2011 17:15*/

	len = strcspn(p1,":");
	strncpy(bandno,p1,len);         /* Band Number   */
    bandno[len]='\0';
	p1 += len;
	p1++; 

	len = strcspn(p1,":");
	strncpy(comment,p1,len);	/* Comment flag */
	comment[len]='\0';
	p1 += len;
	p1++;

	len = strcspn(p1,":");
	strncpy(score1,p1,len);		/* Score 1 */
	score1[len]='\0';

  p3 = score1;
  checkscore("Score1");

	p1 += len;
	p1++;


	len = strcspn(p1,":");
	strncpy(score2,p1,len);		/* Score 2 */
	score2[len]='\0';

	p3 = score2;
  checkscore("Score2");

	p1 += len;
	p1++;


	len = strcspn(p1,":");
	strncpy(score3,p1,len);		/* Score 3 */
	score3[len]='\0';

	p3 = score3;
  checkscore("Score3");

	p1 += len;
	p1++;


	len = strcspn(p1,":");
	strncpy(score4,p1,len);		/* Score 4 */
	score4[len]='\0';

	p3 = score4;
  checkscore("Score4");

	p1 += len;
	p1++;


	len = strcspn(p1,":");
	strncpy(score5,p1,len);		/* Score 5 */
	score5[len]='\0';

	p3 = score5;
  checkscore("Score5");

	p1 += len;
	p1++;

	p2 = strstr(p1,"   ");
	if (p2 != NULL)
	{
		p2[0] = '\n';
	}
	len = strcspn(p1,"\n");

	for(y=0;y<LEN;y++)
	{
		error_text[y] = '\0';
	}

	strncpy(error_text,p1,len);
	error_text[len]='\0';
	p1 = error_text;
	errlen = strlen(error_text);
	ptrctr = 0;

	for(z=0;z<errlen;z++)					/* THIS IS TO DEAL WITH */
	{							/* COMMAS IN THE ERRORS */
		if(error_text[z] == ',')				/* THROWING OUT */
		{						/* THE ZMERGE IMPORT */
			error_text[z] = ' ';
		}
	}

	for(x=0;x<40;x++)
	{
			for(w=0;w<200;w++)
			{
				error_arr[x][w]='\0';
			}
	}

	x=0;
	len = strcspn(p1,"/");

	while(ptrctr < errlen)
	{
		if (len > 199)
		{
			fprintf(stdout,"Sub out of range, %s,%s,%s\n",len,w,x);
		}
		if(len == errlen && x == 0)
		{
			strcpy(error_arr[x],p1);
		}
		else
		{
			if(len == 0 && x > 0)
			{
				ptrctr = errlen;
			}
			else
			{
				strncpy(error_arr[x],p1,len);
			}
		}

		ptrctr = ptrctr + len;
		p1 = p1 + len;
		p1++;
		x++;
		len = strcspn(p1,"/");
	}

}


/***************************   read1  ******************************/

void read1()
{

	for(y=0;y<LEN;y++)
	{
		buf1[y] = '\0';
	}

	fgets(buf1,LEN,in1not2);
	if (feof(in1not2) != 0)
	{
		fprintf(stdout,"End of file %s\n",input1);
	}

	cnt1++;
	p1 = buf1;

	reformat();
}

/* Amendment to intercept numeric overflow in the scores DJS 04/09/2009 */
void checkscore(score)
char *score;
{

	if( strcmp(p3,"***************") == 0 )
	{
		fprintf(stdout,"Correcting numeric overflow on %s for ref %s\n",score,ruref);
		strcpy(p3,"     999.000000");     /* replace for a reasonable value */
	}
}
