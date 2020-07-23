/**************************************************************************
PROGRAM NAME :	ellcol (source ellcol.c)
PURPOSE      : 	Takes the file of Ingres batch take-on errors supplied by
		Ellen and matches it with the file containing image location
		details from Colin Bowler and puts the image location on the
		end of Ellen's file.
PARAMETERS   : 
OUTPUT       : 
WARNING      :	don't even think about running this program if the files are
	      	not sorted on the fields to be matched
COPYRIGHT    :	this program is based on Wayne's JPFULLMATCH filetab
COMMENTS     :	The format of the file from Ellen is :
		inquiry code (letters):ruref:checkletter:SIC92:period:AO Name:
                Address Change (1 or 2):Error text
		The format of the file from Colin is :
		"ruref","period","inquiry code","image location"
		The format of the output file is
		ruref,period,inqcode,sic92,AO Name,address change(Y or N),
		error text,clearance marker (always N),image location
	
WRITTEN BY   : Dominic
**************************************************************************/
#include <string.h>
#include <stdio.h>
#include <time.h>

#define LEN 1000
		      /* file pointers */
FILE *ellen;          /* file from ellen */
FILE *colin;          /* file from colin  */
FILE *notes;          /* the merged file */

char *p1;
char *p2;
char *input1;
char *input2;

char buf1[LEN];       /* buffer area to read in file 1 */
char buf2[LEN];

char comp1[21];
char comp2[21];
char inq1[4];
char ruref1[12];
char period1[7];
char sic92[6];
char inq2[4];
char ruref2[12];
char period2[7];
char firstname[80];
char lastname[80];
char add_err[8];
char error_text[256];
char image_loc[256];
char inqarr[8][2][4] = {
"MP","012","CS","016","PS","017","ES","017","MQ","017","RS","018","WS","018","MT","018"				
};					

long cnt1, cnt2, cnt3, cnt4, cnt5;
time_t tim;
int reply, i, j, len, add_err_int;

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

	if(argc < 3)
	{
		fprintf(stdout,"Error, usage is ellcol <ellen's file> <colin's file>\n");
		exit(1);
	}
   
	tim = time(NULL);
	fprintf(stdout,"\nProgram Begins  %s\n",ctime(&tim));
   
	input1 = argv[1];            
	input2 = argv[2];            
   
	if ((ellen = fopen(input1,"r")) == NULL)
	{
		fprintf(stdout,"Error opening %s does it exist?\n",input1);
		exit(1);
	}

	if ((colin = fopen(input2,"r")) == NULL)
	{
		fprintf(stdout,"Error opening %s does it exist?\n",input2);
		exit(1);
	}

	if ((notes = fopen("notesinp.dat","w")) == NULL)
	{
		fprintf(stdout,"Error creating notesinp.dat\n");
		exit(1);
	}

	cnt1=cnt2=cnt3=cnt4=cnt5=0;

	read1();
	read2();

	while ( (feof(ellen) == 0) && (feof(colin) == 0) )	
	{
	         
		fprintf(stdout,"comp1 = %s\n",comp1);   
		fprintf(stdout,"comp2 = %s\n",comp2);  

		reply = strcmp(comp1,comp2);
		if(reply > 0)
		{
			read2();
		}

		if(reply < 0)
		{
			read1();
		}

		if(reply == 0)
		{
			
			fprintf(notes,"%s,%s,%s,%s,%s %s,",ruref1,period1,inq1,sic92,firstname,lastname);
			if(add_err_int == 1)
			{
				fprintf(notes,"N,");
			}
			else
			{
				fprintf(notes,"Y,");
			}
			fprintf(notes,"%s,N,%s\n",error_text,image_loc);
			cnt3++;
			read1();
			read2();
		}
	}

	if (feof(ellen) != 0)
	{
		while(feof(colin) == 0)
		{
			cnt5++;
			read2();
		}
	}

	if (feof(colin) != 0)
	{
		while(feof(ellen) == 0)
		{
			cnt4++;
			read1();
		}
	}

	fclose(ellen);
	fclose(colin);
	fclose(notes);
  
	fprintf(stdout,"Records read from file 1     = %ld\n",cnt1);
	fprintf(stdout,"Records read from file 2     = %ld\n",cnt2);
	fprintf(stdout,"Matching records             = %ld\n",cnt3);
	tim = time(NULL);
	fprintf(stdout,"Program Ends OK  %s\n",ctime(&tim));

	return 0;

}

/******************************** reformat ****************************/

reformat1()
{
	j=0;
	while(j<8)
	{
		if(strncmp(p1,inqarr[j],2) == 0)
		{
			strncpy(inq1,inqarr[j][1],3);
			inq1[3]='\0';
		}
		j++;
	}

	while(strncmp(p1,":",1) != 0)
	{
		p1++;
	}
	
	p1++;
	strncpy(ruref1,p1,11);
	ruref1[11]='\0';
	
	p1 += 14;	

	while(strncmp(p1," ",1) == 0)
	{
		p1++;
	}
		
	len = strcspn(p1,":");
	strncpy(sic92,p1,len);
	sic92[len]='\0';
		
	p1 += len + 1;

	len = strcspn(p1," ");
	if(len > 4)
	{
		strncpy(period1,p1,6);
		p1 += 7;
	}
	else
	{
		strncpy(period1,p1,len);
		p1 += len + 1;
		strncat(period1,p1,2);
		period1[6]='\0';
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

	strcpy(comp1,ruref1);
	strcat(comp1,period1);
	strcat(comp1,inq1);

return 0;
}

/******************************** reformat ****************************/

reformat2()
{
	p2++;
	strncpy(ruref2,p2,11);
	ruref2[11]='\0';
	p2 += 14;
	strncpy(period2,p2,6);
	period2[6]='\0';
	p2 += 9;
	strncpy(inq2,p2,3);
	inq2[3]='\0';
	len = strcspn(p2,"\\");
	p2 += len;
	len = strcspn(p2,".");
	strncpy(image_loc,p2,len+4);
	image_loc[len+4]='\0';
	strcpy(comp2,ruref2);
	strcat(comp2,period2);
	strcat(comp2,inq2);

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
	reformat1();
	return 0;
}

/***************************   read2  ******************************/

read2()
{
 
	fgets(buf2,LEN,colin);
	if (feof(colin) != 0)
	{
		fprintf(stdout,"End of file %s\n",input2);
		return 0;
	}
	cnt2++;
	p2 = buf2;
	reformat2();
	return 0;
}
