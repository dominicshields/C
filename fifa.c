#include <string.h>
#include <stdio.h>

FILE *inp;		/* input file */
FILE *outp;		/* variable file pointer */

char *p1;
char *input;
char buf[80];
char bufout[80];
int i,x,len;

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

   if ((outp = fopen("rankout.txt","w")) == NULL)
   {
	  fprintf(stderr,"Error writing rankout\n");
	  exit(1);
   }
   
   read1();

   while (feof(inp) == 0)
   {
	for(x=0;x<41;x++)
	{
		bufout[x]='\0';
	}

	i=0;
	while(i<7)
	{
		if(i==0)
		{
			len = strlen(buf);
			strncpy(bufout,buf,(len-1));
		}
		else
		{
			strncat(bufout,",",1);
			len = strlen(buf);
			strncat(bufout,buf,(len-1));
		}
		i++;
		read1();
	}

	fprintf(outp,"%s\n",bufout);
	/*read1();*/
   }

   fclose(inp);
   
   return 0;

 }

 /***************************   read1  ******************************/

 read1()
 {

  fgets(buf,30,inp);
  if (feof(inp) != 0)
  {
     fprintf(stderr,"End of file %s\n",input);
     return 0;
  }
  return 0;
 }
