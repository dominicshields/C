/**************************************************************************
PROGRAM NAME : ntlog
PURPOSE      : reformats the NT server logs output to be useful to Notes
PARAMETERS   : inputfile outputfile
**************************************************************************/
#include <string.h>
#include <stdio.h>
		      /* file pointers */
FILE *inp;            /* input file    */
FILE *out;            /* output file   */

char *input;
char *output;

char *p1;
int posctr,i;
char buf[1000];       /* buffer area to read input file */
char hold[20];

main(argc,argv)
int argc;
char *argv[];
{
   if(argc < 2)
   {
     fprintf(stderr,"Error, usage is cr <inputfile> <outputfile> \n");
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

   read1();
   posctr = 0;

   while ( feof(inp) == 0 ) 
   {
	if(strncmp(p1,"Uptime",6) == 0)
	{
		if(posctr > 0)
		{
			fprintf(out,"#####\n");
		}
		p1 += 21;
		for(i=0;i<20;i++)
		{
			hold[i]='\0';
		}
		strncpy(hold,p1,strlen(p1)-1);
		fprintf(out,"%s,",hold);
		posctr = 1;
		read1();
   	}
   	
   	if(posctr < 7)
   	{
   		posctr++;
   	}
   	if(posctr == 6)
   	{
   		p1 += 20;
   		for(i=0;i<20;i++)
		{
			hold[i]='\0';
		}
		strncpy(hold,p1,strlen(p1)-2);
		fprintf(out,"%s\n",hold);
		posctr++;
		read1();
   	}
   	if(posctr > 6)
   	{
   		fprintf(out,"%s",p1);
   		posctr++;
   	}

    read1();
   }

  fclose(inp);
  fclose(out);

   return 0;

}

read1()
{
  fgets(buf,1000,inp);
  p1=buf;
  return 0;	
}

