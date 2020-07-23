/* sipdata.c
Dominic Shields 20/10/2000

takes a file in this format
Abbott Owen,107,760034,C1 NAT,PERMANENT,T,37,MA10,100
Abrahams Carole,189,5974,C4 LON,PERMANENT,L,29,JH15,100
Abram Geraldine,108,740246,A1 NAT,FIXED TERM,S,37,PA36,80.77
Abram Geraldine,108,740246,A1 NAT,FIXED TERM,S,37,PA49,19.23
Abram Sue E,190,307418,B1 NAT,PERMANENT,N,37,CE10,100
Abram Sue,108,32076,A2 NAT,PERMANENT,S,15,PA50,80.77
Abram Sue,108,32076,A2 NAT,PERMANENT,S,15,PA99,19.23
Abrams Darren P,107,45901,B1 NAT,PERMANENT,T,37,MS50,100

and outputs a file with the person occurring once in the form

740246,Abram Geraldine,PA36;PA49,80.77;19.23

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
char percent[LEN];

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

	if ((out = fopen("sipout.txt","w")) == NULL)
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
	
	fprintf(out,"%06s,%s,%s\n",staffno,costcentre,percent);

	tim = time(NULL);
	fprintf(stderr,"Program Ends OK  %s\n",ctime(&tim));
	return 0;
}

/***************************   samerec  ******************************
Abrams Darren P,107,45901,B1 NAT,PERMANENT,T,37,MS50,100	 */

samerec()
{
	commacnt=0;
	len=0;
	p1 = buf;
	
	while(commacnt < 7 )  /* pos to comma before cost centre */
	{
		p1++;
		if(p1[0] == ',')
		{
			commacnt++;
		}
	}
	p1++;			/* pos past comma before cost centre */
	p2 = p1;
	
	while(commacnt < 8 )	/* find next comma */
	{
		p1++;
		len++;
		if(p1[0] == ',')
		{
			commacnt++;
		}
	}
	
	strcat(costcentre,";");
	strncat(costcentre,p2,len);
		
	p1++;
	len = 0;
	p2=p1;
	
	while(p1[0] != '\n')
	{
		p1++;
		len++;
	}
	
	strcat(percent,";");
	
	if(len == 0)
	{
		strcat(percent,"100");
	}
	else
	{
		strncat(percent,p2,len);
	}
		
	return 0;

}


/***************************   newrec  ******************************/

newrec()
{
	if (flag > 0)
	{
		fprintf(out,"%06s,%s,%s\n",staffno,costcentre,percent);
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
	
	commacnt=0;
	len=0;
	p1 = buf;
	
	while(commacnt < 7 )  /* pos to comma before cost centre */
	{
		p1++;
		if(p1[0] == ',')
		{
			commacnt++;
		}
	}
	p1++;			/* pos past comma before cost centre */
	p2 = p1;
	
	while(commacnt < 8 )	/* find next comma */
	{
		p1++;
		len++;
		if(p1[0] == ',')
		{
			commacnt++;
		}
	}
	
	strncpy(costcentre,p2,len);
	costcentre[4] = '\0';
	
	p1++;
	len = 0;
	p2=p1;
	
	while(p1[0] != '\n')
	{
		p1++;
		len++;
	}
	
	if(len == 0)
	{
		strcpy(percent,"100");
	}
	else
	{
		strncpy(percent,p2,len);
	}
		
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
	percent[i]='\0';
}

return 0;
}