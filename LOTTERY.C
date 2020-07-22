/**********************************************
 lottery.c					
 Does lottery simulation 		
 Dominic Mar 1995                             
***********************************************/
#include <stdio.h>  
#include <stdlib.h>
#include <time.h>

long bonus, z, i, flag, cntmatch, repetitions, cnt; 
long arr[7];
long lots[6];
long tots[7];
double repdbl;

time_t start;
time_t end;

int ir_random(int);

main(argc,argv)
int argc;
char *argv[];
{

if(argc < 8)
{
 fprintf(stdout,"usage is lottery <lottery ball 1 > ... <lottery ball 6> <no of weeks to run>\n");
 exit(1);
}

for(i=0;i<6;i++)
{
	lots[i]=atol(argv[i+1]);
}

repetitions = atol(argv[7]);
fprintf(stdout,"Number of weeks to run = %ld\n",repetitions);

start = time(NULL);
fprintf(stderr,"\nProgram Begins  %s\n",ctime(&start));

while(cnt < repetitions)
{

for(z=0;z<7;z++)
{
  arr[z]= ir_random(49);    
  flag = 0;               
  for(i=0;i<=z;i++)      
  {
    if(arr[i] == arr[z])
    {
      flag++;
      if(flag > 1)
      {
	 z--;
      }
    }
  }
}
/*
fprintf(stdout,"Random numbers are %ld %ld %ld %ld %ld %ld \n",arr[0],arr[1],arr[2],arr[3],arr[4],arr[5]); 
*/
bonus = arr[6];
cntmatch = 0;               

for(z=0;z<6;z++)
{
  for(i=0;i<6;i++)      
  {
    if(lots[i] == arr[z])
    {
	cntmatch++;
    }
  }
}

switch (cntmatch)
{
case 1 :	tots[0]++;break;
case 2 :	tots[1]++;break;
case 3 : 	tots[2]++;break;
case 4 : 	tots[3]++;break;
case 5 :	tots[4]++;
for(i=0;i<6;i++)
{
	if(lots[i] == bonus)
	{
		tots[5]++;
		tots[4]--;
	}
} 			 ;break;
case 6 : 	tots[6]++;break;
}

cnt++;

}

repdbl = repetitions;
fprintf(stdout,"Number of 1 Ball Matches    = %ld   (%lf%)\n",tots[0],(tots[0]/repdbl)*100.0);
fprintf(stdout,"Number of 2 Ball Matches    = %ld   (%lf%)\n",tots[1],(tots[1]/repdbl)*100.0);
fprintf(stdout,"Number of 3 Ball Matches    = %ld    (%lf%)\n",tots[2],(tots[2]/repdbl)*100.0);
fprintf(stdout,"Number of 4 Ball Matches    = %ld     (%lf%)\n",tots[3],(tots[3]/repdbl)*100.0);
fprintf(stdout,"Number of 5 Ball Matches    = %ld       (%lf%)\n",tots[4],(tots[4]/repdbl)*100.0);
fprintf(stdout,"Number of 5 + Bonus Matches = %ld         (%lf%)\n",tots[5],(tots[5]/repdbl)*100.0);
fprintf(stdout,"Number of 6 Ball Matches    = %ld\n",tots[6]);

end = time(NULL);
fprintf(stderr,"\nProgram Ends OK  %s\n",ctime(&end));

return(0);

}

ir_random(num)
int num;
{
long random, ltime;
static long sub = 0;
static int flag = 0;
static long numstore[14000000];

if(flag == 0)       
{
 flag++;
 time(&ltime);                  
 srand(ltime);                
 for(sub=0;sub<14000000;sub++)
 {
  numstore[sub] = (rand());  
 }
 sub=0;
}

if(sub > 13999998)
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

