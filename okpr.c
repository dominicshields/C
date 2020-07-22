/**************************************************************************
PROGRAM NAME	: okpr  (source okpr.c)
PURPOSE	: Change a file's output to look nice when printed
PARAMETERS	: <Filename> [Header Flag] (optional)
OUTPUT		: print with numbers
COMMENTS	: none
AUTHOR		: Dominic 8/9/97
**************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define RECLEN 2000	/* max record length of input file  */

FILE *inp;			/* input file */
FILE *outp;		/* output file */

char *p1;
char *input;
char *output;
char bufin[RECLEN];		/* buffer area to read input file */
char syscomm[80];
int cnt1;
time_t tim;

main(argc,argv)
int argc;
char *argv[];
{
   if(argc < 2)
   {
     fprintf(stderr,"Error, usage is okpr <input file> [T] <- (for optional header)\n");
     fprintf(stderr,"Dominic - 8/9/97\n");
     exit(1);
   }

   tim = time(NULL);
  
   input = argv[1];	/* input filename assigned to pointer */
 
   if ((inp = fopen(input,"r")) == NULL)
   {
      fprintf(stderr,"Error opening %s does it exist?\n",input);
      exit(1);
   }

   if ((outp = fopen("okpr.txt","w")) == NULL)
   {
      fprintf(stderr,"Error opening %s \n",output);
      exit(1);
   } 

   p1 = argv[2];
   *p1 = toupper(*p1);
  
   if(strncmp(p1,"T",1) == 0)
   {
	fprintf(outp,"\tFile %s \tPrinted at  %s\n\n",argv[1],ctime(&tim));
   } 

   read1();
   while (feof(inp) == 0)
   {
	fprintf(outp,"%d\t%s",cnt1,bufin);
	read1();
   }

   fclose(inp);
   fclose(outp);
   strcpy(syscomm,"PRINT /d:lpt1 okpr.txt");
   system(syscomm);
   return 0;
 }

 /***************************   read1  ******************************/

 read1()
 {
  fgets(bufin,RECLEN,inp);
  if (feof(inp) != 0)
  {
   return 0;
  }
  cnt1++;
  return 0;
 }
