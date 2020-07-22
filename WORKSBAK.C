/**************************************************************************
PROGRAM NAME : worksbak  (source worksbak.c)
PURPOSE      : 
PARAMETERS   :
OUTPUT       : 
COMMENTS     : 
**************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dos.h>
#include <dir.h>

#define RECLEN 500

FILE *infil_ptr;

char syscall[RECLEN];
char buf[RECLEN];
char *p1;
char datechar[9];
char dirnam[9];
char monday[5];
char fiddle[5];
char fiddle2[3];
long dateint;
long dirint;
int yearint;
int monint;
int mondayint;
int dayint;
int reply, dateval;
time_t tim;
struct date d;

main(argc,argv)
int argc;
char *argv[];
{
   if(argc < 2)
   {
     fprintf(stdout,"Error, usage is worksbak <deltype .. 1 = weekly, 2 = monthly>\n");
     fprintf(stdout,"Note : This program relies on pkzip and rm being in C:\DOS\n");
     exit(1);
   }

   if(atoi(argv[1]) == 1)
   {
	dateval = 7;
   }
   else
   {
	dateval = 31;
   }

   getdate(&d);
   sprintf(datechar,"%d%02d%02d",d.da_year,d.da_mon,d.da_day);
   dateint=atol(datechar);

   reply = mkdir(datechar);
   if(reply != 0)
   {
	fprintf(stdout,"Backup directory %s already exists ... exiting\n",datechar);
        exit(1);
   }

   reply = chdir(datechar);
   if(reply != 0)
   {
	fprintf(stdout,"Can't change to directory %s ... exiting\n",datechar);
        exit(1);
   }

   sprintf(syscall,"C:\\DOS\\PKZIP %s.ZIP C:\\MSWORKS\\*.WPS",datechar);
   system(syscall);

   reply = chdir("..");
   if(reply != 0)
   {
	fprintf(stdout,"Can't change to root directory ... exiting\n");
        exit(1);
   }

   strcpy(syscall,"DIR 199????? /A:D > zzzzzzzz");
   system(syscall);

   if ((infil_ptr = fopen("zzzzzzzz","r")) == NULL)
   {
      fprintf(stdout,"Error opening zzzzzzzz\n");
      exit(1);
   }

   fgets(buf,500,infil_ptr);
   
   datecalc();
   fprintf(stdout,"Deleting directories older than %ld\n",dateint);

   while(feof(infil_ptr) == 0)
   {
	if(strncmp(buf,"199",3) == 0)
	{
		strncpy(dirnam,buf,8);	
		dirnam[8]='\0';
		dirint=atol(dirnam);
	
		if(dirint < dateint)
		{
   			sprintf(syscall,"rm -f -r %s",dirnam);
   			reply = system(syscall);
			if(reply != 0)
			{
				fprintf(stdout,"Can't delete directory %s ... exiting\n",dirnam);
				exit(1);
			}
		}
	}
   	fgets(buf,500,infil_ptr);
   }

   reply = remove("zzzzzzzz");
   if(reply != 0)
   {
	fprintf(stdout,"Error deleting zzzzzzzz\n");
   } 

   tim = time(NULL);
   fprintf(stdout,"Program Ends OK  %s\n",ctime(&tim));

   return 0;

}

datecalc()
{
   sprintf(monday,"%02d%02d",d.da_mon,d.da_day);
   yearint = d.da_year;
   monint = d.da_mon;
   mondayint = atoi(monday);
   dayint = d.da_day;

   if(dateval == 7)
   {
	if(dayint > dateval)
	{
		dateint -= dateval;
	}		
	else
	{
		switch(monint)
		{
			case 1 : yearint--,monint=12,mondayint -=75;break;
			case 2 : mondayint -=75;break;
			case 3 : mondayint -=78;break;
			case 4 : mondayint -=75;break;
			case 5 : mondayint -=76;break;
			case 6 : mondayint -=75;break;
			case 7 : mondayint -=76;break;
			case 8 : mondayint -=75;break;
			case 9 : mondayint -=75;break;
			case 10 : mondayint -=76;break;
			case 11 : mondayint -=75;break;
			case 12 : monint--,mondayint -=76;break;
		}
		if(monint == 12)
		{
			sprintf(fiddle,"%04d",mondayint);
			p1=fiddle;
			p1 += 2;
			strcpy(fiddle2,p1);
			sprintf(datechar,"%d%02d%s",yearint,monint,fiddle2);
			dateint=atol(datechar);
		}
		else
		{
			sprintf(datechar,"%d%04d",yearint,mondayint);
			dateint=atol(datechar);
		}
	}
	return(0);
   }

   if(dateval == 31)
   {
	if(monint > 1)
	{
		dateint -= 100;
	}		
	else
	{
		dateint -= 8900;
	}

   }	
   return(0);
}
