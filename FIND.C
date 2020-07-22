/**************************************************************************
PROGRAM NAME : find  (source find.c)
PURPOSE      : finds a given string in a file.
PARAMETERS   : filename reclen fieldpos1 fieldlength string
OUTPUT       : records containing the string
	       counts of the above
COMMENTS     : when working out the record lengths, field positions etc,
	       give the exact values, don't worry about newlines etc.
	       I know grep, egrep and fgrep do the same job but I'm sad.
**************************************************************************/
#include <string.h>
#include <stdio.h>
#include <time.h>

FILE *inp;           /* input file */

char *p1;
char *input;
char *pattern;        /* holds match string */
char buf[500];        /* buffer area to read input file */
int pos, reclen, len;
long cnt1, cnt2;
time_t tim;

main(argc,argv)
int argc;
char *argv[];
{
   int reply;
   char ans[1];
   if(argc < 6)
   {
     fprintf(stderr,"Error, usage is find <filename> <reclen> <fieldpos> <fieldlength> <string>\n");
      exit(1);
   }

   tim = time(NULL);
   fprintf(stderr,"\nProgram Begins  %s\n",ctime(&tim));

   input = argv[1];            /* filename 1 assigned to pointer */
   reclen = atoi(argv[2]);
   pos = atoi(argv[3]);
   len = atoi(argv[4]);
   pattern = argv[5];

   pos--;               /* This is done so that the value can be     */
			 /* used to increment the pointer which will  */
			 /* be pointing at the first byte             */

   reclen++;            /* This is done to include the newline char  */

   if ((inp = fopen(input,"r")) == NULL)
   {
      fprintf(stderr,"Error opening %s does it exist?\n",input);
      exit(1);
   }

   cnt1=0;
   cnt2=0;

   read1();

   while (feof(inp) == 0)
   {
     p1 = buf;
     p1 = p1 + pos;

     reply = strncmp(p1,pattern,len);
     if(reply == 0)
     {
	fprintf(stdout,"%s",buf);
	cnt2++;
     }
     read1();
   }


   fclose(inp);

   fprintf(stdout,"Records read from input file = %ld\n",cnt1);
   fprintf(stdout,"Matching records             = %ld\n",cnt2);
   tim = time(NULL);
   fprintf(stderr,"Program Ends OK  %s\n",ctime(&tim));

   return 0;

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
  if (reply != 1)
  {
   fprintf(stderr,"Error reading %s\n",input);
   exit(1);
  }
  cnt1++;
  return 0;
 }
