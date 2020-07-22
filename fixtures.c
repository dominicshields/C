#include <string.h>
#include <stdio.h>
#include <ctype.h>
#define RECLEN 80

FILE *inp;		/* input file */
FILE *outp;		/* variable file pointer */

char *p1;
char *input;
char buf[RECLEN];
char bufout[RECLEN];
char fixdate[13];
char test[2];
char first = ' ';
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

   if ((outp = fopen("fixout.txt","w")) == NULL)
   {
	  fprintf(stderr,"Error writing fixout\n");
	  exit(1);
   }
   
   read1();

   while (feof(inp) == 0)
   {
	for(x=0;x<RECLEN;x++)
	{
		bufout[x]='\0';
	}
	
	strncpy(test,buf,1);
	test[1]='/0';
	first = *test;
printf("first contains %c\n",first);
	if(isdigit(first))
	{	
		printf("test contains %.1s\n",test);
		for(x=0;x<13;x++)
		{
			fixdate[x]='\0';
		}
		strncpy(fixdate,buf,(strlen(buf)-1));
		read1();
	}

	strcpy(bufout,fixdate);
	strcat(bufout,buf);
	fprintf(outp,"%s",bufout);
	i++;
	read1();
	}


   fclose(inp);
   
   return 0;

 }

 /***************************   read1  ******************************/

 read1()
 {

  fgets(buf,RECLEN,inp);
  if (feof(inp) != 0)
  {
     fprintf(stderr,"End of file %s\n",input);
     return 0;
  }
  return 0;
 }
