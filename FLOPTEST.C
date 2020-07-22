/**************************************************************************
PROGRAM NAME : floptest  (source floptest.c)
PURPOSE      : find out how many files will fit on the root dir of a floppy disk. 
PARAMETERS   : none
OUTPUT       : lots of files on the a: drive
COMMENTS     : pointless, sad etc but all in the IOS
**************************************************************************/
#include <stdio.h>

FILE *out;            /* output file   */

char *output;

long cnt=1;
int ret_val;

main(argc,argv)
int argc;
char *argv[];
{
if(argc < 2)
{
	fprintf(stdout,"Error : Usage is %s <num> (where num = 1, 2 or 3)\n\n",argv[0]);
	fprintf(stdout,"1 = Find out how many files fit on the root of A:\n");
	fprintf(stdout,"2 = Find out how many files fit in subdir A:\\newdir (takes a while)\n");
	fprintf(stdout,"3 = Find out how many subdirs will fit on the root of A:\\newdir\n");
	exit(1);
}

if(atoi(argv[1]) == 1)
{
   while(cnt < 1000)
   {
	sprintf(output,"a:\\file%ld",cnt); 

	if ((out = fopen(output,"w")) == NULL)
	{
		fprintf(stdout,"Error opening %s, can't write any more\n",output);
		fprintf(stdout,"%ld files written\n",cnt-1);
		fprintf(stdout,"If you want more files on the floppy - put them in a subdirectory \n");
		exit(1);
	}
	fprintf(out,"Hello\n");
	fclose(out);
	fprintf(stdout,"Written %ld files\n",cnt);
	cnt++;
   }
   fprintf(stdout,"%ld files written\n",cnt);
}

if(atoi(argv[1]) == 2)
{
   sprintf(output,"a:\\newdir"); 
   mkdir(output);
   while(cnt < 10000)
   {
	sprintf(output,"a:\\newdir\\file%ld",cnt); 

	if ((out = fopen(output,"w")) == NULL)
	{
		fprintf(stdout,"Error opening %s, can't write any more\n",output);
		fprintf(stdout,"%ld files written\n",cnt-1);
		exit(1);
	}
	fprintf(out,"Hello\n");
	fclose(out);
	fprintf(stdout,"Written %ld subdir files\n",cnt);
	cnt++;
   }
   fprintf(stdout,"%ld subdirectory files written\n",cnt);
}

if(atoi(argv[1]) == 3)
{
   while(cnt < 1000 && ret_val == 0)
   {
	sprintf(output,"a:\\dir%ld",cnt); 
	ret_val = mkdir(output);
	if(ret_val == 0)
	{
		fprintf(stdout,"Written %ld dirs\n",cnt);
		cnt++;
	}
   }
}

return(0);

}
