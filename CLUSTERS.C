/**************************************************************************
PROGRAM NAME : clusters (source clusters.c)
PURPOSE      : 
WRITTEN BY   : Dominic
**************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define RECLEN 100

FILE *inp;

char *p1;
char *p2;
char *input;
char unixcmd[RECLEN];
char buf[RECLEN];
char hold[RECLEN];
char hold2[RECLEN];
int i, j, len;
unsigned long wastecnt, bytes, waste, waste_temp, tot, cluster;
time_t tim;

main(argc,argv)
int argc;
char *argv[];
{
   if(argc < 1)
   {
     fprintf(stderr,"Error, usage is clusters\n");
      exit(1);
   }

   tim = time(NULL);
   fprintf(stderr,"\nRunning on Drive %s\n",ctime(&tim));

   strcpy(unixcmd,"cd \\");
   system(unixcmd);

   strcpy(unixcmd,"chkdsk > domchk");
   system(unixcmd);
   
   input = "domchk";
   if ((inp = fopen(input,"r")) == NULL)
   {
       fprintf(stderr,"Error opening %s\n",input);
       exit(1);
   }

   read1();

   while (feof(inp) == 0)
   {
	for(i=0;i<strlen(buf);i++)
	{
		if(strncmp(p1,"total disk",10) == 0)
		{ 
			while(p2[0]==' ')
			{
				p2++;
			}
			
			len = strcspn(p2," ");
			strncpy(hold,p2,len);
			hold[len]='\0';
			p2=hold;
			for(j=0;j<(strlen(hold));j++)
			{
				if(strncmp(p2,",",1) != 0)
				{
					strncat(hold2,p2,1);
				}
				p2++;
			
			}
			hold2[j]='\0';	
			tot = atol(hold2);
		}

		if(strncmp(p1,"each",4) == 0)
		{ 
			while(p2[0]==' ')
			{
				p2++;
			}
			
			len = strcspn(p2," ");
			strncpy(hold,p2,len);
			hold[len]='\0';
			p2=hold;
			for(j=0;j<(strlen(hold));j++)
			{
				if(strncmp(p2,",",1) != 0)
				{
					strncat(hold2,p2,1);
				}
				p2++;
			
			}
			hold2[j]='\0';	
			cluster = atol(hold2);
		}
		p1++;
	}

   	read1();
   }

   fclose(inp);

   strcpy(unixcmd,"dir /a /s > domchk");
   system(unixcmd);
   
   input = "domchk";
   if ((inp = fopen(input,"r")) == NULL)
   {
       fprintf(stderr,"Error opening %s\n",input);
       exit(1);
   }

   read1();
   while (feof(inp) == 0)
   {
	for(i=0;i<strlen(buf);i++)
	{
		if(strncmp(p1,"<DIR>",5) == 0)
		{ 
			goto lab;
		}
		if(strncmp(p1,"/",1) == 0)
		{ 
			p1=buf;
			p1 += 11;
			while(p1[0]==' ')
			{
				p1++;
			}
			len = strcspn(p1," ");
			strncpy(hold,p1,len);
			hold[len]='\0';
			p2=hold;
			for(j=0;j<(strlen(hold));j++)
			{
				if(strncmp(p2,",",1) != 0)
				{
					strncat(hold2,p2,1);
				}
				p2++;
			
			}
			hold2[j]='\0';	
			bytes = atol(hold2);
			if(bytes < cluster)
			{
				waste += (cluster - bytes);
				wastecnt++;
				goto lab;
			}
			else
			{
				waste_temp = (bytes % cluster);	
				if(waste_temp > 0)
				{
					waste += waste_temp;
					wastecnt++;
fprintf(stdout,"waste_temp = %ld, wastecnt = %ld\n",waste_temp,wastecnt);
					goto lab;
				}
			}
		}
		p1++;
	}
	lab:
   	read1();
   }

   fprintf(stderr,"Capacity of Disk in Megabytes = %ld (1 Megabyte = %ld bytes)\n",tot/1024/1024,(long)1024*1024);
   fprintf(stderr,"Cluster size = %ld bytes\n",cluster);
   fprintf(stderr,"Bytes Wasted = %ld bytes\n",waste);
   fprintf(stderr,"No of files containing waste = %ld\n",wastecnt);

   fclose(inp);

   tim = time(NULL);
   fprintf(stderr,"\nProgram Ends OK  %s\n",ctime(&tim));

   return 0;

 }

 /***************************   read1  ******************************/

 read1()
 {

  fgets(buf,RECLEN,inp);
  if (feof(inp) != 0)
  {
     return 0;
  }
  p1=buf;
  p2=buf;
  hold[0]='\0';
  hold2[0]='\0';

  return 0;
 }
