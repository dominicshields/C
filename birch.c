#include <string.h>
#include <stdio.h>

FILE *inp;		/* input file */
FILE *outp;		/* variable file pointer */

char *p1;
char *input;
char buf[400];
char bufout[400];
int i,j,x,len;

main(argc,argv)
int argc;
char *argv[];
{
   input = argv[1];
   if ((inp = fopen(input,"r")) == NULL)
   {
	  fprintf(stderr,"Error opening %s does it exist?\n",input);
	  exit(1);
   }

   if ((outp = fopen("birchout.txt","w")) == NULL)
   {
	  fprintf(stderr,"Error writing birchout\n");
	  exit(1);
   }
   
   read1();

   while (feof(inp) == 0)
   {
	
	i=0;
	j=0;
	p1 = buf;
	while(i<strlen(buf))
	{
		if(p1[i]=='@')
		{
			j++;
		}
		i++;
		
	}
	if(j<9)
	{
		i--;
		p1[i]='@';
		fprintf(outp,"%s\n",buf);
	}
	else
	{
		fprintf(outp,"%s",buf);
	}
	read1();
	
   }

   fclose(inp);
   
   return 0;

 }

 /***************************   read1  ******************************/

 read1()
 {
  for(x=0;x<400;x++)
  {
	buf[x]='\0';
  }
  fgets(buf,400,inp);
  if (feof(inp) != 0)
  {
     fprintf(stderr,"End of file %s\n",input);
     return 0;
  }
  return 0;
 }
