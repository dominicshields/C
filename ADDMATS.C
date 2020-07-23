/**************************************************************************
PROGRAM NAME : addmats (source addmats.c)
PURPOSE      : addmats common fields in two files sorted on those
	       fields, writes out addmats and non-addmats, duplicates 
	       in one file not present on the other will not be matched
	       only the first occurrence. The default is to write out
	       the addmats unless addmatswritten is set to n.
	       If the files are not sorted in the match order there
	       is the option  of sorting them if you want.
PARAMETERS   : filename1 filename2 fieldpos1 fieldpos2
	       matchlength addmatswritten (optional)
OUTPUT       : addmats in file1 format (addmats1)
	       addmats in file2 format (addmats2)
	       records in file1 not in file2 (in1not2)
	       records in file2 not in file1 (in2not1)
	       counts of the above
WARNING      : don't even think about running this program if the files are
	       not sorted on the fields to be matched
COPYRIGHT    : this program is based on Wayne's JPFULLMATCH filetab
COMMENTS     : when working out the record lengths, field positions etc,
	       give the exact values, don't worry about newlines etc. Any
	       suggestions for improvements will be welcome (if practical).
	       I know that I've gone mad on pointers and could have done
	       it with at least 2 fewer but I'd have got very confused.
WRITTEN BY   : Dominic
**************************************************************************/
#include <string.h>
#include <stdio.h>
#include <time.h>

#define LEN 1000
		      /* file pointers */
FILE *inp1;           /* input file 1  */
FILE *inp2;           /* input file 2  */
FILE *match1;         /* addmats in file 1 format */
FILE *match2;         /* addmats in file 2 format (these may be the same) */
FILE *in1;            /* non-addmats present in file 1 not in file 2      */
FILE *in2;            /* non-addmats present in file 2 not in file 1      */

char *p1;
char *p2;
char *p3;
char *input1;
char *input2;

char buf1[LEN];       /* buffer area to read in file 1 */
char buf2[LEN];       /* buffer area to read in file 2 */

char sortcmd1[100];
char sortcmd2[100];
char mv1[100];
char mv2[100];
int pos1, pos2, len,cntcomm, a, b, c;
char wantmats[1];
long cnt1, cnt2, cnt3, cnt4, cnt5;
time_t tim;

main(argc,argv)
int argc;
char *argv[];
{
   int reply;

   if(argc < 3)
   {
	fprintf(stderr,"Error, usage is addmats <sorted file1> <sorted file2>\n");
	exit(1);
   }

   input1 = argv[1];            /* filename 1 assigned to pointer */
   input2 = argv[2];            /* filename 2 assigned to pointer */
   
   tim = time(NULL);
   fprintf(stderr,"\nProgram Begins  %s\n",ctime(&tim));

   if ((inp1 = fopen(input1,"r")) == NULL)
   {
      fprintf(stderr,"Error opening %s does it exist?\n",input1);
      exit(1);
   }

   if ((inp2 = fopen(input2,"r")) == NULL)
   {
      fprintf(stderr,"Error opening %s does it exist?\n",input2);
      exit(1);
   }

   if ((match1 = fopen("addmats1","w")) == NULL)
   {
        fprintf(stderr,"Error opening addmats1\n");
        exit(1);
   }

   if ((match2 = fopen("addmats2","w")) == NULL)
   {
          fprintf(stderr,"Error opening addmats2\n");
          exit(1);
   }

   if ((in1 = fopen("in1not2","w")) == NULL)
   {
      fprintf(stderr,"Error opening in1not2\n");
      exit(1);
   }

   if ((in2 = fopen("in2not1","w")) == NULL)
   {
      fprintf(stderr,"Error opening in2not1\n");
      exit(1);
   }

   cnt1=cnt2=cnt3=cnt4=cnt5=0;

   read1();
   read2();

/*********************************** MAIN LOOP ********************************/

   while ( (feof(inp1) == 0) && (feof(inp2) == 0) )
   {

	p1 = buf1;
	p2 = buf2;           
	a=0;
	cntcomm=0;
	while(cntcomm < 2)
	{
		p1++;
		a++;
		if(p1[0] == ',')
		{
			cntcomm++;
		}
	}

	b=0;
	cntcomm=0;
	while(cntcomm < 2)
	{
		p2++;
		b++;
		if(p2[0] == ',')
		{
			cntcomm++;
		}
	}
	if(a > b)
	{
		c = b;
	}
	else
	{
		c = a;
	}
	
	p1 = buf1;
	p2 = buf2;           
  	reply = strncmp(p1,p2,c);
   	if(reply > 0)
   	{
/**********		fprintf(stdout,"Nomatch : p1 = %s",p1);
		fprintf(stdout,"        : p2 = %s\n",p2);
		fprintf(stdout,"        : len = %d\n\n",c); ******/
    		fputs(buf2,in2);
    		cnt5++;
    		read2();
   	}


   	if(reply < 0)
   	{
/*******		fprintf(stdout,"Nomatch : p1 = %s",p1);
		fprintf(stdout,"        : p2 = %s\n",p2);
		fprintf(stdout,"        : len = %d\n\n",c); ******/
    		fputs(buf1,in1);
    		cnt4++;
    		read1();
   	}

   	if(reply == 0)
   	{
/*******		fprintf(stdout,"Match : p1 = %s\n",p1);
		fprintf(stdout,"      : p2 = %s\n",p2);
		fprintf(stdout,"        : len = %d\n\n",c); ******/
         	fputs(buf1,match1);
    		fputs(buf2,match2);
    		cnt3++;
    		read1();
    		read2();
   	}


   }

  if (feof(inp1) != 0)
  {
    while(feof(inp2) == 0)
    {
     fputs(buf2,in2);
     cnt5++;
     read2();
    }
  }

  if (feof(inp2) != 0)
  {
    while(feof(inp1) == 0)
    {
     fputs(buf1,in1);
     cnt4++;
     read1();
    }
  }

  fclose(inp1);
  fclose(inp2);
  fclose(match1);
  fclose(match2);
  fclose(in1);
  fclose(in2);
  fprintf(stdout,"Records read from file 1     = %ld\n",cnt1);
  fprintf(stdout,"Records read from file 2     = %ld\n",cnt2);
  fprintf(stdout,"Matching records             = %ld\n",cnt3);
  fprintf(stdout,"Records in file 1 not file 2 = %ld\n",cnt4);
  fprintf(stdout,"Records in file 2 not file 1 = %ld\n",cnt5);
  tim = time(NULL);
  fprintf(stderr,"Program Ends OK  %s\n",ctime(&tim));

  return 0;

 }

 /***************************   read1  ******************************/

 read1()
 {
  *fgets(buf1,LEN,inp1);
  if (feof(inp1) != 0)
  {
   fprintf(stderr,"End of file %s\n",input1);
   return 0;
  }
  cnt1++;
  return 0;
 }

 /***************************   read2  ******************************/

 read2()
 {
  *fgets(buf2,LEN,inp2);
  if (feof(inp2) != 0)
  {
   fprintf(stderr,"End of file %s\n",input2);
   return 0;
  }
  cnt2++;
  return 0;
 }
