/**************************************************************************
PROGRAM NAME : matchesn  (source matchesn.c)
PURPOSE      : matches common fields in two files sorted on those
	       fields, writes out matches and non-matches, duplicates
	       in one file not present on the other will not be matched
	       only the first occurrence. The default is to write out
	       the matches unless matcheswritten is set to n.
PARAMETERS   : filename1 filename2 fieldpos1 fieldpos2
	       matchlength matcheswritten (optional)
OUTPUT       : matches in file1 format (matches1)
	       matches in file2 format (matches2)
	       records in file1 not in file2 (in1not2)
	       records in file2 not in file1 (in2not1)
	       counts of the above
WARNING      : don't even think about running this program if the files are
	       not sorted on the fields to be matched
COPYRIGHT    : this program is based on Wayne's JPFULLMATCH filetab
COMMENTS     :
**************************************************************************/
#include <string.h>
#include <stdio.h>
#include <time.h>
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

int pos1, pos2, matlen, len;
char wantmats[1];
long cnt1, cnt2, cnt3, cnt4, cnt5;
time_t tim;

main(argc,argv)
int argc;
char *argv[];
{
   int reply;
   char ans[1];
   *wantmats = 'y';
   if(argc < 6)
   {
     fprintf(stderr,"Error, usage is matchesn <filename1> <filename2> <fieldpos1> <fieldpos2> <matchlength>\n");
      exit(1);
   }
   fprintf(stderr,"Are the input files sorted on the match fields? y/n\n");
   *ans = getchar();
   if(*ans == 'n')
   {
    fprintf(stderr,"\nYou Smeghead ! ....... Program exits\n");
    exit(1);
   }

   tim = time(NULL);
   fprintf(stderr,"\nProgram Begins  %s\n",ctime(&tim));

   input1 = argv[1];            /* filename 1 assigned to pointer */
   input2 = argv[2];            /* filename 2 assigned to pointer */
   pos1 = atoi(argv[3]);
   pos2 = atoi(argv[4]);
   len = atoi(argv[5]);
   if(argc == 7)
   {
     *wantmats = *argv[6];
   }
   pos1--;               /* This is done so that these values can be  */
   pos2--;               /* used to increment the pointers which will */
			 /* be pointing at the first byte             */

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
    fputs(buf2,in2);
    cnt5++;
    read2();
   }

   if(reply < 0)
   {
    fputs(buf1,in1);
    cnt4++;
    read1();
   }

   if(reply == 0)
   {
    if(*wantmats ==  'y')
    {
      fputs(buf1,match1);
      fputs(buf2,match2);
    }
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
  *fgets(buf1,500,inp1);
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
  *fgets(buf2,500,inp2);
  if (feof(inp2) != 0)
  {
   fprintf(stderr,"End of file %s\n",input2);
   return 0;
  }
  cnt2++;
  return 0;
 }
