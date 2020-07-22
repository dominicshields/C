/**************************************************************************
PROGRAM NAME : domedit  (source domedit.c)
PURPOSE      : edits by byte position.
PARAMETERS   : -r (optional) filename fieldpos1 fieldlength
	       string up to fieldpos10 fieldlength10 string10
OUTPUT       : edited file with any luck
	       counts of the above
COMMENTS     : This can also use standard input as input in the 
	       following way :
	       process | domedit stdin 1 5 12345 . Note that the
	       filename parameter is replaced by the word stdin.
	       The -r parameter tells the program that the fields
	       should replace not insert.
**************************************************************************/
#include <string.h>
#include <stdio.h>
#include <time.h>

#define RECLEN 2000
#define DOMNO 20

FILE *inp;		/* input file */
FILE *ptr;		/* variable file pointer */

char *p1;
char *input;
char buf[RECLEN];	/* buffer area to read input file */
char buf2[RECLEN];
char *pattern[DOMNO];	/* holds match string */
int pos[DOMNO];
int len[DOMNO];
int reply;
int cnt1, cnt2, i, j, x, num, doneone, lenadj;
int rflag=0;
int params=0;
time_t tim;

main(argc,argv)
int argc;
char *argv[];
{
   if(argc < 5)
   {
     fprintf(stderr,"Error, usage is domedit <filename> <fieldpos> <fieldlength> <string>... <fieldposn> <fieldlengthn> <stringn>\n");
      exit(1);
   }

   tim = time(NULL);
   fprintf(stderr,"\nProgram Begins  %s\n",ctime(&tim));

   cnt1=0;
   cnt2=0;
   doneone=0;
   lenadj=0;
   i=1;

   if(strncmp(argv[1],"-r",2) == 0)
   {
     i++;
     rflag=1;
   }

   input = argv[i];	/* filename 1 assigned to pointer */

   i++;

   for(x=0;i<argc;i++,x++)
   {
     (pos[x]) = atoi(argv[i]);
     if(rflag==1)
     {
	pos[x]--;
     }
     i++;
     (len[x]) = atoi(argv[i]);
     i++;
     pattern[x] = argv[i];
     params++;
   }

   if(strncmp(input,"stdin",5) != 0)
   {
	if ((inp = fopen(input,"r")) == NULL)
	{
	  fprintf(stderr,"Error opening %s does it exist?\n",input);
	  exit(1);
	}
	ptr = inp;  /* set variable file pointer to input file pointer */
   }
   else
   {
	ptr = stdin;    /* set variable file pointer to standard input */
   }

   read1();

   while (feof(ptr) == 0)
   {

     for(i=2,x=0;i<argc;i=i+3,x++)
     {
	p1 = buf;
	if(x>0 && rflag==0 && doneone==0)
	{
		j=x;
		j--;
		lenadj = (lenadj + len[j]);
		pos[x] = (pos[x] + lenadj);
	}


	p1 = p1 + pos[x];

	if( rflag == 1)
	{

		strncpy(p1,pattern[x],len[x]);
	}
	else
	{
		strcpy(buf2,p1);
		strncpy(p1,pattern[x],len[x]);
		p1+= (len[x]);
		p1[0] = '\0';
		strcat(p1,buf2);
	}
     }

/*     fprintf(stdout,"%s\n",buf);   */
     fprintf(stdout,"%s",buf);
     cnt2++;
     doneone=1;
     read1();
   }

   fclose(inp);


   fprintf(stderr,"Records read from input file = %d\n",cnt1);
   fprintf(stderr,"Records written (edited ? )  = %d\n",cnt2);
   tim = time(NULL);
   fprintf(stderr,"Program Ends OK  %s\n",ctime(&tim));

   return 0;

 }

 /***************************   read1  ******************************/

 read1()
 {

  fgets(buf,RECLEN,ptr);
  if (feof(ptr) != 0)
  {
     fprintf(stderr,"End of file %s\n",input);
     return 0;
  }

  cnt1++;
  return 0;
 }
