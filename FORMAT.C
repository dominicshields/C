/* format.c */
#include <stdio.h>

FILE *inp;
FILE *outp;

char *p1;
char *input;
char *output;

char buf[500];
char buf2[500];
char hold[16];

int flag1,flag2,i;

main(argc,argv)
int argc;
char *argv[];
{
   if(argc < 3)
   {
     fprintf(stderr,"Error, usage is format <input file> <output file>\n");
      exit(1);
   }
   input = argv[1];
   output = argv[2];

   if ((inp = fopen(input,"r")) == NULL)
   {
       fprintf(stderr,"Error opening %s does it exist?\n",input);
       exit(1);
   }
   if ((outp = fopen(output,"w")) == NULL)
   {
       fprintf(stderr,"Error creating %s ?\n",output);
       exit(1);
   }

   read1();

   while (feof(inp) == 0)
   {
     flag1 = 0;
     for(i=0;i<strlen(buf);i++)
     {
	if(p1[i] == ',')
	{
		strncpy(hold,p1,15);
		flag1 = 1;
	}
     }

     flag2 = 0;
     for(i=0;i<strlen(buf);i++)
     {
	if(p1[i] != ' ')
	{
		flag2++;
	}
     }

     if(flag1 == 1 || flag2 < 2)
     {
	fprintf(outp,"%s",buf);
     }
     else
     {
	strcpy(buf2,hold);
	strcat(buf2,buf);
	fprintf(outp,"%s",buf2);
     }

     read1();
   }

   fclose(inp);
   fclose(outp);


   return 0;

 }

 /***************************   read1  ******************************/

 read1()
 {
  fgets(buf,500,inp);
  if (feof(inp) != 0)
  {
     fprintf(stderr,"End of file %s\n",input);
     return 0;
  }
  p1 = buf;
  return 0;
 }
