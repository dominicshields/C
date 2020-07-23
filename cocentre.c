#include <string.h>
#include <stdio.h>
#include <time.h>

#define LEN 1000

FILE *cocentre;
FILE *out;

char *p1;
char *p2;
char *input1;

char buf[LEN];
char bufout[LEN];
char group[6];
char desc[256];

int cnt1, cnt2, i, len, grflag;
time_t tim;

main(argc,argv)
int argc;
char *argv[];
{

	if(argc < 2)
	{
		fprintf(stdout,"Error, usage is cocentre <cocentre file>\n");
		exit(1);
	}
   
	tim = time(NULL);
	fprintf(stdout,"\nProgram Begins  %s\n",ctime(&tim));
   
	input1 = argv[1];            
	
	if ((cocentre = fopen(input1,"r")) == NULL)
	{
		fprintf(stdout,"Error opening %s does it exist?\n",input1);
		exit(1);
	}

	if ((out = fopen("d:\\temp\\costout.txt","w")) == NULL)
	{
		fprintf(stdout,"Error creating costout.txt\n");
		exit(1);
	}

	cnt1=cnt2=0;

	read1();
	
	while ( feof(cocentre) == 0 )	
	{
		if (strlen(buf) < 7)
		{
			strcpy(group,buf);
					}
		else
		{
			if(isalpha(buf[0]))
			{
				strcpy(desc,buf);
						}
			else
			{
				for(i=0;i<LEN;i++)
				{
					bufout[i] = '\0';
				}
				strncpy(bufout,group,strlen(group) -1);
				strcat(bufout,";");
				strncat(bufout,desc,strlen(desc) -1);
				strcat(bufout,";");
				strcat(bufout,buf);
				fprintf(out,"%s",bufout);			
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
	fgets(buf,LEN,cocentre);
	if (feof(cocentre) != 0)
	{
		fprintf(stdout,"End of file %s\n",input1);
		return 0;
	}
	cnt1++;
	p1 = buf;
	return 0;
}
