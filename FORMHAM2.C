/* formatham2.c - The Next Day */
#include <stdio.h>

FILE *inp;
FILE *outp;

char *p1;
char *input;
char *output;

char buf[500];
char buf2[500];
char holdstuff[9][500];
char hold[16];
char year[5];

int marginflag,i,j,k,len,margin,cnt,cnt2;

main(argc,argv)
int argc;
char *argv[];
{
if(argc < 3)
{
	fprintf(stderr,"Error, usage is formatham2 <input file> <output file>\n");
	exit(1);
}

input = argv[1];
output = argv[2];

if ((inp = fopen(input,"r")) == NULL)
{
	fprintf(stderr,"Error opening %s does it exist?\n",input);
	exit(1);
}
if ((outp = fopen(output,"w")) == NULL)
{
	fprintf(stderr,"Error creating %s ?\n",output);
	exit(1);
}

read1();

while (feof(inp) == 0)
{
	if (marginflag == 0)
	{
	    	for(margin=0;margin<10;margin++,p1++)
     		{
			if(strncmp(p1,"SMALL",5) == 0)
			{
				marginflag++;
     				break;
			}
		}
	}

	p1 = buf;

	if(marginflag == 1)
	{
		p1 += margin;
	}

	if(strncmp(p1,"S",1) == 0 || strncmp(p1,"L",1) == 0 || strncmp(p1,"A",1) == 0)
	{
		for(j=0;j<15;j++)
		{
			hold[j]='\0';
		}

		if (strncmp(p1,"A",1) == 0)
		{
			strcpy(hold,"T");
		}
		else
		{
			strncpy(hold,p1,1);
		}

		for(j=0;j<15;j++,p1++)
		{
			if(strncmp(p1,"(",1) == 0)
			{
				break;
			}
		}

		if(j < 15)
		{
			p1++;
			strncat(hold,",",1);
			strncat(hold,p1,5);
		}
		else
		{
			p1 -= 15;
			len = strcspn(p1," ");
			p1 += len + 1;
			len = strcspn(p1," ");
			p1 += len + 1;
			strncat(hold,",",1);
			strncat(hold,p1,2);
		}
	}


	p1 = buf;
	if(p1[margin] == '1')
	{
		p1 += margin;
		strncpy(year,p1,4);
		year[4]='\0';
		strcpy(buf2,hold);
		strcat(buf2,",");
		strcat(buf2,buf);
		p1 = buf2;
		p1 += (strlen(hold)+margin+5);
		i=0;
		while (i < 5)
		{
			*p1 = ',';
			p1 += 10;
			i++;
		}

		switch (atoi(year))
		{
		case 1988 : for(i=0;i<8;i++)
			    {
				fprintf(outp,"%s",holdstuff[i]);
			    };
			    strcpy(holdstuff[0],buf2);
			    len=strlen(holdstuff[0]);
			    len--;
			    for(k=len;k<500;k++)
			    {
				holdstuff[0][k]= '\0';
			    }
			    strcat(holdstuff[0],",,,,,\n");
			    cnt=1;
			    break;
		default :
		if(cnt > 7)
		{
			len=strlen(holdstuff[cnt-7]);
			len--;
			for(k=len;k<500;k++)
			{
				holdstuff[cnt-7][k]= '\0';
			}
			p1=buf2;
			while(strncmp(p1,"198",3) != 0 && strncmp(p1,"199",3) != 0)
			{
				p1++;
			}
			p1 +=4;
			strcat(holdstuff[cnt-7],p1);
		}
		else
		{
			strcpy(holdstuff[cnt],buf2);
		}
			    cnt++;
			    break;
		}
	}

	read1();
}

fclose(inp);
fclose(outp);


return 0;

}

 /***************************   read1  ******************************/

read1()
{
fgets(buf,500,inp);
if (feof(inp) != 0)
{
	fprintf(stderr,"End of file %s\n",input);
	return 0;
}
p1 = buf;
return 0;
}
