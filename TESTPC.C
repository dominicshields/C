/* testpc.c */
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
		     /* file pointers */
FILE *inp;           /* input file    */
FILE *out;           /* OK file       */

char *p1;
char *input;

char buf[500];       /* buffer area to read input file */

int pos;
int reclen;
int spacepos;
int error;
long cnt1=0;
long cnt2=0;
long cnt3=0;
time_t tim;
			       /* ILLEGAL CHARACTERS */

main(argc,argv)
int argc;
char *argv[];
{
   int reply;

   tim = time(NULL);
   fprintf(stderr,"\nProgram Begins  %s\n",ctime(&tim));

   input = argv[1];            /* filename 1 assigned to pointer */
   pos = *argv[2];
   reclen = *argv[3];

   pos--;
   reclen++;

   if ((inp = fopen(input,"r")) == NULL)
   {
      fprintf(stderr,"Error opening %s does it exist?\n",input);
      exit(1);
   }

   if ((out = fopen("pcodesok","w")) == NULL)
   {
      fprintf(stderr,"Error opening file postcodesok\n");
      exit(1);
   }


   read1();

   while ( (feof(inp) == 0) )
   {
     p1 = buf;
     p1 = p1 + pos;
     writ();
     read1();

   }

  fclose(inp);
  fclose(out);


  fprintf(stdout,"Records read from input file      = %ld\n",cnt1);
  fprintf(stdout,"Number of postcodes OK            = %ld\n",cnt2);


  tim = time(NULL);
  fprintf(stderr,"Program Ends OK  %s\n",ctime(&tim));

  return 1;

 }

  /***************************   writ  ******************************/

  writ()
  {
   int reply;
   reply = fwrite(buf,reclen,1,out);
   if (reply != 1)
   {
    fprintf(stderr,"Error writing to file postcodesok\n");
    exit(1);
   }
   cnt2++;
   return 1;
  }
  /***************************   read1  ******************************/

 read1()
 {
  int reply;

  reply = fread(buf,reclen,1,inp);
  if (feof(inp) != 0)
  {
   fprintf(stderr,"End of file %s\n",input);
   return 0;
  }
  cnt1++;

  if (reply != 1)
  {
   fprintf(stderr,"Error reading %s\n",input);
   exit(1);
  }
  return 1;
 }


