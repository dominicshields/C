/**************************************************************************
PROGRAM NAME :	commonnes (source commonnes.c)
PURPOSE      : 	Takes the NES file of Ingres batch take-on errors supplied by
		Common Software and re-formats it to look nice for ZMerge
		to stick it into Notes.
PARAMETERS   :	Input filename
OUTPUT       :	INQUIRYinp.dat which goes into ZMerge.
COMMENTS     :	The format of the file from Common Software is :
		inquiry code (letters): serial number(=ruref):
		checkletter: employer ref(=SIC92): period: AO Name:
                Error text
         How 'commonnes' differs from 'common':-
                	No processing of comment field (not in file);
                	Length of ruref,SIC92 and period fields
                NES is the only exceptional inquiry in Notes take-on


CHANGE LOG
    DJS 28/06/2012 17:54
    A diagnostic print statement at line 271 was filling up the log for no reason
    Commented it out.
    ****************************
    
		25/04/03 Sylvia
		Amended to now include processing of 5 scores for selective editing
		******

		Amended to include extra field (nino, group, score, comm_marker and sano)
		and changing the names of ruref and sic92 to serial_no and paye. JR 25/5/2002

		Amended to include an extra field,team, in the pck record. JR 14/3/2002

		COMPLETELY CHANGED 22/7/98
		Changed to work for any file output by common software.
		The format of the output file is
		ruref,period,inqcode,sic92,AO Name,
		error text,clearance marker (always N)

		RELEASED AS VERSION 1 OF Common.c 31/7/98

WRITTEN BY   : Dominic
**************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LEN 2000
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
char serial_no[8];
char period[5];
char paye[14];
char coname[80];
char coname2[80];
char aoname[80];
char team[2];
char nino[10];
char group[2];
char score1[17];		/* Scores for selective editing */
char score2[17];
char score3[17];
char score4[17];
char score5[17];
char comm_marker[2];
char sano[4];
char error_text[LEN];
char error_arr[30][200];

long cnt1, cnt2;
time_t tim;
int reply, i, j, x, w, z, y, len, errlen, ptrctr, a;

int read1(void);      /* Function prototypes DJS 04/11/2008 */
int reformat(void);
int findcol(void);

main(argc,argv)
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

	read1();

	while ( feof(in1not2) == 0 )
	{


		fprintf(notes,"%s,%s,%s,%s,%s,%s,%s,%s,%s,",serial_no,period,inq,coname2,paye,aoname,team,nino,group);
		fprintf(notes,"%s,%s,%s,%s,%s,%s,%s,",score1,score2,score3,score4,score5,comm_marker,sano);
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
		read1();
	}

	fclose(in1not2);
	fclose(notes);

	fprintf(stdout,"Records read from input file = %ld\n",cnt1);
	fprintf(stdout,"Records written              = %ld\n",cnt2);
	tim = time(NULL);
	fprintf(stdout,"Program Ends OK  %s\n",ctime(&tim));

	return 0;

}

/******************************** reformat ****************************/

reformat()
{
	j=0;

	strncpy(inq,p1,3);		/* INQ CODE */
	inq[3]='\0';

	while(strncmp(p1,":",1) != 0)
	{
		p1++;
	}

	p1++;
	strncpy(serial_no,p1,7);	/* SERIAL NO */
	serial_no[7]='\0';

	p1 += 10;

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
	strncpy(paye,p1,len);	/* PAYE (DISTNO EMPREF)*/
	paye[len]='\0';

	p1 += len;
	p1++;

	strncpy(period,p1,4);	/* PERIOD */
	period[4]='\0';

	p1 += 4;
	if(strncmp(p1,"00",2) == 0)
	{
		period[4]='1';
		period[5]='2';
	}

	p1++;

	len = strcspn(p1,":");
	strncpy(aoname,p1,len);	/* NAME OF AO */
	aoname[len]='\0';


	p1 += len;
	p1++;

	strncpy(team,p1,1);     /*  TEAM   */
	team[1]='\0';

	p1 += 1;
	p1++;
	strncpy(nino,p1,9);    /*    NINO    */
	nino[9]='\0';

	p1 += 9;
	p1++;
	strncpy(group,p1,1);    /*   GROUP    */
	group[1]='\0';

	p1 += 2;                /* Position past the group field and colon */
  
  findcol();               /* find the field length */
	strncpy(score1,p1,a);    /*   SCORE1    */
	score1[a]='\0';
	
	p1 += (a+2);              /* Position past score1 */
	
  findcol();                /* find the field length */
	strncpy(score2,p1,a);     /*   SCORE2    */
	score2[a]='\0';

  p1 += (a+2);              /* Position past score2 */

  findcol();                /* find the field length */
	strncpy(score3,p1,a);     /*   SCORE3    */
	score3[a]='\0';

  p1 += (a+2);              /* Position past score3 */
  
  findcol();                /* find the field length */
	strncpy(score4,p1,a);     /*   SCORE4    */
	score4[a]='\0';

  p1 += (a+2);              /* Position past score4 */
  
  findcol();                /* find the field length */
	strncpy(score5,p1,a);     /*   SCORE5    */
	score5[a]='\0';

  p1 += (a+1);              /* Position past score5 */
  
/* NOTICED THAT THIS WAS FILLING UP THE LOG FOR NO REASON DJS 28/06/2012 17:54
  printf("OK to here 10 - p1 = %s\n",p1);
	fflush(stdout); */
  
	strncpy(comm_marker,p1,1);
	comm_marker[1]='\0';

	p1 += 1;
	p1++;
	strncpy(sano,p1,3);    /*   SA number    */
	sano[3]='\0';

	p1++;

	while(strncmp(p1,":",1) != 0)
	{
		p1++;
	}
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

	for(x=0;x<30;x++)
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

return 0;
}


/***************************   read1  ******************************/

read1()
{
	for(y=0;y<LEN;y++)
	{
		buf1[y] = '\0';
	}

	fgets(buf1,LEN,in1not2);
	if (feof(in1not2) != 0)
	{
		fprintf(stdout,"End of file %s\n",input1);
		return 0;
	}
	cnt1++;
	p1 = buf1;
	reformat();
	return 0;
}

/***************************   findcol  ******************************/

findcol()
{
	p3 = p1;
  a=0;
  while(strncmp(p3,":",1) != 0)
	{
		p3++;
		a++;
	}
 
	return 0;
}
