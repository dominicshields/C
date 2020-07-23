/* Reformats files where each field is listed as a new record delimited by newline example

Position
BC
Number
Time
CName
Sname
Club
Sex
Males
Females
Age Group
Males
Females
Age Rank

*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define RECLEN 1000     /* Max record length of input file  */
#define FIELDS 12

		      						/* file pointers */
FILE *inp;            /* input file    */
FILE *out;            /* output file   */

char *input;
char *output;

char buf[RECLEN];       /* buffer area to read input file */
char outstr[RECLEN];
long fieldcnt, writecnt;
int i;

main(argc,argv)
int argc;
char *argv[];
{
   if(argc < 2)
   {
     fprintf(stderr,"Error, usage is %s <inputfile> <outputfile>\n",argv[0]);
      exit(1);
   }

   input = argv[1];            /* input filename assigned to pointer */
   output = argv[2];           /* output filename assigned to pointer */


   if ((inp = fopen(input,"r")) == NULL)
   {
      fprintf(stderr,"Error opening %s does it exist?\n",input);
      exit(1);
   }

   if ((out = fopen(output,"w")) == NULL)
   {
      fprintf(stderr,"Error opening %s\n",output);
      exit(1);
   }

   fieldcnt=0;
   writecnt=0;

   *fgets(buf,RECLEN,inp);
/*    fieldcnt++; */
/*    strncpy(outstr,buf,strlen(buf)-1);*/

   while ( feof(inp) == 0 )
   {
   	if(fieldcnt < FIELDS)
   	{
   		strncat(outstr,buf,strlen(buf)-1);
   		strcat(outstr,",");
  	}
  	else
  	{
  	 strcat(outstr,"\n");
     fputs(outstr,out);
     for(i=0;i<RECLEN;i++)
    {
      outstr[i] = '\0';    /* DEAL WITH VARIABLE LENGTH RECORDS THOROUGHLY */
    }
     
     writecnt++;
     fieldcnt=0;
     strncpy(outstr,buf,strlen(buf)-1);
     strcat(outstr,",");
   }

    *fgets(buf,RECLEN,inp);
    fieldcnt++;
   }

  fclose(inp);
  fclose(out);

  fprintf(stdout,"Wrote %ld records to %s\n",writecnt,output);

  return 0;

 }

