/* numeric.c                           */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>

int b,c,i,hooray;
/*float a,d, store,number;   */
long a,d, store,number;
char totstring[20];
char hold[2];
char *ptr;
time_t tim;

int main(argc,argv)
int argc;
char *argv[];
{
  tim = time(NULL);
  fprintf(stdout,"\nProgram Begins  %s\n",ctime(&tim));

/*  a=atof(argv[1]);  */
  a=atol(argv[1]);
  hooray=0;
/*  while(hooray < 2)  */
  while (!kbhit())
  {
	store=0;
	a++;
   /*	sprintf(totstring,"%f",a); */
	sprintf(totstring,"%ld",a);
	b = strlen(totstring);
	ptr=totstring;
	for(c=0;c<b;c++)
	{
		strncpy(hold,ptr,1);
		hold[1] = '\0';
	 /*	number=atof(hold); */
		number=atol(hold);

		d=number;
		for(i=1;i<number;i++)
		{
			d = (number*d);
		}
		store += d;
		ptr++;
	}

	if(store == a)
	{
	/*	fprintf(stdout,"Number that passes is %f\n",a); */
		fprintf(stdout,"Number that passes is %ld\n",a);
		hooray++;
	}

	if(a == atof(argv[2]))
	{
		fprintf(stdout,"Reached %s \n",argv[2]);
		break;
	}


  }
  fprintf(stdout,"ENDED OR INTERRUPTED AT NUMBER %ld\n",a);
  tim = time(NULL);
  fprintf(stdout,"Program ends at %s\n",ctime(&tim));
  return(0);
}


