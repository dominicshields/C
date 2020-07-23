#include <string.h>
#include <stdio.h>
#include <time.h>

#define LEN 2000

FILE *foldoc;
FILE *out;

char *p1;
char *p2;
char *input1;

char buf[LEN];
char bufout[LEN];

int cnt1, cnt2, i;
time_t tim;

main(argc,argv)
int argc;
char *argv[];
{

	if(argc < 2)
	{
		fprintf(stdout,"Error, usage is foldoc <foldoc file>\n");
		exit(1);
	}
   
	tim = time(NULL);
	fprintf(stdout,"\nProgram Begins  %s\n",ctime(&tim));
   
	input1 = argv[1];            
	
	if ((foldoc = fopen(input1,"r")) == NULL)
	{
		fprintf(stdout,"Error opening %s does it exist?\n",input1);
		exit(1);
	}

	if ((out = fopen("foldocout.txt","w")) == NULL)
	{
		fprintf(stdout,"Error creating foldocout.txt\n");
		exit(1);
	}

	cnt1=cnt2=0;

	read1();
	
	while ( feof(foldoc) == 0 )	
	{
		if(p1[0] == ' ')
		{
			fprintf(out,"%s",p1);
		}
		else
		{
			if(p1[0] == '\t')
			{
				fprintf(out,"%s",p1);
			}
			else
			{
				if(p1[0] == '\n')
				{
					fprintf(out,"%s",p1);
				}
				else
				{
					fprintf(out,"****\n",p1);
					for(i=0;i<LEN;i++)
					{
						bufout[i] = '\0';
					}
					strncpy(bufout,p1,strlen(buf)-1);
					fprintf(out,"%s***\n",bufout);
				}
			}
		}
		read1();
	}

	tim = time(NULL);
	fprintf(stderr,"Program Ends OK  %s\n",ctime(&tim));
	return 0;
}

/***************************   read1  ******************************/

read1()
{
	for(i=0;i<LEN;i++)
	{
		buf[i] = '\0';
	}
	fgets(buf,LEN,foldoc);
	if (feof(foldoc) != 0)
	{
		fprintf(stdout,"End of file %s\n",input1);
		return 0;
	}
	cnt1++;
	p1 = buf;
	return 0;
}
