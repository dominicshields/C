/* premfix - Reformat Premiership fixtures list 2003/2004 12/08/2003
revised to deal with 2004/2005 list 08/08/2004 15:44
Input data form
DD/MM/YYYY,
Birmingham,Tottenham Hotspur,12:00
Portsmouth,Aston Villa,12:30
Leicester,Southampton,15:00
Manchester United,Bolton,15:00
Arsenal,Everton,15:00
Blackburn,Wolves,15:00
Fulham,Middlesbrough,15:00
DD/MM/YYYY,
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

FILE *inp;		/* input file */
FILE *outp;		/* variable file pointer */

char *input;
char *p1;
char *p2;
char buf[80];
char bufout[80];
char properdate[12];
char day[4];
char month[4];
char year[6];
int i,x,len;

main(argc,argv)
int argc;
char *argv[];
{
   input = argv[1];
   if ((inp = fopen(input,"r")) == NULL)
   {
	  fprintf(stderr,"Error opening %s does it exist?\n",input);
	  exit(1);
   }

   if ((outp = fopen("fixtures.txt","w")) == NULL)
   {
	  fprintf(stderr,"Error writing fixtures.txt\n");
	  exit(1);
   }
   
   read1();

   while (feof(inp) == 0)
   {
/*printf("buf = %s",buf);*/
	p1=buf;
	
	if(strlen(buf) == 12)
	{
		formatdate();
	}
	else
	{
		strcpy(bufout,properdate);
		strcat(bufout,p1);
		fprintf(outp,"%s",bufout);
		for(x=0;x<80;x++)
		{
			bufout[x]='\0';
		}
	}
	read1();
   }

	strcpy(bufout,properdate);
	strcat(bufout,p1);
	fprintf(outp,"%s",bufout);
	fclose(inp);
   
   return 0;

 }

 /***************************   read1  ******************************/

 read1()
 {
  for(x=0;x<80;x++)
  { 
      buf[x]='\0';
  }
  fgets(buf,80,inp);
  if (feof(inp) != 0)
  {
     fprintf(stderr,"End of file %s\n",input);
     return 0;
  }
  return 0;
 }
 
  /***************************   formatdate  *****************************
08/16/2003,      */
 formatdate()
 {
 	for(x=0;x<12;x++)
  	{ 
      		properdate[x]='\0';
  	}
  	for(x=0;x<3;x++)
  	{ 
      		month[x]='\0';
      		day[x]='\0';
  	}
  	for(x=0;x<5;x++)
  	{ 
      		year[x]='\0';
  	}
 	strncpy(day,p1,3);
	month[3] = '\0';
	p1 +=3;
	strncpy(month,p1,3);
	day[3] = '\0';
	p1 +=3;
	strcpy(year,p1);
	year[5] = '\0';
	
	strcpy(properdate,day);
	strcat(properdate,month);
	strcat(properdate,year);
	
printf("properdate = %s\n",properdate);
	return(0);
 }
