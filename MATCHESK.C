/**************************************************************************
PROGRAM NAME : matchesk (source matchesk.c)
PURPOSE      : like matches but copes with multiple (different) numbers of 
		duplicates
PARAMETERS   : filename1 filename2 fieldpos1 fieldpos2
	       matchlength matcheskwritten (optional)
OUTPUT       : matchesk in file1 format (matchesk1)
	       matchesk in file2 format (matchesk2)
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

char buf1[1000];       /* buffer area to read in file 1 */
char buf2[1000];       /* buffer area to read in file 2 */

char sortcmd1[100];
char sortcmd2[100];
char mv1[100];
char mv2[100];
char holdkey1[1000];
char holdkey2[1000];
int pos1, pos2, len;
char wantmats[1];
long cnt1, cnt2, cnt3, cnt4, cnt5, cnt6;
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
     fprintf(stderr,"Error, usage is matchesk <filename1> <filename2> <fieldpos1> <fieldpos2> <matchlength>\n");
      exit(1);
   }

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

   fprintf(stderr,"Are the input files sorted on the match fields? y/n\n");
   *ans = getchar();
   if(*ans == 'n')
   {
     fflush(stdin);
     fprintf(stderr,"Do you want me to sort them ? y/n\n");
     *ans = getchar();
     if(*ans == 'n')
     {
       fprintf(stderr,"Don't trust me do you, very wise, exiting program\n");
       exit(1);
     }
     else
     {
       fprintf(stderr,"SORTING %s \n",input1);
       sprintf(sortcmd1,"sort +0.%d -0.%d %s > tmpsrt1",pos1,(pos1+len),input1);
       sprintf(mv1,"mv tmpsrt1 %s",input1);
       fprintf(stderr,"%s\n",sortcmd1);
       system(sortcmd1);
       fprintf(stderr,"%s\n",mv1);
       system(mv1);
       fprintf(stderr,"SORTING %s \n",input2);
       sprintf(sortcmd2,"sort +0.%d -0.%d %s > tmpsrt2",pos2,(pos2+len),input2);
       sprintf(mv2,"mv tmpsrt2 %s",input2);
       fprintf(stderr,"%s\n",sortcmd2);
       system(sortcmd2);
       fprintf(stderr,"%s\n",mv2);
       system(mv2);
     } 
   }

   tim = time(NULL);
   fprintf(stderr,"\nProgram Begins  %s\n",ctime(&tim));

   fclose(inp1);
   fclose(inp2);

   if ((inp1 = fopen(input1,"r")) == NULL)
   {
      fprintf(stderr,"Error reopening %s big problems here\n",input1);
      exit(1);
   }

   if ((inp2 = fopen(input2,"r")) == NULL)
   {
      fprintf(stderr,"Error reopening %s big problems here\n",input2);
      exit(1);
   }

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

   cnt1=cnt2=cnt3=cnt4=cnt5=cnt6=0;

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
	if(strncmp(p2,holdkey2,len) == 0)
	{
		fputs(buf2,match2);
		cnt6++;
	}
	else
	{
    		fputs(buf2,in2);
	}
    cnt5++;
    read2();
   }

   if(reply < 0)
   {
	if(strncmp(p1,holdkey1,len) == 0)
	{
		fputs(buf1,match1);
		cnt6++;
	}
	else
	{
    		fputs(buf1,in1);
	}
    
    cnt4++;
    read1();
   }

   if(reply == 0)
   {
     if(*wantmats == 'y')
     {
      fputs(buf1,match1);
      fputs(buf2,match2);
      strncpy(holdkey1,p1,len);
      strncpy(holdkey2,p2,len);
      holdkey1[len]='\0';
      holdkey2[len]='\0';
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
	if(strncmp(p2,holdkey2,len) == 0)
	{
		fputs(buf2,match2);
		cnt6++;
	}
	else
	{
    		fputs(buf2,in2);
	}     
     cnt5++;
     read2();
    }
  }

  if (feof(inp2) != 0)
  {
    while(feof(inp1) == 0)
    {
	if(strncmp(p1,holdkey1,len) == 0)
	{
		fputs(buf1,match1);
		cnt6++;
	}
	else
	{
    		fputs(buf1,in1);
	}
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
  fprintf(stdout,"1 for 1 matching records     = %ld\n",cnt3);
  fprintf(stdout,"Duplicate matching records   = %ld\n",cnt6);
  fprintf(stdout,"Records in file 1 not file 2 = %ld\n",cnt4);
  fprintf(stdout,"Records in file 2 not file 1 = %ld\n",cnt5);
  tim = time(NULL);
  fprintf(stderr,"Program Ends OK  %s\n",ctime(&tim));

  return 0;

 }

 /***************************   read1  ******************************/

 read1()
 {
  *fgets(buf1,1000,inp1);
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
  *fgets(buf2,1000,inp2);
  if (feof(inp2) != 0)
  {
   fprintf(stderr,"End of file %s\n",input2);
   return 0;
  }
  cnt2++;
  return 0;
 }
