/**************************************************************************
PROGRAM NAME : aces3  (source aces3.c)
**************************************************************************/
#include <string.h>
#include <stdio.h>
#include <time.h>

FILE *inp;		/* input file */
FILE *cap;		/* main output file */
FILE *sto;		/* stocks output file */

char *p1;
char *p2;
char *input;
char *output1;
char *output2;
char buf[1000];		/* buffer area to read input file */
char buf2[10000];	/* buffer area to hold main joined file */
char buf3[10000];	/* buffer area to hold 2nd joined file */
char capex[10000];
char stocks[10000];
char sic[6];
char period[7];
char hold[15];
int reply;
int cnt1, cnt2, i, j, x, len;
time_t tim;

main(argc,argv)
int argc;
char *argv[];
{
   if(argc < 4)
   {
     fprintf(stderr,"Error, usage is aces3 <inputfilename> <main output file> <stocks output file>\n");
      exit(1);
   }

   tim = time(NULL);
   fprintf(stderr,"\nProgram Begins  %s\n",ctime(&tim));

   cnt1=0;
   cnt2=0;
   input = argv[1];	/* input file assigned to pointer */
   output1 = argv[2];	/* main output file assigned to pointer */
   output2 = argv[3];	/* output file assigned to pointer */

   if ((inp = fopen(input,"r")) == NULL)
   {
      fprintf(stderr,"Error opening %s does it exist?\n",input);
      exit(1);
   }

   if ((cap = fopen(output1,"w")) == NULL)
   {
      fprintf(stderr,"Error opening %s permissions?, disk full?\n",output1);
      exit(1);
   }

   if ((sto = fopen(output2,"w")) == NULL)
   {
      fprintf(stderr,"Error opening %s permissions?, disk full?\n",output2);
      exit(1);
   }

   read1();
   strncpy(hold,buf,len);
   findname();

   read1();

   while (feof(inp) == 0)
   {
	if(strncmp(hold,buf,len) != 0)
	{
		strcat(buf2,":");
		strcat(buf2,sic);
		strcat(buf2,":");
		strcat(buf2,capex);
		strcat(buf2,"###\n");
		strcpy(buf3,hold);
		strcat(buf3,":");
		strcat(buf3,stocks);
		strcat(buf3,"###\n");
		fputs(buf2,cap);
		fputs(buf3,sto);
		for(j=0;j<10000;j++)
		{
			buf2[j]='\0';
			buf3[j]='\0';
			capex[j]='\0';
			stocks[j]='\0';
		}
		cnt2++;
   		strncpy(hold,buf,len);
	}
		
   		findname();
   		read1();
   }

   fclose(inp);


   fprintf(stderr,"Records read from input file   = %d\n",cnt1);
   fprintf(stderr,"Records written to output file = %d\n",cnt2);
   tim = time(NULL);
   fprintf(stderr,"Program Ends OK  %s\n",ctime(&tim));

   return 0;

 }

 /***************************   findname  ******************************/

 /* layout is 
 capex file : ruref : name : sic : period : capex comment 
 stocks file : ruref : stocks comment */

findname()
{
   j=0;
   x=0;
   p1 = buf;			/* point at buffer */
   while(j<2)
   {
	p1++;
	x++;
	if(strncmp(p1,":",1) == 0)
	{
		j++;
	}
   }
   strncpy(buf2,buf,x);		/* buf2 contains ruref:name */
   x=0;
   p2=p1;			/* synchronize pointers */
   p2++;			/* increment p2 */
   while(j<3)
   {
	p1++;
	x++;
	if(strncmp(p1,":",1) == 0)
	{
		j++;
	}
   }
   strncpy(sic,p2,x-1);		/* holds latest sic */
   p1++;  			/* increment p1 to point at period */
   strncpy(period,p1,6);	/* holds period */
   p1+=6;			/* move pointer over period */
   p2=p1;			/* synchronize pointers */
   x=0;
   j=4;
   while(j<5)
   {
	p1++;
	x++;
	if(strncmp(p1,":",1) == 0)
	{
		j++;
	}
   }
   if(x>1)
   {
	p2++;
	strncat(capex,period,6);	/* copy capex period and */
	strcat(capex,"  ");		/* comment with 2 spaces */
	strncat(capex,p2,x-1);
	strcat(capex,"\n");
   }
   p1++;
   p2=p1;			/* synchronize pointers */
   x=0;
   while(j<6)
   {
	p1++;
	x++;
	if(strncmp(p1,"\n",1) == 0)
	{
		j++;
	}
   }
   if(x>1)
   {
	strncat(stocks,period,6);	/* copy stocks period and */
	strcat(stocks,"  ");		/* comment with 2 spaces */
	strncat(stocks,p2,x);
	strcat(stocks,"\n");
   }

}


 /***************************   read1  ******************************/

 read1()
 {

  fgets(buf,512,inp);
  if (feof(inp) != 0)
  {
     fprintf(stderr,"End of file %s\n",input);
     return 0;
  }

  len = strcspn(buf,":");
  cnt1++;
  i++;
  return 0;
 }
