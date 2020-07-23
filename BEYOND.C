/**************************************************************************
PROGRAM NAME :	beyond (source beyond.c)
PURPOSE      : 	Converts the BeyondMail NAB into Notes format
PARAMETERS   :	Input filename
OUTPUT       :	NAB in the right format for Notes
COMMENTS     :	The format of the file from BeyondMail is
		N,Lee Turner,BMAIL,lturner@ons-mail
		The format of the output to Notes is
		Lee,Turner,lturner@MHS@LNROUTER,Lee Turner
	
WRITTEN BY   : Dominic
**************************************************************************/
#include <string.h>
#include <stdio.h>
#include <time.h>

#define LEN 1000
			/* file pointers */
FILE *beyond;		/* file from BeyondMail */
FILE *notes;		/* file to Notes */

char *p1;
char *p2;
char *input1;

char buf1[LEN];		/* buffer area to read in file 1 */

char firstname[100];
char lastname[100];
char unam[100];
char shortname[100];

long cnt1, cnt2;
time_t tim;
int reply, i, j, len, x, flag;

main(argc,argv)
int argc;
char *argv[];
{
	if(argc < 2)
	{
		fprintf(stdout,"Error, usage is beyond <BeyondMail File>\n");
		exit(1);
	}
   
	tim = time(NULL);
	fprintf(stdout,"\nProgram Begins  %s\n",ctime(&tim));
   
	input1 = argv[1];            
	
	if ((beyond = fopen(input1,"r")) == NULL)
	{
		fprintf(stdout,"Error opening %s does it exist?\n",input1);
		exit(1);
	}

	if ((notes = fopen("notesadd.txt","w")) == NULL)
	{
		fprintf(stdout,"Error creating notesadd.txt\n");
		exit(1);
	}

	cnt1=cnt2=0;

	read1();
	
	while ( feof(beyond) == 0 )	
	{
	 	fprintf(notes,"%s,%s,%s@MHS@LNROUTER,%s\n",firstname,lastname,unam,shortname);
		cnt2++;
		read1();
	}

	fclose(beyond);
	fclose(notes);
  
	fprintf(stdout,"Records read from input file = %ld\n",cnt1);
	fprintf(stdout,"Records written              = %ld\n",cnt2);
	tim = time(NULL);
	fprintf(stdout,"Program Ends OK  %s\n",ctime(&tim));

	return 0;

}

/******************************** reformat ****************************
WHAT HAPPENS HERE IS THAT THIS LAYOUT
N,Owen Abbott,MS-Mail,AbbotO @ ons-mail

IS CONVERTED TO THIS LAYOUT
Owen,Abbott,AbbotO@MHS@LNROUTER,Owen Abbott

where it gets complicated is - as ever the 1% of very important people
who have to be different.
**********************************************************************/
reformat()
{
	for(i=0;i<100;i++)
	{
		shortname[i]='\0';
		firstname[i]='\0';
		lastname[i]='\0';
		unam[i]='\0';
	}

	p1 += 2;		/* position past the N, */
	p2=p1;
	x=0;
	flag=0;
	while(x<20)		/* checking for the nightmare VIA PS names */
	{
		if(strnicmp(p2,"via ps",6) == 0)
		{
			flag=1;
			break;
		}
		x++;
		p2++;
	}
	if(flag == 1)			/* if there is a PS, position past it */
 	{
		len = x+7;
	}
	else
	{
		len = strcspn(p1," ");	/* by finding the first space work out the length */
	}				/* of the first name */
	strncpy(firstname,p1,len);
	firstname[len]='\0';
	p1 += len;			/* position past first name */
	p1++;
	len = strcspn(p1,",");		/* should give length of surname */
	strncpy(lastname,p1,len);
	lastname[len]='\0';
	strcpy(shortname,firstname);
	strcat(shortname," ");
	strcat(shortname,lastname);
	
	while(strncmp(p1,",",1)!=0)	/* position to comma after surname */
	{
		p1++;
	}
	p1++;				/* position past comma */
	while(strncmp(p1,",",1)!=0)	/* position to comma after mail type */
	{
		p1++;
	}
	p1 += 1;			/* position past comma */
	len = strcspn(p1," @");		/* find position of @ with space */
	if(len == 0)
	{
		len = strcspn(p1,"@");		/* find position of @ on its own*/
	}
	strncpy(unam,p1,len);
	p1[len]='\0';
	
return 0;
}


/***************************   read1  ******************************/

read1()
{
	fgets(buf1,LEN,beyond);
	if (feof(beyond) != 0)
	{
		fprintf(stdout,"End of file %s\n",input1);
		return 0;
	}
	cnt1++;
	p1 = buf1;
	reformat();
	return 0;
}
