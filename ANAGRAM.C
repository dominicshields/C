/* anagram.c                           */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(argc,argv)
int argc;
char *argv[];
{
  int a, b, c, d, e, ran, mark;
  int arr[18];
  char mess[19];
  char hold[19];
  strcpy(mess,"CHRISTMASGREETINGS");

  for(a=0;a<1000;a++)
  {
	for(b=0;b<18;b++)
	{
		ran = ir_random(18);
		ran--;
		arr[b] = ran;
		mark=0;
		d=b;

		for(c=0;c<18;c++)
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

	for(b=0;b<18;b++)
	{
		hold[b] = mess[arr[b]];
	}

	hold[18] = '\0';
	fprintf(stdout,"%s\n",hold);
	strcpy(hold,"");
	for(e=0;e<18;e++)
	{
		arr[e] = 99;
	}

  }
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
