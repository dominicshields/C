/*---------------------------------------------------------------------
delbut.c : Deletes all files except those specified at the command line

----------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define LEN 80

FILE *infil_ptr;

int i, x, cnt, sub, flag, ret_val, wildflag;
char namearr[LEN][LEN];
char buf[LEN];
char system_call[200];
char ans[1];
char *p1;
char *filename;


main(argc,argv)
int argc;
char *argv[];
{
   if(argc < 2)
   {
	fprintf(stdout,"Error : usage is %s <filename1>....<filenameN>\n",argv[0]);
	fprintf(stdout,"If you don't enter at least 1 filename you might as well use del *.*\n",argv[0]);
	exit(1);
   }
   
   sub=0;
   for(i=1;i<argc;i++)
   {
	filename=argv[i];
	for(x=0;x<strlen(filename);x++)
	{
		wildflag=0;
		if(filename[x] == '*' || filename[x] == '?')
		{
			dirlook();
			wildflag=1;
			break;
		}
	}

	if(wildflag == 0)
	{	
		strcpy(namearr[sub],filename);
		for(x=0;x<(strlen(namearr[sub]));x++)
		{
			namearr[sub][x] = tolower(namearr[sub][x]);
		}
		sub++;
	}
   }

   fprintf(stdout,"\n");

   for(i=0;i<sub;i++)
   {
	fprintf(stdout,"%s\n",namearr[i]);
   }

   fprintf(stdout,"\nThe action I will take is to keep the %d file(s) listed above\n",sub);
   fprintf(stdout,"and delete all the others in this directory\n\n");
   fflush(stdin);
   fprintf(stdout,"Are you sure you want to proceed ? y or n <return>\n");

   *ans = getchar();
   fflush(stdin);

   if(tolower(*ans) != 'y')
   {
	fprintf(stdout,"Abandoning deletions\n");
	exit(1);
   }

   strcpy(system_call,"DIR /b > z@@#####.zzz");

   system(system_call);

   readfile(1);

   if(cnt != sub)
   {
	fprintf(stdout,"\nOh dear, I couldn't find all the files you listed\n");
	fprintf(stdout,"As this could mean that you are in the wrong directory\n");
	fprintf(stdout,"I'm outa here - no files have been deleted\n");
	exit(1);
   }

   fprintf(stdout,"\nDeleting files .... tense isn't it ?\n");

   readfile(2);

   remove("z@@#####.zzz"); 
   fprintf(stdout,"\nFinished deleting files - if you want them back they are in c:\\temp\n");

   return(0);
}

/************************** dirlook *************************/
dirlook()
{
   sprintf(system_call,"DIR %s /A:-D /B /L > z@@#####.zzz",filename);
   system(system_call);

   if ((infil_ptr = fopen("z@@#####.zzz","r")) == NULL)
   {
        fprintf(stdout,"Error opening z@@#####.zzz\n");
        exit(1);
   }

   read1();
   
   while(feof(infil_ptr) == 0)
   {
        strcpy(namearr[sub],buf);
	sub++;
        read1();
   }
        
   fclose(infil_ptr);
   remove("z@@#####.zzz");
   return(0);
}

/************************** read1 *************************/
read1()
{
   fgets(buf,500,infil_ptr);
   p1 = buf;
   while(!isspace(p1[0]))
   {
	p1++;
   }
   p1[0]='\0';

   for(x=0;x<(strlen(buf));x++)
   {
	buf[x] = tolower(buf[x]);
   }
   return(0);
}

/************************** readfile *************************/
readfile(int j)
{
   cnt=0;

   if((infil_ptr = fopen("z@@#####.zzz","r")) == NULL)
   {
	fprintf(stdout,"Error opening directory listing file ... exiting\n");
	exit(1);
   }

   read1();

   while(feof(infil_ptr) == 0)
   {
	flag=0;
	for(i=0;i<=sub;i++)
	{
		if(strcmp(buf,namearr[i]) == 0)
		{
			cnt++;
			flag=1;
		}
	}
	
	if(flag == 0 && j == 2 && strcmp(buf,"z@@#####.zzz") != 0)
	{
		delfiles();
	}	

	for(i=0;i<LEN;i++)
	{
		buf[i]='\0';
	}

   read1();
   }

   fclose(infil_ptr);
   return(0);

}

/************************** delfiles *************************/
delfiles()
{

   sprintf(system_call,"copy %s c:\\temp",buf);

   ret_val = system(system_call);
   if(ret_val != 0)
   {
	fprintf(stdout,"Copy to trashcan (Directory C:\\TEMP failed for some reason .. exiting\n");
	exit(1);
   }

   remove(buf);
   return(0);
}
