/************************************************************
ncd2.c

Taking cost centre details and inserting leading zeros where
necessary
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
	fprintf(stderr,"Error, usage is ncd2 <input file> <output file>\n");
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
	
	len = 0;

	while(p1[0] != ':')
	{
		p1++;
		len++;
	}

	switch (len)
	{
		case 5 : strcpy(bufout,"0");break;
		case 4 : strcpy(bufout,"00");break;
	}
	
	strcat(bufout,buf);
		
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
