/**************************************************************************
PROGRAM NAME : tab  (source tab.c)
PURPOSE      : 
PARAMETERS   : inputfile 
OUTPUT       : 
**************************************************************************/
#include <string.h>
#include <stdio.h>
		      /* file pointers */
FILE *inp;            /* input file    */
FILE *out;            /* output file   */

char *input;
char *output;
char *p1;
char *p2;
char buf[1000];       /* buffer area to read input file */
char hold[1000];
char fighold[10];
int adjust, cnt, cnt1, i, fig;

main(argc,argv)
int argc;
char *argv[];
{
   if(argc < 3)
   {
     fprintf(stderr,"Error, usage is tab <inputfile> adjustment\n");
      exit(1);
   }

   input = argv[1];            /* input filename assigned to pointer */
   adjust = atoi(argv[2]);
   
   if ((inp = fopen(input,"r")) == NULL)
   {
      fprintf(stderr,"Error opening %s does it exist?\n",input);
      exit(1);
   }

   fgets(buf,1000,inp);
   
   while (feof(inp) == 0) 
   {
	cnt=0;
   	p1=buf;
	while(strncmp(p1,".t",2) != 0)
	{	
		p1++;
		cnt++;
	}
	if(strncmp(p1,".tab",4) == 0 || strncmp(p1,".t",2) == 0)
	{
		sortitout();
	}
/*	printf("buf  = %s\n",buf);fflush(stdout); */
	fprintf(stdout,"%s",hold);
	fgets(buf,1000,inp);		
   }
     
   fclose(inp);
  
   return 0;
}

sortitout()
{			
	for(i=0;i<1000;i++)
	{
		hold[i]= '\0';
			}
	for(i=0;i<10;i++)
	{
		fighold[i]= '\0';
	}
	strncpy(hold,buf,cnt);

	while(!isdigit(p1[0]))
	{
		strncat(hold,p1,1);
		p1++;
	}	
	p2=p1;
	cnt1=1;
	p2++;
	while(isdigit(p2[0]))
	{
		p2++;
		cnt1++;
	}	
	strncpy(fighold,p1,cnt1);
	fig=atoi(fighold);
	fig = fig + adjust;
	sprintf(fighold,"%d",fig);
	strcat(hold,fighold);
	strcat(hold,p2);
  return 0;
}
