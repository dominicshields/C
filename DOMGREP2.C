/**************************************************************************
PROGRAM NAME : domgrep2 (source domgrep2.c)
PURPOSE      : finds a given string anywhere in a file just like the
               UNIX grep but without the limitations of that.
PARAMETERS   : -v (optional) -o (optional) filename string1 to string10 
OUTPUT       : records containing the string
	       counts of the above
COMMENTS     : This can also use standard input as input in the 
	       following way :
	       process | domgrep stdin 12345 . Note that the
	       filename parameter is replaced by the word stdin.
	       The -v parameter does the same as the grep -v, it gives
	       you the lines that do not conform to the pattern.
	       The -o parameter specifies that where more than one
	       search string is used, records that satisfy any one of
	       them will be found. In other words -o tells the program 
	       to look for ORs rather than ANDs.
	       At this stage the program has no way of coping with a
	       mixture of ands and ors.
WRITTEN BY   : Dominic
**************************************************************************/
#include <string.h>
#include <stdio.h>
#include <time.h>

#define RECLEN 2000
#define DOMNO 10

FILE *inp;		/* input file */
FILE *ptr;		/* variable file pointer */

char *p1;
char *input;
char buf[RECLEN];	/* buffer area to read input file */
char *pattern[DOMNO];	/* holds match string */
int pos[DOMNO];
int len[DOMNO];
int reply;
int cnt1, cnt2, i, j, k, l, x, okflag, found, wildcard;
int vflag=0;
int oflag=0;
int params=0;
time_t tim;

main(argc,argv)
int argc;
char *argv[];
{
   if(argc < 3)
   {
     fprintf(stderr,"Error, usage is domgrep <filename> <string>... <stringn>\n");
      exit(1);
   }

   tim = time(NULL);
   fprintf(stderr,"\nProgram Begins  %s\n",ctime(&tim));

   cnt1=0;
   cnt2=0;

   i=2;

   if(strncmp(argv[1],"-v",2) == 0 || strncmp(argv[2],"-v",2) == 0)
   {
     i++;
     vflag=1;
   }

   if(strncmp(argv[1],"-o",2) == 0 || strncmp(argv[2],"-o",2) == 0)
   {
     i++;
     oflag=1;
   }

   j = (i-1);           /* got to be done this way I'm afraid */

   input = argv[j];	/* filename 1 assigned to pointer */

   for(l=0;l<strlen(input);l++)
   {
	if(input[l] == '*') 		/* if wildcard = 1 then */
	{				/* ls the dir for the   */
		wildcard = 1;		/* expression and read  */
	}				/* into a temp file     */
   }					/* the read this        */

   for(x=0;i<argc;i++,x++)
   {
     pattern[x] = argv[i];
     len[x] = strlen(argv[i]);
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
     okflag=0;
     found=0;
     reply=0;

     for(x=0;x<params;x++)
     {
	p1 = buf;
	k=0;
	while(k<strlen(buf))
	{
		reply = strncmp(p1,pattern[x],len[x]);
		if(reply == 0)
		{
			found++;
			break;
		}
		p1++;
		k++;
	}
     }

     if(found == params && vflag == 0 && oflag == 0)  /* ANDS WANTED */
     {
	fprintf(stdout,"%s",buf);
	cnt2++;
     }

     if(found < params && vflag == 1 && oflag == 0)   /* ANDS NOT WANTED */ 
     {
	fprintf(stdout,"%s",buf);
	cnt2++;
     }


     if(found <= params && found > 0 && vflag == 0 && oflag == 1)   /* ORS WANTED */ 
     {
	fprintf(stdout,"%s",buf);
	cnt2++;
     }

     if(found == 0 && vflag == 1 && oflag == 1)   /* ORS NOT WANTED */ 
     {
	fprintf(stdout,"%s",buf);
	cnt2++;
     }  

     read1();
   }

   fclose(inp);


   fprintf(stderr,"Records read from input file = %d\n",cnt1);
   fprintf(stderr,"Matching records             = %d\n",cnt2);
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
