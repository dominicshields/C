/*
Sat Aug 18 2001  
Charlton  v  Everton  15:00 
 
 
Derby  v  Blackburn  15:00 
 
 
Leeds  v  Southampton  15:00 

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
char rawdate[20];
char properdate[20];
char month[3];
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
   	printf("buf = %s",buf);
	if(strlen(buf) > 2)
	{
		if(strncmp(buf,"Sat ",4) == 0 || strncmp(buf,"Sun ",4) == 0 || strncmp(buf,"Mon ",4) == 0 ||
		strncmp(buf,"Tue ",4) == 0 ||strncmp(buf,"Wed ",4) == 0)
		{
			formatdate();
		}
		else
		{
			p1=buf;
			p2=buf;
			len = 0;
			while(strncmp(p2,"  v",3) != 0)
			{
				p2++;
				len++;
			}
			strncpy(bufout,p1,len);
			strcat(bufout,",");
			p1 += len + 5;
			p2=p1;
			len = 0;
			while(strncmp(p2,"  ",2) != 0)
			{
				p2++;
				len++;
			}
			strncat(bufout,p1,len);
			strcat(bufout,",");
			p1 += len + 2;
			strncat(bufout,p1,5);
			strcat(bufout,",");

			strncat(bufout,rawdate,strlen(rawdate) -1);
			strcat(bufout,",");
			strcat(bufout,properdate);
			
			fprintf(outp,"%s",bufout);
			for(x=0;x<80;x++)
			{
				bufout[x]='\0';
			}
		}

	
	}
	read1();
   }

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
Sat Aug 18 2001      */
 formatdate()
 {
	strcpy(rawdate,buf);
	p1 = buf;
	p1 +=4;
	
	if(strncmp(p1,"Aug",3) == 0)
	{
		strcpy(month,"08");
	}
	if(strncmp(p1,"Sep",3) == 0)
	{
		strcpy(month,"09");
	}
	if(strncmp(p1,"Oct",3) == 0)
	{
		strcpy(month,"10");
	}
	if(strncmp(p1,"Nov",3) == 0)
	{
		strcpy(month,"11");
	}
	if(strncmp(p1,"Dec",3) == 0)
	{
		strcpy(month,"12");
	}
	if(strncmp(p1,"Jan",3) == 0)
	{
		strcpy(month,"01");
	}
	if(strncmp(p1,"Feb",3) == 0)
	{
		strcpy(month,"02");
	}
	if(strncmp(p1,"Mar",3) == 0)
	{
		strcpy(month,"03");
	}
	if(strncmp(p1,"Apr",3) == 0)
	{
		strcpy(month,"04");
	}
	if(strncmp(p1,"May",3) == 0)
	{
		strcpy(month,"05");
	}
	
	p1 +=4;
	for(x=0;x<20;x++)
  	{ 
      		properdate[x]='\0';
  	}
	strncpy(properdate,p1,2);
	strcat(properdate,"/");
	strcat(properdate,month);
	strcat(properdate,"/");
	p1 +=3;
	strcat(properdate,p1);
	
printf("properdate = %s",properdate);
	return(0);
 }
