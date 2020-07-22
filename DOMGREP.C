/**************************************************************************
PROGRAM NAME : domgrep  (source domgrep.c)
PURPOSE      : finds a given string in a given position in a file
	       an RSEL in old money.
PARAMETERS   : -v (optional) -o (optional) -g -l -b filename fieldpos1 fieldlength
	       string up to fieldpos10 fieldlength10 string10 (use -h for help)
OUTPUT       : records containing the string
	       counts of the above
COMMENTS     : This can also use standard input as input in the 
	       following way :
	       process | domgrep stdin 1 5 12345 . Note that the
	       filename parameter is replaced by the word stdin.
	       The -v parameter does the same as the grep -v, it gives
	       you the lines that do not conform to the pattern.
	       The -o parameter specifies that where more than one
	       search string is used, records that satisfy any one of
	       them will be found. In other words -o tells the program 
	       to look for ORs rather than ANDs.
	       The -g flag says that you want records only where the pattern 
               is greater than the string.
	       The -l flag says that you want records only where the pattern 
               is less than the string.
	       The -b flag says that you want records only where the pattern 
               is between than the 2 (only 2) strings.
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
int cnt1, cnt2, i, j, x, okflag, bcnt;
int vflag=0;
int oflag=0;
int lflag=0;
int gflag=0;
int bflag=0;
int params=0;
time_t tim;

main(argc,argv)
int argc;
char *argv[];
{
   if(argc < 5 && (strncmp(argv[1],"-h",2) != 0))
   {
     fprintf(stderr,"Error, usage is domgrep <filename> <fieldpos> <fieldlength> <string>... <fieldposn> <fieldlengthn> <stringn>\n");
      exit(1);
   }

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

   if(strncmp(argv[1],"-l",2) == 0 || strncmp(argv[2],"-l",2) == 0)
   {
     i++;
     lflag=1;
   }

   if(strncmp(argv[1],"-g",2) == 0 || strncmp(argv[2],"-g",2) == 0)
   {
     i++;
     gflag=1;
   }

   if(strncmp(argv[1],"-b",2) == 0 || strncmp(argv[2],"-b",2) == 0)
   {
     i++;
     bflag=1;
   }

   if(lflag == 1 && gflag == 1)
   {
	fprintf(stderr,"Don't be a pillock, you can't have -l and -g together\n");
	exit(1);
   }

   if(lflag == 1 && bflag == 1)
   {
	fprintf(stderr,"Don't be a pillock, you can't have -l and -b together\n");
	exit(1);
   }
	
   if(gflag == 1 && bflag == 1)
   {
	fprintf(stderr,"Don't be a pillock, you can't have -g and -b together\n");
	exit(1);
   }

/*   if((gflag == 1 || bflag == 1 || lflag == 1) && vflag == 1) */
   if((gflag == 1 || lflag == 1) && vflag == 1)
   {
	fprintf(stderr,"Don't use -v with -l or -g, just reverse the logic !!\n");
	exit(1);
   }

   if(bflag == 1 && (argc < 9 || argc > 10))
   {
	fprintf(stderr,"-b (between) only allows 2 strings - think it through !\n");
	exit(1);
   }


   if(strncmp(argv[1],"-h",2) == 0)
   {
	fprintf(stderr,"domgrep [-ovbglh] filename : grep by byte position (RSEL).\n");
	fprintf(stderr,"The most common usage is domgrep filename bytepos length string.\n");
	fprintf(stderr,"You can pass in up to 10 lots of the above parameters but remember that\n");
	fprintf(stderr,"these are ANDS.\n");
	fprintf(stderr,"-o before the filename makes them into normal ORs (not XORs)\n");
	fprintf(stderr,"-v before the filename says that you want everything that doesn't match\n");
	fprintf(stderr,"-l before the filename says that you want everything less than the string\n");
	fprintf(stderr,"-g before the filename says that you want everything greater than the string\n");
	fprintf(stderr,"-b before the filename says that you want everything between the 2 strings\n");
	fprintf(stderr,"-h brings up this screen\n");
	fprintf(stderr,"\ndomgrep version 2 	Dominic 3/3/96\n");
	exit(1);
   }

   tim = time(NULL);
   fprintf(stderr,"\nProgram Begins  %s\n",ctime(&tim));

   j = (i-1);           /* got to be done this way I'm afraid */

   input = argv[j];	/* filename 1 assigned to pointer */

   for(x=0;i<argc;i++,x++)
   {
     (pos[x]) = atoi(argv[i]);
     pos[x]--;
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
     okflag=0;
     reply=0;
     bcnt=0;

/*     for(i=2,x=0;i<argc;i=i+3,x++) */
     for(i=2,x=0;i<argc-1;i=i+3,x++)
     {
	p1 = buf;
	p1 = p1 + pos[x];
	reply = strncmp(p1,pattern[x],len[x]);
	
	if(bflag == 0 && lflag == 0 && gflag == 0)
	{
		if(reply != 0)
		{
		   	okflag++;
		}
	}
	else
	{
/*fprintf(stderr,"reply = %d, p1 = %s, i = %d, argc = %d\n",reply,p1,i,argc); */
		if(reply <= 0 && gflag == 1)
		{
		   	okflag++;
		}
	
		if(reply >= 0 && lflag == 1)
		{
		   	okflag++;
		}

		if(reply < 0 && bflag == 1 && bcnt == 0)
		{
		   	okflag++;
		}

		if(reply > 0 && bflag == 1 && bcnt == 1)
		{
		   	okflag++;
		}
		bcnt++;
	}

     }

     if(okflag == 0 && vflag == 0 && oflag == 0)  /* ANDS WANTED */
     {
	fprintf(stdout,"%s",buf);
	cnt2++;
     }

     if(okflag > 0 && vflag == 1 && oflag == 0)   /* ANDS NOT WANTED */ 
     {
	fprintf(stdout,"%s",buf);
	cnt2++;
     }

     if(okflag < params && vflag == 0 && oflag == 1)   /* ORS WANTED */ 
     {
	fprintf(stdout,"%s",buf);
	cnt2++;
     }

     if(okflag == params && vflag == 1 && oflag == 1)   /* ORS NOT WANTED */ 
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
