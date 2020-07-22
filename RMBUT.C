/*---------------------------------------------------------------------
rmbut.c	 : Deletes all files except those specified at the command line

----------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define LEN 50

FILE *infil_ptr;

int i, cnt, flag, ret_val, len;
char namearr[LEN][LEN];
char buf[LEN];
char system_call[200];
char ans[1];
char *p1;

main(argc,argv)
int argc;
char *argv[];
{
if(argc < 2)
{
	fprintf(stdout,"Error : usage is %s <filename1>....<filenameN>\n",argv[0]);
	fprintf(stdout,"If you don't enter at least 1 filename you might as well use rm *\n",argv[0]);
	exit(1);
}

for(i=1;i<argc;i++)
{
	strcpy(namearr[i],argv[i]);
}

fprintf(stdout,"\n");

for(i=1;i<argc;i++)
{
	fprintf(stdout,"%s\n",namearr[i]);
}

fprintf(stdout,"\nThe action I will take is to keep the %d file(s) listed above\n",(argc-1));
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
	
strcpy(system_call,"ls -1 > /tmp/dominic");

system(system_call);

readfile(argc,1);

if(cnt != argc-1)
{
	fprintf(stdout,"\nOh dear, I couldn't find all the files you listed\n");
	fprintf(stdout,"As this could mean that you are in the wrong directory\n");
	fprintf(stdout,"I'm outa here - no files have been deleted\n");
	exit(1);
}

fprintf(stdout,"\nDeleting files .... tense isn't it ?\n");

readfile(argc,2);

remove("/tmp/dominic");
fprintf(stdout,"\nFinished deleting files - if you want them back they are in /tmp\n");

return(0);
}

readfile(int args,int j)
{
cnt=0;

if((infil_ptr = fopen("/tmp/dominic","r")) == NULL) 
{
	fprintf(stdout,"Error opening directory listing file ... exiting\n");
	exit(1);
}

fgets(buf,500,infil_ptr);

while(feof(infil_ptr) == 0)
{
	flag=0;
	for(i=1;i<args;i++)
	{
		p1 = buf;
		while(!isspace(p1[0]))
		{
			p1++;
		}
		p1[0]='\0';
		if(strcmp(buf,namearr[i]) == 0) 
		{
			cnt++;
			flag=1;
		}
	}
	
	if(flag == 0 && j == 2)
	{
		delfiles();
	}	

	fgets(buf,500,infil_ptr);
}

fclose(infil_ptr);
return(0);

}

delfiles()
{

sprintf(system_call,"cp %s /tmp",buf);

ret_val = system(system_call);
if(ret_val != 0)
{
	fprintf(stdout,"Copy to trashcan (/tmp) failed for some reason .. exiting\n");
	exit(1);
}

remove(buf);
return(0);
}
