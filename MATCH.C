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
COMMENTS     :	I'm assuming for the sake of getting this program written that
		the format of the file from Ellen is :
		ruref,inquiry code,period,SIC92,AO Name,Address Change (Y or N),
		Error text
		The format of the file from Colin is :
		ruref,inquiry code,period,image location
	
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

long cnt1, cnt2, cnt3, cnt4, cnt5;
time_t tim;
int reply;

main(argc,argv)
int argc;
char *argv[];
{
   
	if(argc < 3)
	{
		fprintf(stderr,"Error, usage is ellcol <ellen's file> <colin's file>\n");
		exit(1);
	}
   
	tim = time(NULL);
	fprintf(stderr,"\nProgram Begins  %s\n",ctime(&tim));
   
	input1 = argv[1];            
	input2 = argv[2];            
   
	if ((ellen = fopen(input1,"r")) == NULL)
	{
		fprintf(stderr,"Error opening %s does it exist?\n",input1);
		exit(1);
	}

	if ((colin = fopen(input2,"r")) == NULL)
	{
		fprintf(stderr,"Error opening %s does it exist?\n",input2);
		exit(1);
	}

	if ((notes = fopen("notesinp.dat","w")) == NULL)
	{
		fprintf(stderr,"Error creating notesinp.dat\n");
		exit(1);
	}

	cnt1=cnt2=cnt3=cnt4=cnt5=0;

	read1();
	read2();

	while ( (feof(ellen) == 0) && (feof(colin) == 0) )
	{
		p1 = buf1;
		p2 = buf2;         
		fprintf(stderr,"p1 = %s\n",p1);   
		fprintf(stderr,"p2 = %s\n",p2);  

		reply = strncmp(p1,p2,21);
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
			p2 += 22;
			buf1[(strlen(buf1)-1)] = '\0';
			strcat(buf1,",N,");
			strcat(buf1,p2);	
			fputs(buf1,notes);
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
	fprintf(stderr,"Program Ends OK  %s\n",ctime(&tim));

	return 0;

}

/***************************   read1  ******************************/
read1()
{
	fgets(buf1,LEN,ellen);
	if (feof(ellen) != 0)
	{
		fprintf(stderr,"End of file %s\n",input1);
		return 0;
	}
	cnt1++;
	return 0;
}

/***************************   read2  ******************************/
read2()
{
 
	fgets(buf2,LEN,colin);
	if (feof(colin) != 0)
	{
		fprintf(stderr,"End of file %s\n",input2);
		return 0;
	}
	cnt2++;
	return 0;
}
