/**************************************************************************
PROGRAM NAME : wordcnt  (source wordcnt.c)
PURPOSE      :
PARAMETERS   :
OUTPUT       :
COMMENTS     : none
**************************************************************************/
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define RECLEN 1000     /* max record length of input file     */
#define ARRMAX 1000     /* max number of words array can store */

FILE *inp;		/* input file */

int comp( );
int compint( );

struct wordarr {
	char wordstore[25];
	int wordcnt;
};
struct wordarr word[ARRMAX];

char sortarr[ARRMAX][25];

int sortcnt[ARRMAX];

char *p1;
char *p2;
char input[60];
char buf[RECLEN];		/* buffer area to read input file */
int arrsub=0;

/************************  main  ************************/

main()
{
   int reply, no_reqd, i, j, cutoff;
   float k, scale, max_occurs;
   fprintf(stdout,"Please enter the full pathname of the text file\n");

   scanf("%s",input);
   fflush(stdin);

   fprintf(stdout,"Please enter the number of highest frequency required\n");
   scanf("%d",&no_reqd);
   fflush(stdin);

   if ((inp = fopen(input,"r")) == NULL)
   {
      fprintf(stderr,"Error opening %s does it exist?\n",input);
      exit(1);
   }

   read1();

   while (feof(inp) == 0)
   {
	searchline();
	read1();
   }
   fclose(inp);

   for(i=0;i<arrsub;i++)
   {
	strcpy(sortarr[i],word[i].wordstore);
   }

   qsort((void *)sortarr,arrsub,25,comp);

   for(i=0;i<arrsub;i++)
   {
	sortcnt[i] = word[i].wordcnt;
   }

   qsort(sortcnt,arrsub,sizeof(int),compint);

   i=(arrsub-no_reqd);
   cutoff = sortcnt[i];
   max_occurs = (float) sortcnt[arrsub-1];
   scale = 1;
   if(max_occurs > 30)
   {
	scale=(float) (max_occurs/30);
   }


   fprintf(stdout,"Word                   Occurrences\n\n");

   for(i=0;i<arrsub;i++)
   {
	for(j=0;j<arrsub;j++)
	{
		if(strcmp(sortarr[i],word[j].wordstore) == 0)
		{
			if(word[j].wordcnt >= cutoff)
			{
				fprintf(stdout,"%-25s  %-5d          ",sortarr[i],word[j].wordcnt);
				for(k=0;k<word[j].wordcnt/scale;k++)
				{
					fprintf(stdout,"*");
				}
				fprintf(stdout,"\n");
			}
			break;
		}
	}
   }

   return 0;

 }

/************************  searchline  ************************/

 searchline()
 {
   int a, b, c, found;
   char hold[25];
   p1 = buf;
   p2 = buf;

   while(p2[0] != '\n')
   {
	while(p2[0] == ' ' || (ispunct(p2[0])))
	{
		p1++;
		p2++;
	}

	b=0;
	while(!(isspace(p2[0])) && !(ispunct(p2[0])))
	{
		p2++;
		b++;

	}

	found=0;

	for (c=0;c<b;c++)
	{
		hold[c] = tolower(p1[c]);
	}
	hold[b] = '\0';

	for(a=0;a<arrsub;a++)
	{
		if (strcmp(hold,word[a].wordstore) == 0)
		{
			found=1;
			break;
		}
	}

	if(found == 0)
	{
		if(isalnum(hold[0]))
		{
			strcpy(word[arrsub].wordstore,hold);
			word[arrsub].wordcnt=1;
			arrsub++;
		}
	}
	else
	{
		(word[a].wordcnt)++;
	}
	p1=p2;

   }

 return 0;
 }

/************************  read1  ************************/

 read1()
 {

   fgets(buf,RECLEN,inp);

   return 0;
 }

/************************  comp  ************************/

int comp(const void *a, const void *b)
{
   return( strcmp(a,b) );
}

/************************  compint  ************************/

int compint(const int *c, const int *d)
{
   return *c-*d;
}
