/* prime.c                           */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>

int b,c,i,primeflag,cnt;
long a,number;
char totstring[20];
char hold[20];
char *ptr;
time_t tim;
ldiv_t lx;

int main(argc,argv)
int argc;
char *argv[];
{
  tim = time(NULL);
  fprintf(stdout,"\nProgram Begins  %s\n",ctime(&tim));

  a=atol(argv[1]);
  cnt=0;
  while (!kbhit())
  {
	primeflag=0;
	a++;
	lx = ldiv(a,2);
	if(lx.rem == 0)
	{
		goto endlab;
	}

	sprintf(totstring,"%ld",a);
	b = strlen(totstring);
	ptr=totstring;
	for(c=b;c>0;c--)
	{
		strncpy(hold,ptr,c);
		hold[c] = '\0';
		number=atol(hold);

		prime();
		if(primeflag > 0)
		{
			break;
		}
	}

	if(primeflag == 0)
	{
		fprintf(stdout,"Number that passes is %ld\n",a);
		cnt++;
	}

	if(a == atof(argv[2]))
	{
		fprintf(stdout,"Reached %s \n",argv[2]);
		break;
	}

	endlab:;
  }
  fprintf(stdout,"ENDED OR INTERRUPTED AT NUMBER %ld\n",a);
  fprintf(stdout,"Number of answers found %d\n",cnt);
  fprintf(stdout,"\nProgram Began %s\n",ctime(&tim));
  tim = time(NULL);
  fprintf(stdout,"Program ends at %s\n",ctime(&tim));
  return(0);
}

/*************************** prime ***************************/

prime()
{

 if(number == 1)
 {
	primeflag++;
	return 1;
 }

 for(i=2;i<=number/2;i++)
 {
	lx = ldiv(number,i);
	if(lx.rem == 0)
	{
		primeflag++;
		return 1;
	}
 }

 return 0;
}