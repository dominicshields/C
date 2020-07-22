#include <string.h>
#include <stdio.h>
		      /* file pointers */
FILE *inp;            /* input file    */
FILE *out;            /* output file   */

char *input;
char *output;

char buf[32];       /* buffer area to read input file */
char fred[32];
long cnt1;
int i,j;

main(argc,argv)
int argc;
char *argv[];
{
 
   input = argv[1];            /* input filename assigned to pointer */
   output = argv[2];           /* output filename assigned to pointer */
   

   if ((inp = fopen(input,"r")) == NULL)
   {
      fprintf(stderr,"Error opening %s does it exist?\n",input);
      exit(1);
   }

   if ((out = fopen(output,"w")) == NULL)
   {
      fprintf(stderr,"error opening %s as write \n",output);
      exit(1);
   }
  

   fgets(buf,32,inp);
   cnt1=0;

   while ( feof(inp) == 0 ) 
   {
    i = strlen(buf);
    j = i -1;
    fred[0]='"';
    strncat(fred,buf,j);
    strcat(fred,"\"\n");
    
    fprintf(out,"intarr(%d)" ,cnt1);
    fprintf(out," = %s",fred);
    fgets(buf,32,inp);
    cnt1++;
    for(i=0;i<32;i++)
    {
	fred[i]='\0';
    }

   }

  fclose(inp);
  fclose(out);

  return 0;

 }

