/**************************************************************************
PROGRAM NAME : dssd  (source dssd.c)
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
char buf[2000];		/* buffer area to read input file */
char buf2[10000];	/* buffer area to hold joined file */
char comment[10000];
char ruref[12];
char name[36];
char sic[6];
char period[7];
int reply;
int cnt1, cnt2, i, j, l, x, len, newflag,namelen;
time_t tim;

main(argc,argv)
int argc;
char *argv[];
{
   if(argc < 3)
   {
     fprintf(stderr,"Error, usage is %s <inputfilename> <outputfilename>\n",argv[0]);
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
   p1 = buf;
   strncpy(ruref,buf,11);
   p1 += 20;
   strncpy(sic,p1,5);
   p1 = buf;
   p1 +=26;
   len = strcspn(p1,"|");
   namelen = len;
   strncpy(name,p1,len);

/*   findname();*/

   while (feof(inp) == 0)
   {
	if(strncmp(ruref,buf,11) != 0) /* checks for new ruref */
	{
   		newflag=0;
		strncpy(buf2,ruref,11);
		strcat(buf2,"|");
		strncat(buf2,name,namelen);
		strcat(buf2,"|");
		strcat(buf2,sic);
		strcat(buf2,"|\n");
		strcat(buf2,comment);
		strcat(buf2,"###\n");
		fputs(buf2,out);
		cnt2++;
		for(j=0;j<10000;j++)
		{
			buf2[j]='\0';
			comment[j]='\0';
		}
   		p1 = buf;
   		strncpy(ruref,buf,11);
   		p1 += 20;
   		strncpy(sic,p1,5);
   		p1 = buf;
   		p1 +=26;
   		len = strcspn(p1,"|");
   		namelen = len;
   		strncpy(name,p1,len);
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

 /* old layout is 
    ruref : name : sic : period : comment 
    new layout is 
    ruref | period : sic92 | name | comment | author */

findname()
{
   j=0;
   x=0;
   p1 = buf;				/* point at buffer */
   p1 +=12;				/* point past ruref */
   p2 = p1;
   
   p2 = buf;
   p2 +=26;				/* position at name */
   len = strcspn(p2,"|");		/* find length of name */
   p2 += len;				/* point at comment */
   p2++;
   len = strcspn(p2,"|");		/* find length of comment */

   if (len > 2)				/* ignore empty comments */
   {
   	strncpy(period,p1,6);		/* copy in current period */
   	strncat(comment,period,6);	
   	strcat(comment,"  ");		/* comment with 2 spaces */
   	p1 = buf;
   	p1 +=26;			/* position at name */
   	len = strcspn(p1,"|");		/* find length of name */
   	p1 += len;			/* point at comment */
   	p1++;
   	len = strcspn(p1,"|");		/* find length of comment */
   	strncat(comment,p1,len);	/* append comment */
   	p1 += len;			/* point at author */
   	p1++;
   	strcat(comment,"   ");
   	strcat(comment,p1);		/* append author */
    }
  return 0;
}


 /***************************   read1  ******************************/

 read1()
 {
  for(j=0;j<2000;j++)
  {
	buf[j]='\0';
  }
  fgets(buf,2000,inp);
  if (feof(inp) != 0)
  {
     fprintf(stderr,"End of file %s\n",input);
     return 0;
  }
  cnt1++;
  return 0;
 }
