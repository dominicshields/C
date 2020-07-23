/**************************************************************************
PROGRAM NAME : muppet2
PURPOSE      : FIND SURNAMES I've mangled
PARAMETERS   : inputfile outputfile
**************************************************************************/
#include <string.h>
#include <stdio.h>
#include <ctype.h>
		      /* file pointers */
FILE *inp;            /* input file    */
FILE *out;            /* output file   */

char *input;
char *output;

char *p1;
char *p2;

int posctr,i,firstrec,len,address,ewnos;
long readcnt=0;
long writecnt=0;
char buf[1000];       /* buffer area to read input file */
char hold[1000];
char ewhold[20];
char edhold[20];


main(argc,argv)
int argc;
char *argv[];
{
   if(argc < 2)
   {
     fprintf(stderr,"Error, usage is muppet1 <inputfile> <outputfile> \n");
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
   firstrec = 0;
   address = 0;
   ewnos = 0;
 
   while ( feof(inp) == 0 ) 
   {
   	if(strncmp(p1,"\"Payee Number",13) == 0)
	{
		for(i=0;i<50;i++)
		{
			hold[i]='\0';
		}
		p1 += 16;
		strncpy(hold,p1,8);
		strcat(hold,",");
   	}
/* "Surname","SOREMEKUN  */   	
	if(strncmp(p1,"\"Surname",8) == 0)
	{
		p1 += 11;
		len = strcspn(p1," ");
		strncat(hold,p1,len);
		p1+=len;
		p1++;
		if(strncmp(p1,"     ",5) != 0)
		{
			p1-=2;
			if(p1[0] != ' ')
			{
				p1++;
			}
			strcat(hold,p1);
			writerec();
		}
   	}   	

    read1();
   }
   
  fprintf(stdout,"\nRecords Read  = %ld\n",readcnt);
  fprintf(stdout,"Records Written = %ld\n",writecnt);
  fclose(inp);
  fclose(out);

   return 0;

}

/*---------------------------- READ1 ----------------------------*/

read1()
{
  fgets(buf,1000,inp);
  p1=buf;
  readcnt++;
  return 0;	
}

/*-------------------------- WRITEREC ----------------------------*/

writerec()
{
	fprintf(out,"%s",hold);
	writecnt++;
	return 0;
}