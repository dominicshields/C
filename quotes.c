#include <string.h>
#include <stdio.h>

#define BUFLEN 1000

FILE *inp;
FILE *out;

char *input;
char *output;

char buf[BUFLEN];
char bufout[BUFLEN]; 

char *p1;
char *p2;

long cnt1, cnt2;

int i;
int j;

main(argc,argv)
int argc;
char *argv[];
{
   if(argc < 3)
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

   fgets(buf,BUFLEN,inp);
   cnt1++;

   while ( feof(inp) == 0 )
   {
		lines();
		fgets(buf,BUFLEN,inp);
		cnt1++;
   }

   lines();
    
   fclose(inp);
   fclose(out);

   fprintf(stdout,"%ld records copied from %s to %s\n",cnt2,input,output);

   return 0;

 }

int lines()
{
    p1 = buf;
    p2 = bufout;
    i=0;
    j=0;
    while(i <= strlen(buf))
    {
    	if(p1[0] == '"')
    	{
    	
    	}
		else
		{
			p2[j]=p1[0];
    	   	j++;
		}
		i++;
    	p1++;
    }
    
    bufout[i] = '\n';
    fputs(bufout,out);
    cnt2++;

	for(i=0;i<BUFLEN;i++)
    {
    	bufout[i] = '\0';
    }
	return 0;
    
}