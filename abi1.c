/**************************************************************************
PROGRAM NAME :	abi (source abi1.c)
PURPOSE      : 	Takes the file of Ingres batch take-on errors supplied by
		ABI and re-formats it to look nice for ZMerge to stick it
		into Notes.
PARAMETERS   :	Input filename
OUTPUT       :	Notesinp.dat which goes into ZMerge. 
COMMENTS     :	The format of the file from abi1 is :
		inquiry code (letters):ruref:checkletter:SIC92:period:AO Name:
                Address Change (1 or 2):Error text

CHANGE LOG
		PLEASE NOTE : THE ADDRESS CHANGE FIELD ACTUALLY IS
		1 = COMMENT PRESENT
		2 = NO COMMENT
		I have changed the code below accordingly, I have left the
		fields misleadingly called add_err*	Dom 5/2/97

		FURTHER CHANGE : The "Address Change" field can now contain
		the value 5 to denote a touchtone record and the program
		now deals with CAPEX (inquiry 19 and 20)	Dom 29/9/97

		The format of the output file is
		ruref,period,inqcode,sic92,AO Name,comment(Y or N),
		error text,clearance marker (always N),image location
	
WRITTEN BY   : Dominic
**************************************************************************/
#include <string.h>
#include <stdio.h>
#include <time.h>

#define LEN 1000
					/* file pointers */
FILE *abi1;		/* file from abi1 */
FILE *notes;		/* the merged file */

char *p1;
char *p2;
char *input1;

char buf1[LEN];		/* buffer area to read in file 1 */

char inq[4];
char ruref[12];
char period[7];
char sic92[6];
char coname[80];
char coname2[80];
char firstname[80];
char lastname[80];
char add_err[8];
char error_text[256];
char error_arr[10][80];

long cnt1, cnt2;
time_t tim;
int reply, i, j, x, w, z, len, errlen, ptrctr;

main(argc,argv)
int argc;
char *argv[];
{

	if(argc < 2)
	{
		fprintf(stdout,"Error, usage is abi1 <abi1's file>\n");
		exit(1);
	}
   
	tim = time(NULL);
	fprintf(stdout,"\nProgram Begins  %s\n",ctime(&tim));
   
	input1 = argv[1];            
	
	if ((abi1 = fopen(input1,"r")) == NULL)
	{
		fprintf(stdout,"Error opening %s does it exist?\n",input1);
		exit(1);
	}

	if ((notes = fopen("notesinp.dat","w")) == NULL)
	{
		fprintf(stdout,"Error creating notesinp.dat\n");
		exit(1);
	}

	cnt1=cnt2=0;

	read1();
	
	while ( feof(abi1) == 0 )	
	{
/*	 	fprintf(notes,"%s,%s,%s,%s,%s,%s %s,",ruref,period,inq,coname,sic92,firstname,lastname);*/
		fprintf(notes,"%s,%s,%s,%s,%s,%s %s,",ruref,period,inq,coname2,sic92,firstname,lastname);
		fprintf(notes,"%s,\n",error_arr[0]);
		if(x>0)
		{
			for(w=1;w<(x-1);w++)
			{
				fprintf(notes,"%s\n",error_arr[w]);
			}
			fprintf(notes,"%s",error_arr[x]);
	/*		fprintf(notes,"###\n");                    */
		}
		fprintf(notes,"###\n");
		cnt2++;
		read1();
	}

	fclose(abi1);
	fclose(notes);
  
	fprintf(stdout,"Records read from input file = 	%ld\n",cnt1);
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
	{									/* COMMAS IN THE COMPANY */
		if(coname[z] == ',')			/* NAME THROWING OUT */
		{								/* THE ZMERGE IMPORT */
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
	period[4]='1';
	period[5]='2';

	p1 += 7;
	
	len = strcspn(p1," ");
	strncpy(firstname,p1,len);	/* FIRST NAME OF AO */
	firstname[len]='\0';
	
	p1 += len;
	p1++;

	len = strcspn(p1," ");
	strncpy(lastname,p1,len);	/* SECOND NAME OF AO */
	lastname[len]='\0';

	while(strncmp(p1,":",1) != 0)
	{
		p1++;
	}
	p1++;

	p2 = strstr(p1,"  ");
	p2[0] = '\n';
	len = strcspn(p1,"\n");
	strncpy(error_text,p1,len);
	error_text[len]='\0';
	p1 = error_text;
	errlen = strlen(error_text);
	ptrctr = 0;
	
	for(x=0;x<10;x++)
	{
			for(w=0;w<80;w++)
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
	fgets(buf1,LEN,abi1);
	if (feof(abi1) != 0)
	{
		fprintf(stdout,"End of file %s\n",input1);
		return 0;
	}
	cnt1++;
	p1 = buf1;
	reformat();
	return 0;
}
