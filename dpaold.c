/************************************************************
dpa.c

Trying to reformat the Data Protection Act Stuff from PS2000
to a format that will get it into Notes.

Format coming in

1	Staff No (or similar)
2	Surname
3	First Name
New 4	Preferred Name
4	Middle Name(s)
5	Previous Surname
6	Sex
7	DOB
8	Marital Status
9	Address Line 1
10	Address Line 2
11	Address Line 3
12	Address Line 4
13	Postcode
14	Home Telephone No
15	Emergency contact name
16	Emergency contact relation
17	Ethnic origin
18	Nationality code
19	Disability
20	NI number
---------------------------------------
21	Qualification level
22	Qualification type         OCCURS MANY TIMES
23	Subject
---------------------------------------
24	Current grade
25	Date to substantive grade
26	Method to current post
27	Date to current post
28	Division code
29	Group code
30	Location
31	Room no.
---------------------------------------
32	Historical job dates
33	Historical division codes		OCCURS MANY TIMES
34	Historical method to job
New	Historical Grade
--------------------------------------------		
35	Historical (appointment) status dates   OCCURS MANY TIMES
36	Historical statuses
37	Historical hours
---------------------------------------
38	Sick start dates
39	Sick end dates				OCCURS MANY TIMES
40	Sick reason descriptions
41	Sick certification codes
---------------------------------------
42	Staff appraisal start dates		
43	Staff appraisal end dates
44	Performance marks			OCCURS MANY TIMES
45	Performance marks on temp promotion
46	Grade report written on

The idea is to output six multiline delimited records per person,
the first containing fields 1 to 20 and 24 to 31,
the second containing all the occurrances of Qualifications
the third containing all the occurrances of job moves
the fourth containing the first 2000 or so bytes of sick absences
the fifth containing the next 2000 or so bytes of sick absences
the sixth containing all occurances of appraisals

The resulting file will have to be imported to Notes using the 
MULTILINE ability of ZMERGE.

And may God have mercy on my soul.
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
char mainrec[LEN];
char quals[LEN];
char moves[LEN];
char sick[LEN];
char sickcont[LEN];
char sickcont2[LEN];
char app[LEN];

long cnt1, cnt2, cnt3;
time_t tim;
int reply, commacnt, bytecnt, i, sicklen;

main(argc,argv)
int argc;
char *argv[];
{

if(argc < 3)
{
	fprintf(stderr,"Error, usage is dpa <input file> <output file>\n");
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

	if(isdigit(p1[0]))
	{
		/* do line 1 function */
		if(cnt1 > 1)
		{
			previous();
		}
		mainline();
	}
	else
	{
		/* do sub line function */
		subline();
	}

	read1();
}

previous();	/* DO THE LAST RECORD */

fclose(inp);
fclose(outp);

fprintf(stdout,"-------------------------------------------------------------------\n");  
fprintf(stdout,"Records read from %s	= %ld\n",input,cnt1);
fprintf(stdout,"Records written to %s		= %ld\n",output,cnt2);
fprintf(stdout,"Number of people			= %ld\n",cnt3);
fprintf(stdout,"-------------------------------------------------------------------\n");	
tim = time(NULL);
fprintf(stderr,"Program Ends OK  %s\n",ctime(&tim));

return 0;
}

