/**************************************************************************
PROGRAM NAME : cr  (source cr.c)
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

