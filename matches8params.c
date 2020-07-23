/**************************************************************************
PROGRAM NAME : matches (source matches.c)

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
COMMENTS     : when working out the record lengths, field positions etc,
	       give the exact values, don't worry about newlines etc. Any
	       suggestions for improvements will be welcome (if practical).
	       I know that I've gone mad on pointers and could have done
	       it with at least 2 fewer but I'd have got very confused.
WRITTEN BY   : Dominic
Updated 12/10/2011 17:50: The LEN constant had to be increased from 1000
	                        to 5000 to cope with an MWSS error that was causing
	                        the matching to fail. Dominic
Updated 24/10/2012 17:39: Changed this code but did not recompile yet as its a
                          cosmetic issue - namely the error message if you do not
                          input the correct number of parameters was misleading.
                          Also added two code comments about strncmp and fgets
                          that are important for how this program operates.
**************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define LEN 5000

int read1(void);
int read2(void);

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
char *output1;
char *output2;

char buf1[LEN];       /* buffer area to read in file 1 */
char buf2[LEN];       /* buffer area to read in file 2 */

char sortcmd1[100];
char sortcmd2[100];
char mv1[100];
char mv2[100];
int pos1, pos2, len;
char wantmats[1];
long cnt1, cnt2, cnt3, cnt4, cnt5;
time_t tim;

/*read1();
read2();*/

main(argc,argv)
int argc;
char *argv[];
{
   int reply;
   char ans[1];
   *wantmats = 'y';
   if(argc < 8)
   {
     fprintf(stderr,"Error, usage is matches <filename1> <filename2> <outfile1> <outfile2> <fieldpos1> <fieldpos2> <matchlength>\n");
      exit(1);
   }

   input1 = argv[1];            /* filename 1 assigned to pointer */
   input2 = argv[2];            /* filename 2 assigned to pointer */
   output1 = argv[3];
   output2 = argv[4];
   pos1 = atoi(argv[5]);
   pos2 = atoi(argv[6]);
   len = atoi(argv[7]);
   if(argc == 9)
   {
     *wantmats = *argv[8];
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

   tim = time(NULL);
   fprintf(stderr,"\nProgram Begins  %s\n",ctime(&tim));


   if(*wantmats == 'y')
   {
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
   }

   if ((in1 = fopen(output1,"w")) == NULL)
   {
      fprintf(stderr,"Error opening in1not2\n");
      exit(1);
   }

   if ((in2 = fopen(output2,"w")) == NULL)
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

/*
NOTE ON THE STRNCMP BELOW - THIS CAME FROM THE C++ STANDARD - http://www.cplusplus.com/reference/clibrary/cstring/strncmp/
DEMONSTRATING THAT THE len (size_t) PARAMETER CAN BE ARBITRARILY LONG
int strncmp ( const char * str1, const char * str2, size_t num );Compare characters of two strings
Compares up to num characters of the C string str1 to those of the C string str2.
This function starts comparing the first character of each string. If they are equal to each other,
it continues with the following pairs until the characters differ, until a terminating null-character
is reached, or until num characters match in both strings, whichever happens first.
*/
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
     if(*wantmats == 'y')
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
  if(*wantmats == 'y')
  {
		fclose(match1);
		fclose(match2);
  }
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

/*
NOTE ON THE FGETS BELOW - THIS CAME FROM THE C++ STANDARD - http://www.cplusplus.com/reference/clibrary/cstdio/fgets/
THE IMPORTANT BIT IS ABOUT THE NULL NOBODY MENTIONS
char * fgets ( char * str, int num, FILE * stream );Get string from stream
Reads characters from stream and stores them as a C string into str until (num-1) characters have been read or either
a newline or the end-of-file is reached, whichever happens first.
A newline character makes fgets stop reading, but it is considered a valid character by the function and included
in the string copied to str.
A terminating null character is automatically appended after the characters copied to str.
*/
 /***************************   read1  ******************************/

 read1()
 {
  fgets(buf1,LEN,inp1);
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
  fgets(buf2,LEN,inp2);
  if (feof(inp2) != 0)
  {
   fprintf(stderr,"End of file %s\n",input2);
   return 0;
  }
  cnt2++;
  return 0;
 }
