/**************************************************************************
PROGRAM NAME : cr  (source cr.c)
PURPOSE      : copies the records specified (by line number) from a file
	       			to a named output file, big deal but I don't know how to do
	       			this easily with one command in unix.
PARAMETERS   : inputfile outputfile startrec endrec (defaults to EOF).
OUTPUT       : file containing records asked for.
	           : counts to confirm number copied.
COPYRIGHT    : this program is based on VME's COPY RECORDS command.
COMMENTS     :
WRITTEN BY   : Dominic
**************************************************************************/
#include <string.h>
#include <stdio.h>

#define MAX 5000
		      /* file pointers */
FILE *inp;            /* input file    */
FILE *out;            /* output file   */

char *input;
char *output;

char buf[MAX];       /* buffer area to read input file */

long startrec, endrec, cnt1, cnt2;

main(argc,argv)
int argc;
char *argv[];
{
   if(argc < 4)
   {
     fprintf(stderr,"Error, usage is cr <inputfile> <outputfile> <startrec> <endrec> \n");
      exit(1);
   }

   input = argv[1];            /* input filename assigned to pointer */
   output = argv[2];           /* output filename assigned to pointer */
   startrec = atoi(argv[3]);
   endrec = atoi(argv[4]);

  
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

   fgets(buf,MAX,inp);
   cnt1++;

   while (( feof(inp) == 0 ) && cnt1 <= endrec)  
   {

    if(startrec <= cnt1)     
    {
     fputs(buf,out);
     cnt2++;
    }

    fgets(buf,MAX,inp);
    cnt1++;

   }

  fclose(inp);
  fclose(out);

  fprintf(stdout,"%ld records copied from %s to %s\n",cnt2,input,output);
     fprintf(stdout,"Thank you for using Domware\n");

  return 0;

 }

