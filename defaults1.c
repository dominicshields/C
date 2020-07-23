/* defaults.c
Dominic Shields 20/10/2000

takes a file in this format

Abbott Owen,107,760034,C1 NAT,PERMANENT,T,MA10,AA019, ,1
Abbott Owen,107,760034,C1 NAT,PERMANENT,T,MA10,AA029, ,28.6
Abbott Owen,107,760034,C1 NAT,PERMANENT,T,MA10,AA900, ,7.4
Abrahams Carole,189,5974,C4 LON,PERMANENT,L,JH15,MA001,,29
Abram Geraldine,108,740246,A1 NAT,FIXED TERM,S,PA36,LA002, ,29.9

and outputs a file with the person occurring once in the form

760034,Abbott Owen,"MA10":"MA10":"MA10","AA019":"AA029":"AA900",1:28.6:7.4

in other words, the multi-value fields are in a form that can be inported to Notes
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define LEN 100

FILE *in;
FILE *out;

char *p1;
char *p2;
char *input1;

char buf[LEN];
char bufout[LEN];

char staffno[7];
char staffnoz[7];
char staffnohold[7];
char name[LEN];
char costcentre[LEN];
char outputcode[LEN];
char filler[LEN];
char hours[LEN];
char mins[LEN];
char hourschar[LEN];
char hourshold[4];
char minschar[4];
char minshold[4];
double minsdouble;
int minsint;


int cnt1, cnt2, i, commacnt, len, ccctr, flag;
time_t tim;

main(argc,argv)
int argc;
char *argv[];
{
	if(argc < 2)
	{
		fprintf(stdout,"Error, usage is in <input file>\n");
		exit(1);
	}
   
	tim = time(NULL);
	fprintf(stdout,"\nProgram Begins  %s\n",ctime(&tim));
   
	input1 = argv[1];            
	
	if ((in = fopen(input1,"r")) == NULL)
	{
		fprintf(stdout,"Error opening %s does it exist?\n",input1);
		exit(1);
	}

	if ((out = fopen("defout.txt","w")) == NULL)
	{
		fprintf(stdout,"Error creating defout.txt\n");
		exit(1);
	}

	cnt1=cnt2=commacnt=flag=0;
	
	init();

	read1();
	
	while ( feof(in) == 0 )	
	{
		commacnt=0;
		len=0;
		while(commacnt < 2 )
		{
			p1++;
			if(p1[0] == ',')
			{
				commacnt++;
			}
		}
		p1++;
		p2 = p1;
		while(p1[0] != ',')	
		{				
			p1++;
			len++;
		}
	
		if(strncmp(p2,staffno,len) == 0)
		{
			samerec();
		}
		else
		{
			newrec();
		}
		
		read1();
	}

	tim = time(NULL);
	fprintf(stderr,"Program Ends OK  %s\n",ctime(&tim));
	return 0;
}

/***************************   samerec  ******************************
Abbott Owen,107,760034,C1 NAT,PERMANENT,T,MA10,AA019, ,1
Abbott Owen,107,760034,C1 NAT,PERMANENT,T,MA10,AA029, ,28.6
Abbott Owen,107,760034,C1 NAT,PERMANENT,T,MA10,AA900, ,7.4
Abrahams Carole,189,5974,C4 LON,PERMANENT,L,JH15,MA001,,29
Abram Geraldine,108,740246,A1 NAT,FIXED TERM,S,PA36,LA002, ,29.9 */

