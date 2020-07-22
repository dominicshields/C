/**************************************************************************
PROGRAM NAME : apr  (source apr.c)
PURPOSE      : copies or appends the records specified (by line number)
	       from a file to a named output file, if this exists, the
	       records are appended to it, otherwise the output file is
	       created.
PARAMETERS   : inputfile outputfile startrec endrec (defaults to EOF).
OUTPUT       : file containing records asked for.
	       counts to confirm number copied/appended.
WARNING      : You know those people who say why write a C program to
	       do something that UNIX can do easily by splitting a
	       100 MB file into 500 files then cut them, grep them, paste
	       them, sort them etc and you'll get exactly the same result
	       as this program, they're boring they are.
COPYRIGHT    : this program is sort of like VME's APPEND RECORDS command
	       but different.
COMMENTS     : I would have called the program "ar" but that's a UNIX
	       command that is.
**************************************************************************/
#include <string.h>
#include <stdio.h>
		      /* file pointers */
FILE *inp;            /* input file    */
FILE *out;            /* output file   */

char *input;
char *output;

char buf[1000];       /* buffer area to read input file */

char mode[9];
long startrec, endrec, cnt1, cnt2;

main(argc,argv)
int argc;
char *argv[];
{
   if(argc < 4)
   {
     fprintf(stderr,"Error, usage is apr <inputfile> <outputfile> <startrec> <endrec> \n");
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

   if ((out = fopen(output,"r")) == NULL)
   {
      fprintf(stderr,"Target file %s does not exist, apr will copy records\n",output);
      strcpy(mode,"Copied");
   }
   else 
   {
      fprintf(stderr,"Target file %s exists, apr will append records\n",output);
      strcpy(mode,"Appended");
   }

   if ((out = fopen(output,"a")) == NULL)
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

  fprintf(stdout,"%ld records %s from %s to %s\n",cnt2,mode,input,output);

  return 0;

 }

