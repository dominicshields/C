/**************************************************************************
PROGRAM NAME : cr  (source cr.c)
PURPOSE      : copies the records specified (by line number) from a file
	       to a named output file, big deal but I don't know how to do
	       this with ONE command in unix. For instance tail will only
	       work up to 4KB from the end of a file, split involves
	       tedious multiple commands, cp only copies whole files
	       (as far as I know), there's a size limit with vi,
	       head only gets you lines from the front and grepping,
	       fgrepping etc is not feasible if there's nothing different
	       about the records you want to copy.
PARAMETERS   : inputfile outputfile startrec endrec (defaults to EOF).
OUTPUT       : file containing records asked for.
	       counts to confirm number copied.
WARNING      : don't bother to bore me by telling me that if you split a
	       100 MB file into 500 files then cut them, grep them, paste
	       them, sort them etc you'll get exactly the same result as this
	       program because I'm not interested.
COPYRIGHT    : this program is based on VME's COPY RECORDS command.
COMMENTS     :
**************************************************************************/
#include <string.h>
#include <stdio.h>
		      /* file pointers */
FILE *inp;            /* input file    */
FILE *out;            /* output file   */

char *input;
char *output;
char *quiet;

char buf[1000];       /* buffer area to read input file */

long startrec, endrec, cnt1, cnt2;

main(argc,argv)
int argc;
char *argv[];
{
   if(argc < 4)
   {
     fprintf(stderr,"Error, usage is cr <inputfile> <outputfile> <startrec> <endrec> <q>\n");
      exit(1);
   }

   input = argv[1];            /* input filename assigned to pointer */
   output = argv[2];           /* output filename assigned to pointer */
   startrec = atoi(argv[3]);
   endrec = atoi(argv[4]);

   if(argc == 6)
   {
	quiet = argv[5];
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

   cnt1=0;
   cnt2=0;

   *fgets(buf,1000,inp);
   cnt1++;

   while (( feof(inp) == 0 ) && cnt1 <= endrec)  
   {

    if(startrec <= cnt1)     
    {
     fputs(buf,out);
     cnt2++;
    }

    *fgets(buf,1000,inp);
    cnt1++;

   }

  fclose(inp);
  fclose(out);

  if(*quiet != 'q')
  {
  	fprintf(stdout,"%ld records copied from %s to %s\n",cnt2,input,output);
  }

  return 0;

 }

