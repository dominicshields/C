/**************************************************************************
PROGRAM NAME : matches
PURPOSE      : an intelligent aquatic mammal
PARAMETERS   : filename1 filename2 reclen1 reclen2 fieldpos1 fieldpos2
	       matchlength
OUTPUT       : matches in file1 format (matches1)
	       matches in file2 format (matches2)
	       records in file1 not in file2 (in1not2)
	       records in file2 not in file1 (in2not1)
	       counts of the above
WARNING      : don't even think about running this program if the files are
	       not sorted on the fields to be matched
COPYRIGHT    : this program is based on Wayne's JPFULLMATCH filetab
COMMENTS     : when working out the record lengths, field positions etc,
	       give the exact values, don't worry about newlines etc. Any
	       suggestions for improvements will be contemptuously ignored
	       (only joking). I know that I've gone mad on pointers and
	       could have done it with at least 2 fewer but I'd have got
	       very confused.
**************************************************************************/
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>
		      /* file pointers */
FILE *inp1;           /* input file 1  */
FILE *inp2;           /* input file 2  */
FILE *match1;         /* matches in file 1 format */
FILE *match2;         /* matches in file 2 format (these may be the same) */
FILE *in1;            /* non-matches present in file 1 not in file 2      */
FILE *in2;            /* non-matches present in file 2 not in file 1      */

char *p1;
char *p2;
char *input1;
char *input2;

char buf1[500];       /* buffer area to read in file 1 */
char buf2[500];       /* buffer area to read in file 2 */

int pos1, pos2, reclen1, reclen2, matlen, len;
long cnt1, cnt2, cnt3, cnt4, cnt5;
time_t tim;

main(argc,argv)
int argc;
char *argv[];
{
   int reply;
   char ans[1];
   if(argc < 8)
   {
     fprintf(stderr,"Error, usage is matches <filename1> <filename2> <reclen1> <reclen2> <fieldpos1> <fieldpos2> <matchlength>\n");
      exit(1);
   }
   fprintf(stderr,"Are the input files sorted on the match fields? y/n\n");
   *ans = getche();
   if(*ans == 'n')
   {
    fprintf(stderr,"\nYou Smeghead ! ....... Program exits\n");
    exit(1);
   }

   tim = time(NULL);
   fprintf(stderr,"\nProgram Begins  %s\n",ctime(&tim));

   input1 = argv[1];            /* filename 1 assigned to pointer */
   input2 = argv[2];            /* filename 2 assigned to pointer */
   reclen1 = atoi(argv[3]);
   reclen2 = atoi(argv[4]);
   pos1 = atoi(argv[5]);
   pos2 = atoi(argv[6]);
   len = atoi(argv[7]);
   pos1--;               /* This is done so that these values can be  */
   pos2--;               /* used to increment the pointers which will */
			 /* be pointing at the first byte             */

   reclen1++;            /* This is done to include the newline char  */
   reclen2++;

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

   if ((match1 = fopen("matches1","w")) == NULL)
   {
      fprintf(stderr,"Error opening matches1\n");
      exit(1);
   }

   if ((match2 = fopen("matches2","w")) == NULL)
   {
      fprintf(stderr,"Error opening matches2\n");
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

   while ( (feof(inp1) == 0) && (feof(inp2) == 0) )
   {

   p1 = buf1;
   p2 = buf2;           /* this is the reason for subtracting 1      */
   p1 = p1 + pos1;      /* from pos1 and pos2 to get the offset from */
   p2 = p2 + pos2;      /* the first byte                            */

   reply = strncmp(p1,p2,len);
   if(reply > 0)
   {
    writin2();
    read2();
   }

   if(reply < 0)
   {
    writin1();
    read1();
   }

   if(reply == 0)
   {
    writmat();
    read1();
    read2();
   }

}

  if (feof(inp1) != 0)
  {
    while(feof(inp2) == 0)
    {
     writin2();
     read2();
    }
  }

  if (feof(inp2) != 0)
  {
    while(feof(inp1) == 0)
    {
     writin1();
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
  int reply;

  reply = fread(buf1,reclen1,1,inp1);
  if (feof(inp1) != 0)
  {
   fprintf(stderr,"End of file %s\n",input1);
   return 0;
  }
  if (reply != 1)
  {
   fprintf(stderr,"Error reading %s\n",input1);
   exit(1);
  }
  cnt1++;
  return 0;
 }

 /***************************   read2  ******************************/

 read2()
 {
  int reply;

  reply = fread(buf2,reclen2,1,inp2);
  if (feof(inp2) != 0)
  {
   fprintf(stderr,"End of file %s\n",input2);
   return 0;
  }
  if (reply != 1)
  {
   fprintf(stderr,"Error reading %s\n",input2);
   exit(1);
  }
  cnt2++;
  return 0;
 }

  /***************************   writin1  ******************************/

  writin1()
  {
   int reply;
   reply = fwrite(buf1,reclen1,1,in1);
   if (reply != 1)
   {
    fprintf(stderr,"Error writing to file in1not2");
    exit(1);
   }
   cnt4++;
   return 0;
  }

/***************************   writin2  ******************************/

  writin2()
  {
   int reply;
   reply = fwrite(buf2,reclen2,1,in2);
   if (reply != 1)
   {
    fprintf(stderr,"Error writing to file in2not1");
    exit(1);
   }
   cnt5++;
   return 0;
  }

  /***************************   writmat  ******************************/

  writmat()
  {
   int reply;
   reply = fwrite(buf1,reclen1,1,match1);
   if (reply != 1)
   {
    fprintf(stderr,"Error writing to file matches1");
    exit(1);
   }

   reply = fwrite(buf2,reclen2,1,match2);
   if (reply != 1)
   {
    fprintf(stderr,"Error writing to file matches2");
    exit(1);
   }

   cnt3++;
   return 0;
  }
