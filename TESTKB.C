/* testkb.c    testing kbhit()  written on borland C    */
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

main(argc,argv)
int argc;
char *argv[];
{
int cnt=0;
time_t tim, tim2;

   tim = time(NULL);

   while(!kbhit())
   {
    fprintf(stdout,"Stop the program \n");
    cnt++;
   }

   tim2 = time(NULL);

   fprintf(stdout,"Program %s started %s\n",argv[0],ctime(&tim));
   fprintf(stdout,"Looped %d times ended at %s\n",cnt,ctime(&tim2));

   return(0);
}
