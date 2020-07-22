/* numfind.c                           */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

FILE *outp;

int main(argc,argv)
int argc;
char *argv[];
{
  ldiv_t lx;
  int a,b,c,d,cnt,hooray,ran,mark;
  int arr[9];
  long cnt2=0;
  long num=0;
  char hold[10];
  char totstring[10];
  time_t tim;

  if(argc > 5)            /* this doesn't do anything but I was sick */
  {                       /* of the Borland C complier grumbling     */
    *argv[4] = 1;
  }
  tim = time(NULL);
  fprintf(stdout,"\nProgram Begins  %s\n",ctime(&tim));

  if ((outp = fopen("numtest","w")) == NULL)
  {
      fprintf(stderr,"Error opening numtest\n");
      exit(1);
   }

  strcpy(totstring,"123456789");
  totstring[9]='\0';
  hooray=0;

  while(hooray == 0)
  {
	cnt=0;
	for(a=1;a<10;a++)
	{
		strncpy(hold,totstring,a);
		hold[a]='\0';
		num=atol(hold);
		lx = ldiv(num,a);
		if(lx.rem == 0)
		{
			cnt++;
		}
		else
		{
			break;
		}
	 }
	 if(cnt == 9)
	 {
		hooray=1;
	 }
	 strncpy(hold,totstring,9);
	 hold[9]='\0';

	 for(b=0;b<9;b++)
	 {
		ran = ir_random(9);
			ran--;
		arr[b] = ran;
		mark=0;
		d=b;

		for(c=0;c<9;c++)
		{
			if(arr[c] == arr[d])
			{
				mark++;
				if(mark > 1)
				{
					b--;
					mark=0;
				}
			}
		}
	}

	for(b=0;b<9;b++)
	{
		totstring[b] = hold[arr[b]];
	}
	totstring[9]='\0';
	for(b=0;b<9;b++)
	{
		arr[b] = 99;
	}
	hold[0]='\0';
	cnt2++;
	lx = ldiv(cnt2,100000);
	if(lx.rem == 0)
	{
		printf("LOOPED %ld   times\n",cnt2);
	}


  }

  tim = time(NULL);
  fprintf(stdout,"The number is %ld   :  Time is %s\a\n",num,ctime(&tim));
  return(0);
}

ir_random(num)
int num;
{
   int random;
   time_t t;
   static int flag=0;
   static long sub;
   static long numstore[10000];

   if(flag == 0)
   {
	flag++;
	srand((unsigned) time(&t));
	for(sub=0;sub<10000;sub++)
	{
		numstore[sub] = (rand());
	}
	sub=0;
   }

   if(sub > 9998)
   {
	flag = 0;
   }
   random = (((numstore[sub]) % num) + 1);
   sub++;
   return(random);
}
