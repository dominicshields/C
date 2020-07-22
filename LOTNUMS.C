/*-----------------------------------------------------------------
PROGRAM NAME : lotnums (source lotnums.c)

Description  : Lists all possible lottery entries
	       
Author       : Dominic Shields
-----------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

FILE *outp;

int lots[6];
long ops;
time_t start;
time_t end;

main(argc,argv)
int argc;
char *argv[];
{
start = time(NULL);
fprintf(stderr,"\nProgram Begins  %s\n",ctime(&start));
   
outp = fopen("allnums.txt","w");

lots[0]=1;
lots[1]=2;
lots[2]=3;
lots[3]=4;
lots[4]=5;
lots[5]=6;
ops = 6;

while ( lots[0] < 45)  
{
	while ( lots[1] < 46)  
   	{
   		while ( lots[2] < 47)  
   		{
   			while ( lots[3] < 48)  
   			{
   				while ( lots[4] < 49)  
   				{
   					while ( lots[5] < 50)  
   					{
    					fprintf(outp,"%d\t",lots[0]);
    					fprintf(outp,"%d\t",lots[1]);
    					fprintf(outp,"%d\t",lots[2]);
    					fprintf(outp,"%d\t",lots[3]);
    					fprintf(outp,"%d\t",lots[4]);
    					fprintf(outp,"%d\n",lots[5]);
    					lots[5]++;
    					ops += 8;
   					}
   				lots[4]++;
   				lots[5]=lots[4]+1;
   				ops += 3;
   				}
   			lots[3]++;
   			lots[4]=lots[3]+1;
   			lots[5]=lots[4]+1;
   			ops += 4;
   			}
   		lots[2]++;
   		lots[3]=lots[2]+1;
   		lots[4]=lots[3]+1;
   		lots[5]=lots[4]+1;
   		ops += 5;
   		}
	lots[1]++;
	lots[2]=lots[1]+1;
	lots[3]=lots[2]+1;
	lots[4]=lots[3]+1;
	lots[5]=lots[4]+1;
	ops += 6;
	}
lots[0]++;
lots[1]=lots[0]+1;
lots[2]=lots[1]+1;
lots[3]=lots[2]+1;
lots[4]=lots[3]+1;
lots[5]=lots[4]+1;
ops += 7;
}

end = time(NULL);
fprintf(stderr,"Program Ends OK  %s\n",ctime(&end));
fprintf(stderr,"Number of seconds =\t%5.0lf\n",difftime(end,start));
fprintf(stderr,"Number of operations =\t%ld\n",ops);
fprintf(stderr,"Operations per second =\t%11.2lf\n",ops/difftime(end,start));
fprintf(stderr,"\nRemember that this program has created a 244MB file - allnums.txt\n");

return 0;
}
