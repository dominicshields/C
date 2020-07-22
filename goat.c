/**********************************************
 goat.c
 Simulates the Monty Hall Dilemma
 just_some_guy March 1999
***********************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRMAX 10000000
long repetitions, cnt, win, lose;
int prize, guess, opendoor, switchdoor;
time_t tim;

int random_nums(int);

int main(int argc,char *argv[])
{

if(argc < 2)
{
 fprintf(stdout,"usage is goat <no of games to run>\n");
 exit(1);
}

repetitions = atol(argv[1]);
fprintf(stdout,"Number of games to run = %ld\n",repetitions);

tim = time(NULL);
fprintf(stderr,"Program Begins  %s",ctime(&tim));

cnt = 0;
win = 0;
lose = 0;

while(cnt < repetitions)
{
  prize = random_nums(3);
  guess = random_nums(3);

  if(prize == guess)
  {
  	lose++;
  }
  else
  {
  	do
  	{
  		opendoor = random_nums(3);
  	}
  	while (opendoor == prize || opendoor == guess);

  	do
  	{
  		switchdoor = random_nums(3);
  	}
  	while (switchdoor == opendoor || switchdoor == guess);

  	if(switchdoor == prize)
  	{
  		win++;
  	}
  }

  prize = 0;
  guess = 0;
  opendoor = 0;
  switchdoor = 0;
  cnt++;
}

fprintf(stdout,"Changed and won  = %ld\n",win);
fprintf(stdout,"Changed and lost = %ld\n",lose);
tim = time(NULL);
fprintf(stderr,"Program Ends OK  %s",ctime(&tim));
return(0);
}

random_nums(num)
int num;
{
long random, ltime;
static long sub = 0;
static int flag = 0;
static long numstore[ARRMAX];

if(flag == 0)
{
 flag++;
 time(&ltime);
 srand(ltime);
 for(sub=0;sub<ARRMAX;sub++)
 {
  numstore[sub] = (rand());
 }
 sub=0;
}

if(sub > ARRMAX -1)
{
 flag=0;
}

if (num < 1)
{
 random = (numstore[sub]);
}
else
{
 random = (((numstore[sub]) % num) + 1);
}
sub++;
return(random);
}
