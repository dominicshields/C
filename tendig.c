#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

char numhold[11];
double num, inter, upper, i;
int w, x, y, z, len;
int flag = 0;
int match = 0;
char test[11];
time_t first, second;

main(argc,argv)
int argc;
char *argv[];
{
first = time(NULL);
fprintf(stderr,"\nProgram Begins  %s\n",ctime(&first));

/* num = 3816547290; */
num = (double) 1234567890;
upper = (double) 9876543210;

printf("num = %lf, Upper = %lf\n\n\n",num,upper);

while (num < upper)
{
     for(w=0;w<11;w++)
     {
          test[w] = '\0';
     }
     sprintf(numhold,"%lf",num);
     for(i=1;i<11;i++)
     {
          len = (int) i;
          strncpy(test,numhold,len);
          inter = atof(test);
          z = (int) fmod(inter,i);
          if(z != 0)
          {
               flag = 1;
               break;
          }
     }
     if (flag == 0)
     {
/*        fprintf(stdout,"Candidate Number %s\n",numhold);*/

          for(x=0;x<10;x++)
          {
               for(y=0;y<10;y++)
               {
                    if(numhold[x]==numhold[y])
                    match++;
               }
          }
          if (match == 10)
          {
               fprintf(stdout,"\n*****************************************\n");
               fprintf(stdout,"The number that works is %s\n",numhold);
               fprintf(stdout,"*****************************************\n");
               fflush(stdout);
               goto end;
          }
     }
     match = 0;
     flag = 0;
     num++;
     if(fmod(num,1000000000) == 0)
     {
          second = time(NULL);
          fprintf(stderr,"\n%ld reached %s\n",num,ctime(&second));
     }
}

end:
second = time(NULL);
fprintf(stderr,"\nProgram Ends  %s\n",ctime(&second));
return 0;
}