/***************************   mainline  ******************************/
mainline()
{
sicklen=0;
commacnt=0;
bytecnt=0;

for(i=0;i<LEN;i++)
{
	mainrec[i] = '\0';
}
for(i=0;i<LEN;i++)
{
	quals[i] = '\0';
}
for(i=0;i<LEN;i++)
{
	moves[i] = '\0';
}
for(i=0;i<LEN;i++)
{
	sick[i] = '\0';
}
for(i=0;i<LEN;i++)
{
	sickcont[i] = '\0';
}
for(i=0;i<LEN;i++)
{
	sickcont2[i] = '\0';
}
for(i=0;i<LEN;i++)
{
	app[i] = '\0';
}

while (commacnt < 20)
{
	if(p1[0] == ',')
	{
		commacnt++;
	}
	p1++;
	bytecnt++;
}

strncpy(mainrec,buf,bytecnt);

p2 = p1;
bytecnt = 0;
while (commacnt < 23)
{
	if(p1[0] == ',')
	{
		commacnt++;
	}
	p1++;
	bytecnt++;
}
if(bytecnt > 4)
{
	strncpy(quals,p2,bytecnt);
	strcat(quals,"\n");
}

p2 = p1;
bytecnt = 0;
/*while (commacnt < 30) */
while (commacnt < 31)
{
	if(p1[0] == ',')
	{
		commacnt++;
	}
	p1++;
	bytecnt++;
}
if(bytecnt > 8)
{
	strncat(mainrec,p2,bytecnt-1);
}

p2 = p1;
bytecnt = 0;
/* while (commacnt < 36) */
while (commacnt < 37)
{
	if(p1[0] == ',')
	{
		commacnt++;
	}
	p1++;
	bytecnt++;
}
if(bytecnt > 7)
{
	strncpy(moves,p2,bytecnt);
	strcat(moves,"\n");
}

p2 = p1;
bytecnt = 0;
while (commacnt < 41 & p1[0] != '\n')
{
	if(p1[0] == ',')
	{
		commacnt++;
	}
	p1++;
	bytecnt++;
}
if(bytecnt > 5)
{
	strncpy(sick,p2,bytecnt);
	sicklen += bytecnt;
	strcat(sick,"\n");
	sicklen++;
}

p2 = p1;
bytecnt = 0;
while (p1[0] != '\n')
{
	p1++;
	bytecnt++;
}

if(bytecnt > 6)
{
	strncat(app,p2,bytecnt);
	strcat(app,"\n");
}

fprintf(outp,"%s\n",mainrec);
cnt2++;
cnt3++;
		/*if(cnt2 > 100000)
		{
			printf("100000 reached, buf = %s\n",buf);
			printf("cnt3 = %ld\n",cnt3);
			exit(0);
		}*/
return 0;
}

/***************************   subline  ******************************/
subline()
{
commacnt=0;

while (commacnt < 20)
{
	if(p1[0] == ',')
	{
		commacnt++;
	}
	p1++;
}
		
p2 = p1;
bytecnt = 0;
while (commacnt < 24)
{
	if(p1[0] == ',')
	{
		commacnt++;
	}
	p1++;
	bytecnt++;
}

if(bytecnt > 5)
{
	strncat(quals,p2,bytecnt);
	strcat(quals,"\n");
}

p2 = p1;
/* while (commacnt < 30) */
while (commacnt < 31)
{
	if(p1[0] == ',')
	{
		commacnt++;
	}
	p1++;
}

p2 = p1;
bytecnt = 0;
/*while (commacnt < 36) */
while (commacnt < 37)
{
	if(p1[0] == ',')
	{
		commacnt++;
	}
	p1++;
	bytecnt++;
}
			
if(bytecnt > 7)
{
	strncat(moves,p2,bytecnt);
	strcat(moves,"\n");
}

p2 = p1;
bytecnt = 0;
while (commacnt < 41 & p1[0] != '\n')
{
	if(p1[0] == ',')
	{
		commacnt++;
	}
	p1++;
	bytecnt++;
}
			
if(bytecnt > 5)
{
	sicklen += bytecnt;
	sicklen++;
	if(sicklen > 6000)
	{
		printf("Oops, sick array out of bounds, person = %s\n",mainrec);
		exit(0);
	}
	if(sicklen > 3950)
	{
		strncat(sickcont2,p2,bytecnt);
		strcat(sickcont2,"\n");
	}
	else
	{
		if(sicklen > 1950)
		{
			strncat(sickcont,p2,bytecnt);
			strcat(sickcont,"\n");
		}
		else
		{
			strncat(sick,p2,bytecnt);
			strcat(sick,"\n");
		}
	}
}

p2 = p1;
bytecnt = 0;
while (p1[0] != '\n')
{
	p1++;
	bytecnt++;
}
if(bytecnt > 8)
{
	strncat(app,p2,bytecnt);
	strcat(app,"\n");
}
return 0;
}

/***************************   previous  ******************************/
previous()
{
if(strlen(quals) > 1)
{
	fprintf(outp,"%s#\n",quals);
}
else
{
	fprintf(outp,"None recorded#\n");
}

if(strlen(moves) > 1)
{
	fprintf(outp,"%s##\n",moves);
}
else
{
	fprintf(outp,"None recorded##\n");
}

if(strlen(sick) > 1)
{	
	fprintf(outp,"%s###\n",sick);
	fprintf(outp,"%s####\n",sickcont);
	fprintf(outp,"%s#####\n",sickcont2);
}
else
{
	fprintf(outp,"None recorded###\n");
	fprintf(outp,"####\n");
	fprintf(outp,"#####\n");
}

if(strlen(app) > 1)
{
	fprintf(outp,"%s######\n",app);
}
else
{
	fprintf(outp,"None recorded######\n");
}

cnt2 += 5;

/* fprintf(outp,"\n"); */
							
return 0;
}

/***************************   read1  ******************************/
read1()
{
for(i=0;i<LEN;i++)
{
	buf[i] = '\0';
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
