#include <string.h>
#include <stdio.h>
#include <time.h>

#define LEN 1000

FILE *orgunit;
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
		fprintf(stdout,"Error, usage is orgunit <orgunit file>\n");
		exit(1);
	}
   
	tim = time(NULL);
	fprintf(stdout,"\nProgram Begins  %s\n",ctime(&tim));
   
	input1 = argv[1];            
	
	if ((orgunit = fopen(input1,"r")) == NULL)
	{
		fprintf(stdout,"Error opening %s does it exist?\n",input1);
		exit(1);
	}

	if ((out = fopen("orgout.txt","w")) == NULL)
	{
		fprintf(stdout,"Error creating orgout.txt\n");
		exit(1);
	}

	cnt1=cnt2=0;

	read1();
	
	while ( feof(orgunit) == 0 )	
	{
		cnt2=0;
		for (i=0;i<strlen(buf);i++)
		{
			if(p1[0] == ',')
			{
				cnt2++;
			}
			p1++;
		}

		strncpy(bufout,buf,strlen(buf)-1);
		switch (cnt2)
		{
			case 0 : strcat(bufout,", , , ");break;
			case 1 : strcat(bufout,", , ");break;
			case 2 : strcat(bufout,", ");break;
		}

		fprintf(out,"%s\n",bufout);
		for(i=0;i<LEN;i++)
		{
			bufout[i] = '\0';
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
	fgets(buf,LEN,orgunit);
	if (feof(orgunit) != 0)
	{
		fprintf(stdout,"End of file %s\n",input1);
		return 0;
	}
	cnt1++;
	p1 = buf;
	return 0;
}
