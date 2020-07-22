/* numeric2.c  */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

long lrand48 ();
void srand48 (); 
ldiv_t lx;
long a,cnt,ran,winnings,num,hold;
time_t tim;

int main(argc,argv)
int argc;
char *argv[];
{
  tim = time(NULL);
  fprintf(stdout,"\nProgram Begins  %s\n",ctime(&tim));

  a=0;
  winnings=0;

  while(a < 1001)
  {
	a++;
	ran = ir_random(1000000000);
	hold = 2;
	num = 2;
	cnt=0;
	while(num <= ran)
	{
		num = hold;
		num = (num*num);
		lx = ldiv(ran,num);
		if(lx.rem == 0)
		{
			cnt++;
			break;
		}
		hold++;
	}

	if(cnt == 0)
	{
		winnings +=2;
	}
  }

  tim = time(NULL);
  fprintf(stdout,"(a) You leave the casino clutching %ld sovs guv\n",winnings);
  fprintf(stdout,"Program ends at %s\n",ctime(&tim));
  return(0);
}



ir_random(num)
long num;
{
   long random;
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

