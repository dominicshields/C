/**************************************************************************
PROGRAM NAME	: okpr  (source okpr.c)
PURPOSE	: Change a file's output to look nice when printed
PARAMETERS	:
OUTPUT		:
COMMENTS	: none
**************************************************************************/
#include <stdio.h>
#include <string.h>
#include <time.h>

#define RECLEN 2000	/* max record length of input file  */

FILE *inp;			/* input file */
FILE *outp;		/* output file */

char *p1;
char *input;
char *output;
char bufin[RECLEN];		/* buffer area to read input file */
char bufout[RECLEN];		/* buffer area to put output file */
int pos[FSELNO];
int len[FSELNO];
int cnt1, cnt2;
time_t tim;

main(argc,argv)
int argc;
char *argv[];
{
   int reply, i, x;
   if(argc < 5)
   {
     fprintf(stderr,"Error, usage is fsel <input file> <output file> <fieldpos1> <fieldlength1> .... <fieldposn> <fieldlengthn> \n");
      exit(1);
   }

/* tim = time(NULL);
   fprintf(stderr,"Program Ends OK  %s\n",ctime(&tim)); */

   input = argv[1];	/* input filename assigned to pointer */
   output = argv[2];	/* output filename assigned to pointer */

   for(i=3,x=0;i<argc;i++,x++)
   {
     (pos[x]) = atoi(argv[i]);
     pos[x]--;
     i++;
     (len[x]) = atoi(argv[i]);
   }

   if ((inp = fopen(input,"r")) == NULL)
   {
      fprintf(stderr,"Error opening %s does it exist?\n",input);
      exit(1);
   }

   if ((outp = fopen(output,"w")) == NULL)
   {
      fprintf(stderr,"Error opening %s \n",output);
      exit(1);
   }

   cnt1=0;
   cnt2=0;

   read1();

   while (feof(inp) == 0)
   {

     for(i=3,x=0;i<argc;i=i+2,x++)
     {
       p1 = bufin;
       p1 = p1 + pos[x];
       if(x == 0)
       {
        strncpy(bufout,p1,len[x]);
       }
       else
       {
        strncat(bufout,p1,len[x]);
       }
     }

     fprintf(outp,"%s\n",bufout);
     cnt2++;

     for(x=0;x<RECLEN;x++)
     {
      bufout[x] = '\0';
     }

     read1();
   }

   fclose(inp);
   fclose(outp);

   fprintf(stderr,"%d records read %d records written to %s\n",cnt1,cnt2,output); 

   return 0;

 }

 /***************************   read1  ******************************/

 read1()
 {
  *fgets(bufin,RECLEN,inp);
  if (feof(inp) != 0)
  {
   return 0;
  }
  cnt1++;
  return 0;
 }
