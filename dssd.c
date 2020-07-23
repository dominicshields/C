/**************************************************************************
PROGRAM NAME : dssd  (source dssd.c)
**************************************************************************/
#include <string.h>
#include <stdio.h>
#include <time.h>

FILE *inp;		/* input file */
FILE *out;		/* output file */

char ruref[12];

char *p1;
char *p2;
char *periodptr;
char *sicptr;
char *conameptr;
char *commentptr;
char *input;
char *output;

char name[80];
char sic[7];
char period[7];
char buf[2000];		/* buffer area to read input file */
char buf2[10000];	/* buffer area to hold joined file */
char comment[10000];
int reply;
int cnt1, cnt2, i, j, l, x, len, newflag,namelen;
int lenper, lensic, lenconame, lencom;
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
   p1 += 12;
   lenper = strcspn(p1,"|");		/* find length of period */
   p1 += lenper+1;			/* point past period */
   lensic = strcspn(p1,"|");		/* find length of sic92 */
   if(lensic == 6)
   {
	p1++;
	strncpy(sic,p1,5);
	sic[5]='\0';
	p1 += lensic;
   }
   else
   {
	strncpy(sic,p1,lensic);		/* copy in sic92 */
	sic[lensic]='\0';
	p1 += lensic+1;			/* point past sic92 */
   }
   lenconame = strcspn(p1,"|");		/* find length of company name */
   strncpy(name,p1,lenconame);		/* copy in company name */
   name[lenconame]='\0';

   while (feof(inp) == 0)
   {
	if(strncmp(ruref,buf,11) != 0) /* checks for new ruref */
	{
   		newflag=0;
		strncpy(buf2,ruref,11);
		strcat(buf2,"|");
		strncat(buf2,name,lenconame);
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
		p1 += 12;
		lenper = strcspn(p1,"|");	/* find length of period */
		p1 += lenper+1;			/* point past period */
		lensic = strcspn(p1,"|");	/* find length of sic92 */
		if(lensic == 6)
		{
			p1++;
			strncpy(sic,p1,5);
			sic[5]='\0';
			p1 += lensic;
		}
		else
		{
			strncpy(sic,p1,lensic);		/* copy in sic92 */
			sic[lensic]='\0';
			p1 += lensic+1;			/* point past sic92 */
		}
		lenconame = strcspn(p1,"|");	/* find length of company name */
		strncpy(name,p1,lenconame);	/* copy in company name */
		name[lenconame]='\0';
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

 /*  ruref | period | sic92 | company name | comment | author */

findname()
{
   j=0;
   x=0;
   p1 = buf;				/* point at buffer */
   p1 +=12;				/* point past ruref */
   periodptr = p1;				
   lenper = strcspn(p1,"|");		/* find length of period */
   p1 += lenper+1;			/* point past period */
   sicptr = p1;				
   lensic = strcspn(p1,"|");		/* find length of sic92 */
   p1 += lensic+1;			/* point past sic92 */
   conameptr = p1;			
   lenconame = strcspn(p1,"|");		/* find length of company name */
   p1 += lenconame+1;			/* point past company name */
   commentptr = p1;
   lencom = strcspn(p1,"|");		/* find length of comment */
   p1 += lencom+1;	
   
   if (lencom > 2)			/* ignore empty comments */
   {
   	strncpy(period,periodptr,lenper);	/* copy in current period */
	period[lenper]='\0';
	strncat(comment,period,lenper);
	strcat(comment,"  ");			/* pad comment with 2 spaces */
   	strncat(comment,commentptr,lencom);	/* copy in current comment */
	strcat(comment,"  ");			/* pad comment with 2 spaces */
   	strcat(comment,p1);			/* append author */
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
