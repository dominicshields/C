/* hexdump.c:   dump a file to standard output in hex format */

#include <stdio.h>
#include <ctype.h>

FILE *f;

int i, count, len, value;
unsigned char buf[16];
long size;
float mult;

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
    
	while ((count = fread(buf,1,16,f)) > 0)
	{
		fprintf(stdout,"  %06X ",size+=count);

		/* ..Print Hex Bytes.. */
		for (i = 0; i < 16; ++i)
		{
			if (i < count)
			{
				fprintf(stdout," %02x",buf[i]); 
				if (iscntrl(buf[i]))
				{
					buf[i] = '.';
				}
			}
			else
			{
				fputs("   ",stdout);
				buf[i] = ' ';
			}
			if (i == 7)
			{
				putchar(' ');
			}
       
		}
        		/* ..Print Text Bytes.. */
        		fprintf(stdout," |%16.16s|\n",buf);
			len = strlen(buf);
			
			mult=1;
			for(i=1;i<=len;i++)
			{
				mult *= 16 ;
			}
			fprintf(stdout,"mult = %f\n",mult);
			for(i=0;i<len;i++)
			{
				switch (tolower(buf[i]))
				{
					case 'a' : value=10;break;
					case 'b' : value=11;break;
					case 'c' : value=12;break;
					case 'd' : value=13;break;
					case 'e' : value=14;break;
					case 'f' : value=15;break;
					default  : value=atoi(buf[i]);
				}
					


				mult *= 16;
			}
			
	}

        fclose(f);
        putchar('\f');
	return(0);
}
