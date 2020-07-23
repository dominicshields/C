#include <string.h>
#include <stdio.h>
#include <time.h>

#define LEN 1000

FILE *babel;
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
		fprintf(stdout,"Error, usage is babel <babel file>\n");
		exit(1);
	}
   
	tim = time(NULL);
	fprintf(stdout,"\nProgram Begins  %s\n",ctime(&tim));
   
	input1 = argv[1];            
	
	if ((babel = fopen(input1,"r")) == NULL)
	{
		fprintf(stdout,"Error opening %s does it exist?\n",input1);
		exit(1);
	}

	if ((out = fopen("babelout.txt","w")) == NULL)
	{
		fprintf(stdout,"Error creating babelout.txt\n");
		exit(1);
	}

	cnt1=cnt2=0;

	read1();
	
	while ( feof(babel) == 0 )	
	{
		if(p1[0] == ' ')
		{
			p1 += 9;
			strncat(bufout,p1,strlen(p1)-1);
		}
		else
		{
			fprintf(out,"%s\n",bufout);
			for(i=0;i<LEN;i++)
			{
				bufout[i] = '\0';
			}
			strncpy(bufout,buf,strlen(buf)-1);
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
	fgets(buf,LEN,babel);
	if (feof(babel) != 0)
	{
		fprintf(stdout,"End of file %s\n",input1);
		return 0;
	}
	cnt1++;
	p1 = buf;
	return 0;
}
