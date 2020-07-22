/**************************************************************************
PROGRAM NAME : lotsort  (source lotsort.c)
PURPOSE      : Sorts fields within each record then sorts the file
PARAMETERS   : input file output file 
OUTPUT       : Your original file sorted as you asked
COMMENTS     : Make sure you don't interrupt this job during the mv
	       or you'll lose the lot and make sure that there's space
	       to do it or you'll also lose the lot.
WRITTEN BY   : Dominic
**************************************************************************/
#include <stdio.h>
#include <time.h>

FILE *inp;		/* input file    */
FILE *out;		/* output file    */

int comp( );

char *input;
char *output;

char arr[6][3];
char unixcmd[100];
char buf[500];
char *ptr;
time_t tim;

main(argc,argv)
int argc;
char *argv[];
{
   int reply, i, x;

   input = argv[1];	/* input file name assigned to pointer */
   output = argv[2];    /* output file etc */

   tim = time(NULL);
   fprintf(stderr,"\nProgram %s Begins  %s\n",argv[0],ctime(&tim));

   if(argc ==0)
   {}

   if ((inp = fopen(input,"r")) == NULL)
   {
      fprintf(stderr,"Error opening %s does it exist?\n",input);
      exit(1);
   }

   if ((out = fopen(output,"w")) == NULL)
   {
      fprintf(stderr,"Error creating %s\n",output);
      exit(1);
   }

   fgets(buf,500,inp);
   while(!feof(inp)) /* == 0)     */
   {
	ptr = buf;
	for(i=0;i<6;i++)
	{
		strncpy(arr[i],ptr,2);
		strcat(arr[i],"\0");
/*		fprintf(stdout,"arr[i] = %s\n",arr[i]); */
		ptr +=3;
	}

	qsort((void *)arr,6,3,comp);

	for(i=0;i<5;i++)
	{
		fprintf(out,"%s ",arr[i]);
	}
	fprintf(out,"%s\n",arr[5]);

	fgets(buf,500,inp);
    }
/*
   sprintf(unixcmd,"sort %s > tmpsrt",input);
   fprintf(stderr,"%s\n",unixcmd);
   system(unixcmd);

   strcpy(unixcmd,"");
   sprintf(unixcmd,"mv tmpsrt %s",input);
   fprintf(stderr,"%s\n",unixcmd);
   system(unixcmd);
*/
   fclose(inp);
   fclose(out);

   tim = time(NULL);
   fprintf(stderr,"\nProgram %s Ends  %s\n",argv[0],ctime(&tim));
   return 0;
}

int comp(const void *a, const void *b)
{
	return(strcmp(a,b));
}
