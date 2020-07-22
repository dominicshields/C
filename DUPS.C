/**************************************************************************
PROGRAM NAME : dups  (source dups.c)
PURPOSE      : Finds duplicates in a file, writes out the main file free
	       of duplicates i.e removes all but the first and writes the
	       duplicates removed to another file.
	       If the files are not sorted in the match order there
	       is the option  of sorting them if you want.
PARAMETERS   : filename fieldpos fieldlength
OUTPUT       : A "BSOEQUALKEYED" file with duplicates removed (nodups).
	       A duplicates file, this will not contain the first of a set
	       of duplicates, ie if there were two of a particular key, one
	       would remain on nodups and one would be written to this file
	       I shall call duplicates.
	       counts of the above
WARNING      : don't even think about running this program if the files are
	       not sorted on the fields to be matched
COPYRIGHT    : this program is based on Wayne's JPDUPLICATES filetab
COMMENTS     : none
**************************************************************************/
#include <stdio.h>
#include <time.h>
			/* file pointers */
FILE *inp;		/* input file    */
FILE *nodupes;		/* file with duplicates removed */
FILE *dupes;		/* file of removed duplicates   */

char *p1;
char *p2;

char *input;

char buf1[1000];	/* buffer area to read in input file */
char buf2[1000];	/* holding area for previous record  */
char sortcmd[100];
char mv1[100];

int pos, len;
long cnt1, cnt2, cnt3;
time_t tim;

main(argc,argv)
int argc;
char *argv[];
{
   int reply;
   char ans[1];
   if(argc < 4)
   {
     fprintf(stderr,"Error, usage is dups <filename> <fieldpos> <fieldlength>\n");
      exit(1);
   }

   input = argv[1];	/* input file name assigned to pointer */
   pos = atoi(argv[2]);
   len = atoi(argv[3]);
   pos--;		/* This is done so that the value can be  */
			/* used to increment the pointer which will */
			/* be pointing at the first byte             */

   if ((inp = fopen(input,"r")) == NULL)
   {
      fprintf(stderr,"Error opening %s does it exist?\n",input);
      exit(1);
   }

   fprintf(stderr,"Is the input file sorted on the fields to be compared? y/n\n");
   *ans = getchar();
   if(*ans == 'n')
   {
     fflush(stdin);
     fprintf(stderr,"Do you want me to sort it ? y/n\n");
     *ans = getchar();
     if(*ans == 'n')
     {
       fprintf(stderr,"Don't trust me do you, very wise, exiting program\n");
       exit(1);
     }
     else
     {
       fprintf(stderr,"SORTING %s \n",input);
       sprintf(sortcmd,"sort +0.%d -0.%d %s > tmpsrt",pos,(pos+len),input);
       sprintf(mv1,"mv tmpsrt %s",input);
       fprintf(stderr,"%s\n",sortcmd);
       system(sortcmd);
       fprintf(stderr,"%s\n",mv1);
       system(mv1);
     }
   }

   tim = time(NULL);
   fprintf(stderr,"\nProgram Begins  %s\n",ctime(&tim));

   fclose(inp);

   if ((inp = fopen(input,"r")) == NULL)
   {
      fprintf(stderr,"Error reopening %s big problem here\n",input);
      exit(1);
   }

   if ((nodupes = fopen("nodups","w")) == NULL)
   {
      fprintf(stderr,"Error opening nodups\n");
      exit(1);
   }

   if ((dupes = fopen("duplicates","w")) == NULL)
   {
      fprintf(stderr,"Error opening duplicates\n");
      exit(1);
   }

   cnt1=cnt2=cnt3=0;

   *fgets(buf2,1000,inp);
   cnt1++;
   readin();

   while (feof(inp) == 0)
   {
     p1 = buf1;
     p1 = p1 + pos;
     p2 = buf2;
     p2 = p2 + pos;

     reply = strncmp(p1,p2,len);

     if(reply == 0)
     {
      fputs(buf1,dupes);
      cnt2++;
     }
     else
     {
      fputs(buf2,nodupes);
      cnt3++;
      strcpy(buf2,buf1);
     }
     readin();
  }

  fputs(buf2,nodupes);
  cnt3++;
  fclose(inp);
  fclose(nodupes);
  fclose(dupes);

  fprintf(stdout,"Records read from input file = %ld\n",cnt1);
  fprintf(stdout,"Duplicates written           = %ld\n",cnt2);
  fprintf(stdout,"Records written to nodups    = %ld\n",cnt3);

  tim = time(NULL);
  fprintf(stderr,"Program Ends OK  %s\n",ctime(&tim));

  return 0;

}

/***************************   readin  ******************************/

 readin()
 {
  *fgets(buf1,1000,inp);
  if (feof(inp) != 0)
  {
   fprintf(stderr,"End of file %s\n",input);
   return 0;
  }
  cnt1++;
  return 0;
 }
