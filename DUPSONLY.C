/**************************************************************************
PROGRAM NAME : dupsonly  (source dupsonly.c)
PURPOSE      : Similar to dups but different, this writes out ALL
	       occurrences of a given field and writes out a 
	       "duplicates-free" file containing no record from the 
	       input file that occurred more than once.
	       If the files are not sorted in the match order there
	       is the option  of sorting them if you want.
	       When you want a "BSOEQUALKEYED" file but want to see
	       what was duplicated use dups.
	       When you want a "BSOEQUALKEYED" file and don't care what
	       the duplicates are use sort with a -u flag.
PARAMETERS   : filename fieldpos fieldlength
OUTPUT       : A file containing all records with duplicates in the  
	       field specified, this is called dupsonlydups. A file
	       containing records not duplicated in the input file,
	       this is called dupsfree.
	       Counts of the above
WARNING      : don't even think about running this program if the files are
	       not sorted on the fields to be matched
COPYRIGHT    : this program is based on Wayne's JPDUPLICATES filetab
COMMENTS     : none
**************************************************************************/
#include <string.h>
#include <stdio.h>
#include <time.h>
			/* file pointers */
FILE *inp;		/* input file    */
FILE *dupes;		/* file of duplicates   */
FILE *nodupes;		/* file with all duplicates removed  */

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
   int flag;
   char ans[1];
   if(argc < 4)
   {
     fprintf(stderr,"Error, usage is dupsonly <filename> <fieldpos> <fieldlength>\n");
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

   if ((dupes = fopen("dupsonlydups","w")) == NULL)
   {
      fprintf(stderr,"Error opening dupsonlydups\n");
      exit(1);
   }

   if ((nodupes = fopen("dupsfree","w")) == NULL)
   {
      fprintf(stderr,"Error opening dupsfree\n");
      exit(1);
   }

   cnt1=cnt2=cnt3=flag=0;

   *fgets(buf2,1000,inp);	/* READ AHEAD TO GET FIRST RECORD */
   cnt1++;
   readin();			/* READ AHEAD TO GET SECOND RECORD */

   while (feof(inp) == 0)
   {
     p1 = buf1;
     p1 = p1 + pos;
     p2 = buf2;
     p2 = p2 + pos;

     reply = strncmp(p1,p2,len);

     if(reply == 0)              /* FOUND A MATCH            */
     {
	if(flag == 0)            /* MATCH FOR THE FIRST TIME */
	{                        /* WITH CONTENTS OF BUF2    */
         fputs(buf2,dupes);
	 flag = 1;
         cnt2++;
	 cnt3++;
	}
      fputs(buf1,dupes);
      cnt2++;
     }
     else                        /* NOT A MATCH             */
     {
	if(flag == 0)
	{
     	    fputs(buf2,nodupes);
	    strcpy(buf2,"");
	}
      strcpy(buf2,buf1);
      flag = 0;
     }
     readin();
  }

  if(flag == 0)             /* DEAL WITH ANY RECORDS HELD IN */
  {                         /* BUFFER AND NOT WRITTEN        */
     fputs(buf2,nodupes);
  }

  fclose(inp);
  fclose(dupes);
  fclose(nodupes);

  fprintf(stdout,"Records read from input file = %ld\n",cnt1);
  fprintf(stdout,"Duplicates written           = %ld\n",cnt2);
  fprintf(stdout,"Records not duplicates       = %ld\n",(cnt1-cnt2));
  fprintf(stdout,"Unique duplicates as it were = %ld\n",cnt3);

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

