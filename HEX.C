/**************************************************************************
PROGRAM NAME : hex  (source hex.c)
COMMENTS     :
**************************************************************************/
#include <stdlib.h>
#include <stdio.h>
		      /* file pointers */
FILE *inp;            /* input file    */
FILE *out;            /* output file   */

char *input;
char *output;

char buf[1000];       /* buffer area to read input file */

long num1, num2, len;
char hold[16];

main(argc,argv)
int argc;
char *argv[];
{
   if(argc < 2)
   {
     fprintf(stderr,"Error, usage is cr <inputfile>\n");
      exit(1);
   }

   input = argv[1];            /* input filename assigned to pointer */
 /*  output = argv[2];            output filename assigned to pointer */

   if ((inp = fopen(input,"rb")) == NULL)
   {
      fprintf(stderr,"Error opening %s does it exist?\n",input);
      exit(1);
   }

   if ((out = fopen("outf","w")) == NULL)
   {
      fprintf(stderr,"Error opening outf\n");
      exit(1);
   }

   fgets(buf,1000,inp);

   while (feof(inp) == 0)  
   {
	strcpy(hold,buf);
	len = strlen(hold);
	hold[len-1]='\0';
	num1=atol(hold);
	fprintf(stdout,"hold = %s; dec value = %ld; hex value = %x\n",hold,num1,num1); 
	fgets(buf,1000,inp);
   }

  fclose(inp);
  fclose(out);


  return 0;

 }

