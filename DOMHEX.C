/* domhex.c:   dump a file to standard output in hex format */

#include <stdio.h>
#include <ctype.h>

FILE *f;

char *p1;
int i, count, len, value;
char buf[100];
char hold[2];
long size;
float mult, tot, inter;

main(argc,argv)
int argc;
char *argv[];
{
	if(argc < 2)
	{
		fprintf(stderr,"Usage is %s <filename>\n",argv[0]);
		exit(1);
	}
    
        if ((f = fopen(argv[1],"rb")) == NULL)
	{
		fprintf(stderr,"Error opening %s\n",argv[1]);
		exit(1);
	}

    
	fprintf(stdout,"Dump of %s\n\n",argv[1]);
	size = 0;
    
	while (feof(f) == 0)
	{
		fgets(buf,100,f);
		p1=buf;
       		fprintf(stdout,"input = %s\n",buf);
		len = strlen(buf);
		len-=2;
		mult=1;
		for(i=1;i<len;i++)
		{
			mult *= 16 ;
		}
		tot=0;

		for(i=0;i<len;i++)
		{
			strncpy(hold,p1,1);
			hold[1]='\0';
     		fprintf(stdout,"hold = %s\n",hold);
			if(*hold == 'a' || *hold == 'A')
			{
				value=10;
			}
			if(*hold == 'b' || *hold == 'B')
			{
				value=11;
			}
			if(*hold == 'c' || *hold == 'C')
			{
				value=12;
			}
			if(*hold == 'd' || *hold == 'D')
			{
				value=13;
			}
			if(*hold == 'e' || *hold == 'E')
			{
				value=14;
			}
			if(*hold == 'f' || *hold == 'F')
			{
				value=15;
			}
			if(!isalpha(*hold))
			{
				value=atoi(hold);
			}
				inter = (float) (value*mult);
				tot += inter;
     		fprintf(stdout,"mult = %f\n",mult);
     		fprintf(stdout,"value = %d\n",value);
     		fprintf(stdout,"inter = %f\n",inter);
     		fprintf(stdout,"tot = %f\n",tot);
				mult /= 16;
			p1++;
		}
     		fprintf(stdout,"Decimal Value = %f\n",tot);
			
		fgets(buf,100,f);
	}

        fclose(f);
	return(0);
}
