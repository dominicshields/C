/**************************************************************************
PROGRAM NAME :	reformat (source reformat.c)
PURPOSE      : 	Takes the file of Ingres batch take-on errors supplied by
		Ellen and re-formats it to look nice for ZMerge to stick it
		into Notes.
PARAMETERS   :	Input filename
OUTPUT       :	Notesinp.dat which goes into ZMerge. 
COMMENTS     :	The format of the file from Ellen is :
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

		FURTHER CHANGE - 06/10/98 : Now catering for company name
		and got shot of the not used image location

		The format of the output file is
		ruref,company name, period,inqcode,sic92,AO Name,comment(Y or N),
		error text,clearance marker (always N) Dom 06/10/98

	
WRITTEN BY   : Dominic
**************************************************************************/
#include <string.h>
#include <stdio.h>
#include <time.h>

#define LEN 1000
					/* file pointers */
FILE *ellen;		/* file from ellen */
FILE *notes;		/* the merged file */

char *p1;
char *p2;
char *input1;

char buf1[LEN];		/* buffer area to read in file 1 */

char inq[4];
char ruref[12];
char fil[10];
char period[7];
char period_hold[5];
char mm[3];
char sic92[6];
char firstname[80];
char company[80];
char lastname[80];
char add_err[8];
char error_text[1024];
char error_text_nocomm[1024];
char image_loc[256];
char inqarr[9][2][4] = {
"MP","012","CS","016","PS","017","ES","017","MQ","017","RS","018","WS","018","MT","018","  ","019"
};					

long cnt1, cnt2;
time_t tim;
int reply, i, j, z, len, add_err_int;

main(argc,argv)
int argc;
char *argv[];
{

/*********
fprintf(stdout,"The array contains\n");
for(i=0;i<8;i++)
{
	fprintf(stdout,"%s %s\n",inqarr[i][0],inqarr[i][1]);
}	
*************/

	if(argc < 2)
	{
		fprintf(stdout,"Error, usage is reformat <ellen's file>\n");
		exit(1);
	}
   
	tim = time(NULL);
	fprintf(stdout,"\nProgram Begins  %s\n",ctime(&tim));
   
	input1 = argv[1];            
	
	if ((ellen = fopen(input1,"r")) == NULL)
	{
		fprintf(stdout,"Error opening %s does it exist?\n",input1);
		exit(1);
	}

	if ((notes = fopen("D:\\NOTESDATA\\FILES\\PROCESS\\notesinp.dat","w")) == NULL)
	{
		fprintf(stdout,"Error creating notesinp.dat\n");
		exit(1);
	}

	cnt1=cnt2=0;

	read1();
	
	while ( feof(ellen) == 0 )	
	{
	 	fprintf(notes,"%s,%s,%s,%s,%s,%s %s,",ruref,company,period,inq,sic92,firstname,lastname);
		if(add_err_int == 1)
		{
			fprintf(notes,"Y,");	/* LOGIC SWAPPED 5/2/97 */
		}				/* SEE COMMENTS ABOVE   */
		else
		{
			if(add_err_int == 5)
			{
				fprintf(notes,"5,");
			}
			else
			{
				fprintf(notes,"N,");			
			}
		}
		fprintf(notes,"%s,N\n",error_text_nocomm);
		cnt2++;
		read1();
	}

	fclose(ellen);
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
	while(j<9)
	{
		if(strncmp(p1,inqarr[j],2) == 0)
		{
			strncpy(inq,inqarr[j][1],3);
			inq[3]='\0';
		}
		j++;
	}

	while(strncmp(p1,":",1) != 0)
	{
		p1++;
	}
	
	p1++;
	strncpy(ruref,p1,11);
	ruref[11]='\0';
	
	p1 += 14;	

	len = strcspn(p1,":");
	strncpy(company,p1,len);
	company[len]='\0';

	p1 += len;
	p1++;
	
	while(strncmp(p1," ",1) == 0)
	{
		p1++;
	}
		
	len = strcspn(p1,":");
	strncpy(sic92,p1,len);
	sic92[len]='\0';

	if (strcmp(inq,"019") == 0)		/* SORTING OUT THE CAPEX PROD/NON-PROD */
	{
		if(strncmp(sic92,"4",1) > 0)
		{
			strncpy(inq,"020",3);
		}
		if(strncmp(sic92,"0141",4) == 0)
		{
			strncpy(inq,"020",3);
		}
	}
		
	p1 += len + 1;

	len = strcspn(p1," ");
	if(len > 4)
	{
		strncpy(period,p1,6);
		p1 += 7;
	}
	else
	{
		/* the dickhead coding here is not my choosing, crappy MS Visual C++
		will only allow it to work this way */

		strcpy(period,"");
		strcpy(period_hold,"");
		strncpy(period_hold,p1,len);
		period_hold[4]='\0';
		p1 += len + 1;
		strncpy(mm,p1,2);
		mm[2]='\0';
		strcpy(period,period_hold);
		strcat(period,mm);
		period[6]='\0';
		p1 += 3;
	}
		
	len = strcspn(p1," ");
	strncpy(firstname,p1,len);
	firstname[len]='\0';		
	p1 += len + 1;
	len = strcspn(p1," ");
	strncpy(lastname,p1,len);
	lastname[len]='\0';
	while(strncmp(p1,":",1) != 0)
	{
		p1++;
	}
	p1++;
	len = strcspn(p1,":");
	strncpy(add_err,p1,len);	
	add_err[len]='\0';
	add_err_int = atoi(add_err);
	p1 += len + 1;
	len = strcspn(p1,"\n");	
	strncpy(error_text,p1,len);
	error_text[len]='\0';

	
	for(z=0;z<len;z++)					/* THIS IS TO DEAL WITH */
	{									/* COMMAS IN THE COMPANY */
		if(error_text[z] == ',')			/* NAME THROWING OUT */
		{								/* THE ZMERGE IMPORT */
			error_text_nocomm[z] = ' ';
		}
		else
		{
			error_text_nocomm[z] = error_text[z];
		}
			
	}
	error_text_nocomm[len]='\0';

/*
	for(i=0;i<256;i++)
	{
		image_loc[i]='\0';
	}
	p1=ruref;
	strcpy(image_loc,"\\\\NPTCI\\CI\\MPTIFF00\\");
	strncat(image_loc,ruref,8);
	p1 += 8;
	strcat(image_loc,".");
	strcat(image_loc,p1);
	strcat(image_loc,"\\");
	strcat(image_loc,inq);
	strcat(image_loc,"\\pe");
	strcat(image_loc,period);
	strcat(image_loc,".tif"); */

return 0;
}


/***************************   read1  ******************************/

read1()
{
	fgets(buf1,LEN,ellen);
	if (feof(ellen) != 0)
	{
		fprintf(stdout,"End of file %s\n",input1);
		return 0;
	}
	cnt1++;
	p1 = buf1;
	reformat();
	return 0;
}
