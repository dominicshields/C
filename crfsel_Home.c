/**************************************************************************
PROGRAM NAME : crfsel  (source crfsel.c)
PURPOSE      : copies the records specified (by line number) from a file
               to a named output file. Can also do the same thing as a VME FSEL
               or a UNIX cut -c however differs from cut -c in that it can deal
               with many byte ranges.
PARAMETERS   : inputfile outputfile startrec endrec (defaults to EOF).
               if startrec is negative, the program copies this number of
               records from the end of the file - similar to Unix Tail.
               To do the optional FSEL start at parameter 5 with
               fieldpos1 fieldlength1 up to fieldposn fieldlengthn
               a fieldlength of zero means copy until end of record.
OUTPUT       : file containing records asked for.
             : counts to confirm number copied.
COPYRIGHT    : this program is based on VME's COPY RECORDS command.
COMMENTS     :
WRITTEN BY   : Dominic 20/12/2013 18:45:42
**************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define RECLEN 5000     /* Max record length of input file  */
#define FSELNO 10       /* Max number of fsels              */

                      /* file pointers */
FILE *inp;            /* input file    */
FILE *out;            /* output file   */

char *input;
char *output;
char *p1;

char bufin[RECLEN];     /* buffer area to read input file */
char bufout[RECLEN];    /* buffer area to put output file */
int pos[FSELNO];
int len[FSELNO];
int argcnt, reply, i, x;
long startrec, endrec, cnt1, cnt2, cnttotal;
time_t tim;

int read1(void);
int writefsel(void);

main(argc,argv)
int argc;
char *argv[];
{
   argcnt = argc;
   if(argc < 5)
   {
     fprintf(stderr,"Error, usage is %s <inputfile> <outputfile> <startrec> <endrec> [up to %d optional column ranges]\n",argv[0],FSELNO);
      exit(1);
   }

   input = argv[1];            /* input filename assigned to pointer */
   output = argv[2];           /* output filename assigned to pointer */
   startrec = atoi(argv[3]);
   endrec = atoi(argv[4]);
/* Now get the optional fsel parameters */
   for(i=5,x=0;i<argc;i++,x++)
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

   if ((out = fopen(output,"w")) == NULL)
   {
      fprintf(stderr,"Error opening %s\n",output);
      exit(1);
   }

   if (endrec == 0)
   {
     endrec = 2000000000;
   }

   if (startrec < 0)    /* IF BLOCK ADDED DJS 04/02/2013 10:49:16 */
   {
    fprintf(stderr,"startrec = %d so copying this number from EOF\n",startrec);
    /* Have to count all the records first if we want to copy from end of file which is what the negative parameter does */
      cnttotal = 0;
      while ( feof(inp) == 0 )
      {
        read1();
        cnttotal++;
      }
      fclose(inp);           /* CLOSE AND RE-OPEN FILE OR THE POINTERS WILL BE VERY CONFUSED FROM EXPERIENCE */
      inp = fopen(input,"r");
      startrec = cnttotal + startrec;   /* ADDING AS STARTREC IS NEGATIVE - THINK ABOUT IT  */
      if(endrec < 0)
      {
      	endrec = cnttotal + endrec; 		/* ADDING AS STARTREC IS NEGATIVE - THINK ABOUT IT 01/01/2014 14:49:04 */
      }												/* A COUNTING ISSUE ARISES IF THE LAST LINE IS NEWLINE TERMINATED BEFORE EOF AS IS COMMON http://en.wikipedia.org/wiki/Newline */
   }

   cnt1=0;
   cnt2=0;

   read1();
   while (( feof(inp) == 0 ) && cnt1 <= endrec)
   {
    if(startrec <= cnt1)
    {
      if(argc < 6)      /* No FSEL parameters so write entire record */
      {
        fputs(bufin,out);
        cnt2++;
      }
      else
      {
        writefsel();
      }
    }
    read1();
   }

  fclose(inp);
  fclose(out);
  fprintf(stdout,"%ld records copied from %s to %s\n",cnt2,input,output);
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
/***************************   writefsel  ******************************/
 writefsel()
 {
      for(i=5,x=0;i<argcnt;i=i+2,x++)
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

/*     fputs(bufout,out);*/
    fprintf(out,"%s",bufout);
    cnt2++;

    for(x=0;x<RECLEN;x++)
    {
      bufout[x] = '\0';    /* DEAL WITH VARIABLE LENGTH RECORDS THOROUGHLY */
    }
    return 0;
 }
