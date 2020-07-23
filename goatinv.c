/**********************************************
 goatinv.c					
 Simulates the Monty Hall Dilemma inverted
 i.e two cars and one goat
 Dominic Apr 1999                             
***********************************************/
#include <stdio.h>  
#include <stdlib.h>
#include <time.h>

#define ARRMAX 14000
#define ARRLIM (ARRMAX - 2)

long repetitions, cnt, win, lose;
int prize, goat, guess, opendoor, switchdoor;
time_t tim;

main(argc,argv)
int argc;
char *argv[];
{

if(argc < 2)
{
 fprintf(stdout,"usage is goatinv <no of games to run>\n");
 exit(1);
}

tim = time(NULL);
fprintf(stderr,"\nProgram Begins  %s\n",ctime(&tim));

repetitions = atol(argv[1]);
fprintf(stdout,"Number of games to run = %ld\n",repetitions);

cnt = 0;
win = 0;
lose = 0;

while(cnt < repetitions)
{

  goat = ir_random(3);
  guess = ir_random(3);
  
  if(goat == guess)
  {
	/* IF YOU'RE ALWAYS GOING TO STAY PUT ITS THE ONLY TIME YOU CAN'T WIN */
  	lose++;
  }
  else
  {
	/* ALL THE CODE IN THIS ELSE BLOCK IS ACADEMIC */
  	do
  	{
  		opendoor = ir_random(3);
  	}
  	while (opendoor == goat || opendoor == guess);

  	do
  	{
  		switchdoor = ir_random(3);
  	}
  	while (switchdoor == opendoor || switchdoor == guess);

  	if(switchdoor == goat)
  	{
  		win++;
  	}
	else	
	{
		lose++;
	} 

  }
  
  opendoor = 0;
  switchdoor = 0;
  cnt++;
}

fprintf(stdout,"Stay put and won  = %ld\n",win);
fprintf(stdout,"Stay put and lost = %ld\n",lose);

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

