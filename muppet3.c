/**************************************************************************
PROGRAM NAME : muppet1
PURPOSE      : reformats the Census enumerator personnel file to be half useful
PARAMETERS   : inputfile outputfile
LAYOUT OF INPUT FILE : Bonkers Frankly
"Payee Number","00AA0101  "
"Additional Account"," "

LAYOUT OF OUTPUT FILE : (In one record - doh !)
Payee Number,Staff Grade,Date Appointed,Surname,First Name,Initials,Title,
Date of Birth,Sort Code,Account Number,Additional Account,Home Address1,Home Address2,
Home Address3,Home Address4,Postcode,Home Phone,Work Phone,Tax Code,NI Code,
National Insurance No,Car/Van Indicator,Motor Cycle,EW Nos,ED Nos,
Previous Employment Indicator
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
int okflag=0;


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
/* "Payee Number","00AA0101  " */   	
	if(strncmp(p1,"\"Payee Number",13) == 0)
	{
		address = 0;
		okflag = 0;
		for(i=0;i<1000;i++)
		{
			hold[i]='\0';
		}
		p1 += 16;
		strncpy(hold,p1,8);
		strcat(hold,",");
		posctr = 1;
		goto lab;
   	}

/* "Additional Account"," "*/
   	if(strncmp(p1,"\"Additional Account",19) == 0)
	{
		p1 += 22;
		if(strlen(p1) > 4)
		{
			strncat(hold,p1,strlen(p1));
			okflag = 1;
		}
		else
		{
			okflag = 0;
		}
		goto lab;
   	}

/* "Previous Employment Indicator","N"	*/
	if(strncmp(p1,"\"Previous Employment Indicator",30) == 0)
	{	
		if(okflag == 1)
		{
			writerec();
		}
   	}
   	
    lab:
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