samerec()
{
	commacnt=0;
	len=0;
	p1 = buf;

	while(commacnt < 6 )
	{
		p1++;
		len++;
		if(p1[0] == ',')
		{
			commacnt++;
		}
	}
	p1++;
	len=0;
	p2 = p1;
	while(commacnt < 7 )
	{
		p1++;
		len++;
		if(p1[0] == ',')
		{
			commacnt++;
		}
	}
	
	strcat(costcentre,":\"");
	strncat(costcentre,p2,len);
	strcat(costcentre,"\"");
	
	p1++;
	p2 = p1;
	len=0;
	while(commacnt < 8 )
	{
		p1++;
		len++;
		if(p1[0] == ',')
		{
			commacnt++;
		}
	}
	
	strcat(outputcode,":\"");
	strncat(outputcode,p2,5);
	strcat(outputcode,"\"");
	
	p1++;
	p2 = p1;
	len=0;
	while(commacnt < 9 )
	{
		p1++;
		len++;
		if(p1[0] == ',')
		{
			commacnt++;
		}
	}
	p1++;

	len = 0;
	p2=p1;
	while(len < 4 && p1[0] != '.' && p1[0] != '\n')
	{
		p1++;
		len++;
	}
	if(len == 0)
	{
		strcat(hourschar,":");
		strncat(hourschar,p2,strlen(p2));
		strcat(minschar,":");
		strcat(minschar,"0");
	}
	else
	{
		strcat(hourschar,":");
		strncpy(hourshold,p2,len);
		hourshold[len]='\0';
		strcat(hourschar,hourshold);
		p2 += len;
		strncpy(minshold,p2,4);
		minshold[4]='\0';
		strcat(minschar,":");
		minsdouble = atof(minshold); 
		minsdouble *= 60.0;
		minsint = (int) minsdouble;
		sprintf(minshold,"%d",minsint);
		strcat(minschar,minshold);
	}
	
	return 0;

}


/***************************   newrec  ******************************/

newrec()
{
	if (flag > 0)
	{
/*		fprintf(out,"%06s,%s,%s,%s,%s,%s\n",staffno,name,costcentre,outputcode,hourschar,minschar); */
		fprintf(out,"%06s,%s,%s,%s,%s\n",staffno,costcentre,outputcode,hourschar,minschar);
	}
	init();
	flag = 1;
	strncpy(staffno,p2,len);
	staffno[len] = '\0'; 
	commacnt=0;
	len=0;
	p1 = buf;

	while(commacnt < 1 )
	{
		p1++;
		len++;
		if(p1[0] == ',')
		{
			commacnt++;
		}
	}
	strncpy(name,buf,len);
	name[len] = '\0';
	p1++;
	len=0;
	while(commacnt < 6 )
	{
		p1++;
		len++;
		if(p1[0] == ',')
		{
			commacnt++;
		}
	}
	p1++;
	len=0;
	p2 = p1;
	while(commacnt < 7 )
	{
		p1++;
		len++;
		if(p1[0] == ',')
		{
			commacnt++;
		}
	}
	
	ccctr=0;
	
	if(p2[0] == ' ')
	{	
		flag = 0;
		goto nextrec;
	}
	
	strcpy(costcentre,"\"");
	strncat(costcentre,p2,len);
	strcat(costcentre,"\"");
	costcentre[7] = '\0';
	ccctr=1;
	
	p1++;
	p2 = p1;
	len=0;
	while(commacnt < 8 )
	{
		p1++;
		len++;
		if(p1[0] == ',')
		{
			commacnt++;
		}
	}
			
	strcpy(outputcode,"\"");
	strncat(outputcode,p2,5);
	strcat(outputcode,"\"");
	outputcode[7] = '\0';

	p1++;
	p2 = p1;
	len=0;
	while(commacnt < 9 )
	{
		p1++;
		len++;
		if(p1[0] == ',')
		{
			commacnt++;
		}
	}
	p1++;
	len = 0;
	p2=p1;
	while(len < 4 && p1[0] != '.' && p1[0] != '\n')
	{
		p1++;
		len++;
	}
	if(len == 0)
	{
		strncat(hourschar,p2,strlen(p1));
		strcpy(minschar,"0");
	}
	else
	{
		strncpy(hourschar,p2,len);
		p2 += len;
		strncpy(minshold,p2,4);
		minshold[4]='\0';
		minsdouble = atof(minshold); 
		minsdouble *= 60.0;
		minsint = (int) minsdouble;
		sprintf(minshold,"%d",minsint);
		strcpy(minschar,minshold);
	}
nextrec:
	return 0;
}

/***************************   read1  ******************************/

read1()
{
	for(i=0;i<LEN;i++)
	{
		buf[i] = '\0';
	}
	fgets(buf,LEN,in);
	if (feof(in) != 0)
	{
		fprintf(stdout,"End of file %s\n",input1);
		return 0;
	}
	cnt1++;
	p1 = buf;
	return 0;
}

init()
{
for(i=0;i<LEN;i++)
{
	name[i]='\0';
	costcentre[i]='\0';
	outputcode[i]='\0';
	hours[i]='\0';
	hourschar[i]='\0';
	mins[i]='\0';
}
for(i=0;i<4;i++)
{
	hourshold[i]='\0';
	minschar[i]='\0';
	minshold[i]='\0';
}
return 0;
}