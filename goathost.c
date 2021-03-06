/**********************************************
 goathost.c					
 Simulates the Monty Hall Dilemma with the difference that
the host doesn't know where the prize is
 Dominic Mar 1999                             
***********************************************/
#include <stdio.h>  
#include <stdlib.h>
#include <time.h>

#define ARRMAX 14000
#define ARRLIM (ARRMAX - 2)

long repetitions, cnt, win, lose, choices;
int prize, guess, opendoor, switchdoor;
time_t tim;

main(argc,argv)
int argc;
char *argv[];
{

if(argc < 2)
{
 fprintf(stdout,"usage is goathost <no of games to run>\n");
 exit(1);
}

tim = time(NULL);
fprintf(stderr,"\nProgram Begins  %s\n",ctime(&tim));

repetitions = atol(argv[1]);
fprintf(stdout,"Number of games to run = %ld\n",repetitions);

cnt = 0;
win = 0;
lose = 0;
choices = 0;

while(cnt < repetitions)
{

  prize = ir_random(3);
  guess = ir_random(3);
  choices++;

  if(prize == guess)
  {
	do
  	{
  		opendoor = ir_random(3);
  	}
  	while (opendoor == guess);

	if(opendoor == prize)
	{
		/* HOST HAS COCKED IT UP - BREAK OUT */
		goto label;
	}
  	lose++;
  }
  else
  {
	do
  	{
  		opendoor = ir_random(3);
  	}
  	while (opendoor == guess);

	if(opendoor == prize)
	{
		/* HOST HAS COCKED IT UP - BREAK OUT */
		goto label;
	}
  	
  	do
  	{
  		switchdoor = ir_random(3);
  	}
  	while (switchdoor == opendoor || switchdoor == guess);

  	if(switchdoor == prize)
  	{
  		win++;
  	}
	else	
	{
		lose++;
		printf("Hello, I never get executed\n");
	}
  }

   cnt++;

label:
  opendoor = 0;
  switchdoor = 0;
 
}

fprintf(stdout,"Changed and won  = %ld\n",win);
fprintf(stdout,"Changed and lost = %ld\n",lose);
fprintf(stdout,"Actual Choices By Contestant = %ld\n",choices);

tim = time(NULL);
fprintf(stderr,"\nProgram Ends OK  %s\n",ctime(&tim));

return(0);

}

ir_random(num)
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

if(sub > ARRLIM)
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

