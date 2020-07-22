/**************************************************************************
PROGRAM NAME  : fsel  (source fsel.c)
PURPOSE       : Does the same thing as a VME FSEL or a UNIX cut -c
                It differs from cut -c in that it can deal with many byte ranges.
PARAMETERS    : input file output file fieldpos1 fieldlength1 up to
                fieldposn fieldlengthn - a fieldlength of zero means copy until
                end of record.
OUTPUT        : A file cut or FSELd as specified.
COMMENTS      :
CHANGES       : Tidied up some of the scruffy stuff and added the parameter
                value of zero to the fieldlength meaning copy to end of record
                DJS 18/12/2013 16:18:44
**************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define RECLEN 5000     /* max record length of input file  */
#define FSELNO 10       /* max number of fsels              */

FILE *inp;              /* input file */
FILE *outp;             /* output file */

char *p1;
char *input;
char *output;
char bufin[RECLEN];     /* buffer area to read input file */
char bufout[RECLEN];    /* buffer area to put output file */
int pos[FSELNO];
int len[FSELNO];
int cnt1, cnt2;
time_t tim;

int read1(void);

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

   input = argv[1]; /* input filename assigned to pointer */
   output = argv[2];  /* output filename assigned to pointer */

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
       if(x == 0 && len[x] != 0 )  /* FIRST PARAMS - HAVE TO TEST FOR THE DOPEY FIRST LENGTH OF ZERO */
       {
        strncpy(bufout,p1,len[x]);
       }
       else
       {
        /* TEST FOR A LENGTH PARAM OF ZERO MEANING COPY TO END OF RECORD */
        if(len[x] == 0)
        {
          strcat(bufout,p1);
        }
        else
        {
          strncat(bufout,p1,len[x]);
        }
       }
     }

    if(bufout[strlen(bufout)] != '\n' && bufout[strlen(bufout)-1] != '\n')  /* WE DO NOT KNOW IF THE NEWLINE HAS BEEN INCLUDED OR NOT DEPENDING ON PARAMS  */
    {
      bufout[strlen(bufout)] = '\n';           /* SET A NEWLINE IF ABSENT */
    }

    fprintf(outp,"%s",bufout);
    cnt2++;

    for(x=0;x<RECLEN;x++)
    {
      bufout[x] = '\0';    /* DEAL WITH VARIABLE LENGTH RECORDS THOROUGHLY */
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
  fgets(bufin,RECLEN,inp);
  if (feof(inp) != 0)
  {
   return 0;
  }
  cnt1++;
  return 0;
 }
