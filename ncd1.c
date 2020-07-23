/************************************************************
ncd1.c

Taking PS2000 extract and removing any preferred names that
exactly match the first name

Format coming in

1	Staff No
2	First Name
3	Preferred Name			
4	Middle Name(s)
5	Surname
6	Sex
7	Site
8	Room
9	Group
10	Division
11	Branch
12	Pay Band
13	Reporting Officer
14	Countersigning Officer
15	Cost Centre
16	Employment Status
17	FTE
************************************************************/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define LEN 2048
		      /* file pointers */
FILE *inp;            /* PS2000 file   */
FILE *outp;           /* Output file */

char *input;
char *output;

char *p1;
char *p2;

char buf[LEN];       /* buffer area to read input */
char bufout[LEN];
char firstname[80];
char prefname[80];

long cnt1, cnt2, cnt3;
time_t tim;
int reply, commacnt, bytecnt, i, len;

main(argc,argv)
int argc;
char *argv[];
{

if(argc < 3)
{
	fprintf(stderr,"Error, usage is ncd1 <input file> <output file>\n");
	exit(1);
}
   
tim = time(NULL);
fprintf(stderr,"\nProgram Begins  %s\n",ctime(&tim));
   
input = argv[1];            
output = argv[2];            
   
if ((inp = fopen(input,"r")) == NULL)
{
	fprintf(stderr,"Error opening %s does it exist?\n",input);
	exit(1);
}

if ((outp = fopen(output,"w")) == NULL)
{
	fprintf(stderr,"Error creating %s\n",output);
	exit(1);
}

cnt1=0;
cnt2=0;
cnt3=0;

read1();

while (feof(inp) == 0)
{
	p1 = buf;
	p2 = buf;
	
	strncpy(bufout,p1,7);
	
	p1 += 7;
	p2 += 7;
	len = 0;

	while(p2[0] != ',')
	{
		p2++;
		len++;
	}
	
	strncpy(firstname,p1,len);
	firstname[len] = '\0';
	strcat(bufout,firstname);
	strcat(bufout,",");
	p1 += len;
	p1++;
	p2++;
	len = 0;

	while(p2[0] != ',')
	{
		p2++;
		len++;
	}
	
	strncpy(prefname,p1,len);
	prefname[len] = '\0';
	
	reply = strcmp(firstname,prefname);
	
	if (reply != 0)
	{
		strcat(bufout,prefname);
	}
	
	
	strcat(bufout,p2);

	fprintf(outp,"%s",bufout);
	cnt2++;

	read1();
}


fclose(inp);
fclose(outp);

fprintf(stdout,"-------------------------------------------------------------------\n");  
fprintf(stdout,"Records read from %s	= %ld\n",input,cnt1);
fprintf(stdout,"Records written to %s	= %ld\n",output,cnt2);
fprintf(stdout,"-------------------------------------------------------------------\n");	
tim = time(NULL);
fprintf(stderr,"Program Ends OK  %s\n",ctime(&tim));

return 0;
}

/***************************   read1  ******************************/
read1()
{
for(i=0;i<LEN;i++)
{
	buf[i] = '\0';
	bufout[i] = '\0';
}
fgets(buf,LEN,inp);
if (feof(inp) != 0)
{
	fprintf(stderr,"End of file %s\n",input);
	return 0;
}
cnt1++;
return 0;
}
