/**************************************************************************
PROGRAM NAME : aprdate  (source aprdate.c)
PURPOSE      : copies or appends the records specified (by line number)
	       from a file to a named output file, if this exists, the
	       records are appended to it, otherwise the output file is
	       created.
PARAMETERS   : inputfile startrec endrec (defaults to EOF).
OUTPUT       : file containing records asked for.
	       counts to confirm number copied/appended.
Updated 12/10/2011 18:41: The LEN constant had to be increased from 1000
	                        to 5000 to be consistent with the matches program
	                        Dominic
**************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dos.h>

#define LEN 5000

		      /* file pointers */
FILE *inp;            /* input file    */
FILE *out;            /* output file   */

char *input;
char *output;

char buf[LEN];       /* buffer area to read input file */
char mode[9];
char datechar[9];
char outfile[80];

time_t tim;
struct tm *tm;

long startrec, endrec, cnt1, cnt2;

main(argc,argv)
int argc;
char *argv[];
{
   if(argc < 3)
   {
     fprintf(stderr,"Error, usage is aprdate <inputfile> <outfilestub> <startrec> <endrec> \n");
      exit(1);
   }

   input = argv[1];            /* input filename assigned to pointer */
   output = argv[2];     
   startrec = atoi(argv[3]);
   endrec = atoi(argv[4]);

   if ((inp = fopen(input,"r")) == NULL)
   {
      fprintf(stderr,"Error opening %s does it exist?\n",input);
      exit(1);
   }

   tim = time(NULL);
   tm = localtime(&tim);
   strftime(datechar,100,"%Y%m%d",tm);   
   
   strcpy(outfile,output);
   strcat(outfile,datechar);

   if ((out = fopen(outfile,"r")) == NULL)
   {
      fprintf(stderr,"Target file %s does not exist, aprdate will copy records\n",outfile);
      strcpy(mode,"Copied");
   }
   else 
   {
      fprintf(stderr,"Target file %s exists, aprdate will append records\n",outfile);
      strcpy(mode,"Appended");
   }

   if ((out = fopen(outfile,"a")) == NULL)
   {
      fprintf(stderr,"Error opening %s\n",outfile);
      exit(1);
   }

   if (endrec == 0)
   {
     endrec = 2000000000;
   }

   cnt1=0;
   cnt2=0;

   fgets(buf,LEN,inp);
   cnt1++;

   while (( feof(inp) == 0 ) && cnt1 <= endrec) 
   {

    if(startrec <= cnt1)
    {
     fputs(buf,out);
     cnt2++;
    }

    fgets(buf,LEN,inp);
    cnt1++;

   }

  fclose(inp);
  fclose(out);

  fprintf(stdout,"%ld records %s from %s to %s\n",cnt2,mode,input,outfile);

  return 0;

 }

