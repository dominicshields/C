/**************************************************************************
PROGRAM NAME : newaddcdf  (source newaddcdf.c)
PURPOSE      : takes the output from cdf sorted and piped to stdin
	       and gives disk statistics in Megabytes
PARAMETERS   : this is run from the shell script cdfmb 
OUTPUT       : Megabytes free, used and total for a named partition
COMMENTS     : none
**************************************************************************/
#include <string.h>
#include <stdio.h>

FILE *inp;           /* input file */

char *p1;
char *p2;
char *p3;
char *input;
char *lookfor;
char buf[500];        /* buffer area to read input file */
char hold[8];
char holdlook[15];
int cnt, len, flag, pos;
float tot1, tot2, tot3, num1, num2, num3, percent;

main(argc,argv)
int argc;
char *argv[];
{
   int reply;
   char ans[1];
   if(argc < 2)
   {
     fprintf(stderr,"Error, usage is newaddcdf <filename> <diskarea>\n");
      exit(1);
   }

   lookfor = argv[1];
   len = strlen(lookfor);
   inp = stdin;

   tot1=0;
   tot2=0;
   tot3=0;
   flag=0;

   fprintf(stdout,"\n   Partition        Megabytes Free    Megabytes Used    Total Megabytes  %%Free\n");

   read1();

   while (feof(inp) == 0)
   {
     p1 = buf;

     if(strncmp(lookfor,"all",3) != 0)   /* IF A NAMED PARTITION REQUIRED */
     {

     	for(p1=buf;p1[0]!='/';p1++)
     	{
	}

        pos = strcspn(p1," ");

        strncpy(holdlook,p1,pos);

     	for(;p1[0]!=' ';p1++)
     	{
        	if(strncmp(p1,lookfor,len) == 0)
        	{
       			flag = 1;
        	}
        } 
	if(flag == 1)
	{
       	        flag = 0;	
                p1 = buf;
                p1 = p1 + 34; 
       		addup();
	}
     }
     else
     {
       for(p1=buf;p1[0]!='/';p1++)
       {
       }

       pos = strcspn(p1," ");

       strncpy(holdlook,p1,pos);

       p1 = buf;
       p1 = p1 + 34; 
       addup();
     }
     
     read1();
   }


  fclose(inp);

  if(tot3 > 0)
  {
     percent=(tot1/tot3)*100; 
     fprintf(stdout,"\n");
     fprintf(stdout,"Total Megabytes free in %s = %f\n",lookfor,(tot1*512/1024)/1024);
     fprintf(stdout,"Total Megabytes used in %s = %f\n",lookfor,(tot2*512/1024)/1024);
     fprintf(stdout,"Total Megabytes in %s      = %f\n",lookfor,(tot3*512/1024)/1024);
     fprintf(stdout,"Percentage free in %s      = %3.1f\n",lookfor,percent);
     fprintf(stdout,"N.B. One Megabyte = 1048576 bytes\n");
     fprintf(stdout,"\n");
  }

  return 0;

 }

 /***************************   read1  ******************************/

 read1()
 {
  *fgets(buf,500,inp);
  return 0;
 }

/****************************   addup   *******************************/

addup()
{
 num1=num2=num3=0;

       strcpy(hold,"");
       strncpy(hold,p1,8);
       num1 = atoi(hold);
       tot1 = tot1 + num1;

       p1 = p1 + 9;
       strcpy(hold,"");
       strncpy(hold,p1,8);
       num2 = atoi(hold);
       tot2 = tot2 + num2;

       p1 = p1 + 9;
       strcpy(hold,"");
       strncpy(hold,p1,8);
       num3 = atoi(hold);
       tot3 = tot3 + num3;

       if(num3 > 0)              /* PROHIBITS ZERO DIVIDE  */
       {
       num1 = ((num1*512)/1024/1024);
       num2 = ((num2*512)/1024/1024);
       num3 = ((num3*512)/1024/1024);
       percent = (num1/num3) * 100; 
       
fprintf(stdout,"%14s        %9.4f         %9.4f         %9.4f      %3.1f\n",holdlook,num1,num2,num3,percent); 
       }
       strcpy(buf,"");
       strcpy(holdlook,"");
     
}
