/*************************************************************************
PROGRAM NAME : discwipe  (source discwipe.c)
PURPOSE      : Stops deleted files being recovered

PARAMETERS   :
OUTPUT       :
**************************************************************************/
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <stdlib.h>

FILE *outfil_ptr;

time_t tim;

main()
{
   int reply;
   long cnt;
   cnt=0;

   tim = time(NULL);
   fprintf(stderr,"\nProgram Begins  %s\n",ctime(&tim));

   if ((outfil_ptr = fopen("a:\wipefile","a")) == NULL)
   {
      fprintf(stderr,"Error opening output file\n");
      exit(1);
   }

   while (!kbhit())
   {
    fprintf(outfil_ptr,"YOU NOSY BUGGER!\n");
    cnt++;
   }

  fclose(outfil_ptr);


  fprintf(stdout,"Records written to wipefile = %ld\n",cnt);
  fprintf(stdout,"Size of output              = %ld KB\n",(cnt * 17)/1024);
  tim = time(NULL);
  fprintf(stderr,"Program Ends OK  %s\n",ctime(&tim));

  return 0;

 }


