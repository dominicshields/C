/**************************************************************************
PROGRAM NAME : domcsplit  (source domcsplit.c)
PURPOSE      : splits files by pattern.
PARAMETERS   :
WRITTEN BY   : Dominic
**************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <dos.h>
#include <dir.h>
#include <time.h>

#define RECLEN 9000
#define DOMNO 50

FILE *inp;		/* input file */
FILE *out;            /* output file   */
FILE *ptr;		/* variable file pointer */

char *p1;
char *p2;
char *input;
char output[DOMNO];
char tablename[DOMNO];
char tabfilename[DOMNO];
char buf[RECLEN];	/* buffer area to read input file */
char *suffix;
char *pattern;	/* holds match string */
int reply;
long cnt1;
int i;
int x;
int patternlen;
int filecnt;
int rflag=0;
int params=0;
time_t tim;

int read1(void);
int openout(void);

main(argc,argv)
int argc;
char *argv[];
{
   if(argc < 3)
   {
     fprintf(stderr,"Error, usage is domcsplit suffix filename pattern \n");
      exit(1);
   }

	tim = time(NULL);
   fprintf(stderr,"Program Begins  %s\n",ctime(&tim));

	filecnt = 0;
  cnt1=0;

	suffix = argv[1];
  input = argv[2];	/* fileassigned to pointer */
  pattern = argv[3];
  patternlen = strlen(pattern);

  fprintf(stderr,"suffix = %s, Input File = %s, Pattern = %s\n",suffix,input,pattern);

	if ((inp = fopen(input,"r")) == NULL)
	{
	  fprintf(stderr,"Error opening %s does it exist?\n",input);
	  exit(1);
	}

	ptr = inp;  /* set variable file pointer to input file pointer */

   read1();
   openout();

   while (feof(ptr) == 0)
   {
			reply = strncmp(p1,pattern,patternlen);
			if (reply == 0)   /* Found Match so close last file and create a new one and write current line to it*/
			{
				   fclose(out);
				   filecnt++;
				   openout();
				   fputs(buf,out);
			}
			else
			{
					fputs(buf,out);
			}

     read1();
   }

   fclose(inp);
   fclose(out);


   fprintf(stderr,"Records read from %s = %d  files written = %d\n",input,cnt1,filecnt);

   tim = time(NULL);
   fprintf(stderr,"Program Ends OK  %s\n",ctime(&tim));

   return 0;

 }

 /***************************   read1  ******************************/

 read1()
 {

  fgets(buf,RECLEN,ptr);
  if (feof(ptr) != 0)
  {
     return 0;
  }
	p1 = buf;
  cnt1++;
  return 0;
 }

 openout()
 {

	p1 += patternlen;
	strcpy(tablename,p1);
	if (tablename[0] == '\0')
	{
		strcpy(tablename,"Init File");
	}
	else
	{
		tabfilename[0] = '\0';
		p2 = tablename;
		x = 0;
  	for(i=0;i<strlen(tablename);i++,p2++)
  	{
  		if(tablename[i] == '`' || tablename[i] == ' ' )
  		{
  		}
  		else
  		{
  			strncat(tabfilename,p2,1);
  			x++;
  		}
  	}
  		tabfilename[x-1] = '\0';
  		printf("tabfilename = %s\n",tabfilename);
	}
	
	sprintf(output,"%s%s",tabfilename,suffix);
		
  if ((out = fopen(output,"w")) == NULL)
   {
      fprintf(stderr,"Error opening %s\n",output);
      exit(1);
   }

    return 0;
 }
