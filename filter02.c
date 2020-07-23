#include <string.h>
#include <stdio.h>

FILE *inp;
FILE *out;

char *input;
char *output;

char buf[1000];       /* buffer area to read input file */

long cnt1, cnt2;

main(argc,argv)
int argc;
char *argv[];
{
   if(argc < 2)
   {
     fprintf(stderr,"Error, usage is %s <inputfile> <outputfile>\n",argv[0]);
      exit(1);
   }

   input = argv[1];            /* input filename assigned to pointer */
   output = argv[2];           /* output filename assigned to pointer */
  
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

   cnt1=0;
   cnt2=0;

   fgets(buf,1000,inp);
   cnt1++;

   while ( feof(inp) == 0 )
   {

    if(strncmp(buf,"02",2) == 0)
    {
     fputs(buf,out);
     cnt2++;
    }

    fgets(buf,1000,inp);
    cnt1++;

    }

    if(strncmp(buf,"02",2) == 0)
    {
     fputs(buf,out);
     cnt2++;
    }

  fclose(inp);
  fclose(out);

  fprintf(stdout,"%ld records copied from %s to %s\n",cnt2,input,output);

  return 0;

 }

