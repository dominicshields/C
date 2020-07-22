/**************************************************************************
PROGRAM NAME : aces4  (source aces4.c)
**************************************************************************/
#include <string.h>
#include <stdio.h>
#include <time.h>

FILE *inp;		/* input file */
FILE *out;		/* output file */

char *p1;
char *p2;
char *input;
char *output;
char buf[1000];		/* buffer area to read input file */
char buf2[10000];	/* buffer area to hold joined file */
char comment[10000];
char sic[6];
char period[7];
char hold[15];
int reply;
int cnt1, cnt2, i, j, l, x, len, newflag;
time_t tim;

main(argc,argv)
int argc;
char *argv[];
{
   if(argc < 3)
   {
     fprintf(stderr,"Error, usage is aces4 <inputfilename> <outputfilename>\n");
      exit(1);
   }

   tim = time(NULL);
   fprintf(stderr,"\nProgram Begins  %s\n",ctime(&tim));

   cnt1=0;
   cnt2=0;
   input = argv[1];	/* input file assigned to pointer */
   output = argv[2];	/* output file assigned to pointer */

   if ((inp = fopen(input,"r")) == NULL)
   {
      fprintf(stderr,"Error opening %s does it exist?\n",input);
      exit(1);
   }

   if ((out = fopen(output,"w")) == NULL)
   {
      fprintf(stderr,"Error opening %s permissions?, disk full?\n",output);
      exit(1);
   }

   newflag=0;
   read1();
   strncpy(hold,buf,len);
   findname();

   read1();

   while (feof(inp) == 0)
   {
	if(strncmp(hold,buf,len) != 0)
	{
   		newflag=0;
		strcat(buf2,":");
		strcat(buf2,sic);
		strcat(buf2,":\n");
		strcat(buf2,comment);
		strcat(buf2,"###\n");
		fputs(buf2,out);
		for(j=0;j<10000;j++)
		{
			buf2[j]='\0';
			comment[j]='\0';
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
    ruref : name : sic : period : comment */

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
	p1++;			/* find the sic */
	x++;
	if(strncmp(p1,":",1) == 0)
	{
		j++;
	}
   }
   if(newflag == 0)
   {
   	for(l=0;l<6;l++)		/* sic found */		
   	{
		sic[l]='\0';
   	}
   	strncpy(sic,p2,x-1);		/* holds latest sic */
	newflag=1;
   }
   p1++;  			/* increment p1 to point at period */
   strncpy(period,p1,6);	/* holds period */
   p1+=6;			/* move pointer over period */
   p2=p1;			/* synchronize pointers */
   x=0;
   j=4;
   p2++;
   strncat(comment,period,6);	/* copy capex period and */
   strcat(comment,"  ");		/* comment with 2 spaces */
   strcat(comment,p2);
  /*************** strcat(comment,"\n");  ****/
  
}


 /***************************   read1  ******************************/

 read1()
 {
  for(j=0;j<1000;j++)
  {
	buf[j]='\0';
  }
  fgets(buf,1000,inp);
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
