#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LEN 2000
#define ARRROWS 10
#define ARRCOLS 50

FILE *pdf;
FILE *out;

int read1(void);

char *p1;
char *p2;
char *input1;
char *pch;

char buf[LEN];
char bufout[LEN];
long cnt1, cnt2, i, j, cntrec;
time_t tim;
/* 
1. GunPos
2. Bib
3. GunTime
4. Forename
5. Surname
6. M/F
7. Cat      (Optional)
8. Club     (Optional)
9. ChipPos
10. ChipTime
*/
char result[ARRROWS][ARRCOLS];

main(argc,argv)
int argc;
char *argv[];
{

	if(argc < 2)
	{
		fprintf(stdout,"Error, usage is pdftxt <pdf text file>\n");
		exit(1);
	}
   
	tim = time(NULL);
	fprintf(stdout,"\nProgram Begins  %s\n",ctime(&tim)); 
   
	input1 = argv[1];            
	
	if ((pdf = fopen(input1,"r")) == NULL)
	{
		fprintf(stdout,"Error opening %s does it exist?\n",input1);
		exit(1);
	}

	if ((out = fopen("pdfout.csv","w")) == NULL)
	{
		fprintf(stdout,"Error creating pdfout.txt\n");
		exit(1);
	}

	fprintf(out,"GunPos,Bib,GunTime,Forename,Surname,M/F,Cat,Club,ChipPos,ChipTime\n");

	cnt1=cnt2=cntrec=0;
	read1();
	cntrec = 1;
	while ( feof(pdf) == 0 )	
	{
/* GunPos Bib GunTime Forename Surname M/F Cat Club ChipPos ChipTime 
   Cat and club are optional fields sadly*/
		pch = strstr (p1,":");
		if(cntrec > 7 && pch != NULL)             /* Last field is chip time HH:MM:SS hence colon  */
		{
					strncat(result[cntrec-1],p1,strlen(p1)-1);
					
					if(cntrec == 8)     /*NO AGE CAT AND NO CLUB */
					{
						strncpy(result[9],result[7],strlen(result[7]));
						for(j=0;j<ARRCOLS;j++)
						{
							result[8][j] = '\0';
							result[8][j] = '\0';
						}
						strncpy(result[8],result[6],strlen(result[6]));
						for(j=0;j<ARRCOLS;j++)
						{
							result[7][j] = '\0';
							result[7][j] = '\0';
						}
						strncpy(result[7],"",1);
						for(j=0;j<ARRCOLS;j++)
						{
							result[6][j] = '\0';
							result[6][j] = '\0';
						}
						strncpy(result[6],"",1);
					}
					
					
					if(cntrec == 9)     /*EITHER NO AGE CAT OR CLUB */
					{
						strncpy(result[9],result[8],strlen(result[8]));
						for(j=0;j<ARRCOLS;j++)
						{
							result[8][j] = '\0';
						}
						strncpy(result[8],result[7],strlen(result[7]));
						for(j=0;j<ARRCOLS;j++)
						{
							result[7][j] = '\0';
						}
						
						if(strlen(result[6]) < 3 )   /* AGE CAT*/
						{
							strncpy(result[7],"",1);
						}
						else                         /* CLUB */
						{
							strncpy(result[7],result[6],strlen(result[6]));
							for(j=0;j<ARRCOLS;j++)
							{
								result[6][j] = '\0';
							}
							strncpy(result[6],"",1);
						}
					}
					
					for(i=0;i<ARRROWS;i++)
					{
						fprintf(out,"%s,",result[i]);
					}
					fprintf(out,"\n");

					for(i=0;i<ARRROWS;i++)
					{
						for(j=0;j<ARRCOLS;j++)
						{
							result[i][j] = '\0';
						}
					}
					cnt2++;
					cntrec = 0;
		}
		else
		{
				strncat(result[cntrec-1],p1,strlen(p1)-1);
		}

		read1();
		cntrec++;
	}

	tim = time(NULL);
	fprintf(stderr,"Program Ends OK  %s Read %ld  Wrote %ld\n",ctime(&tim), cnt1, cnt2); 
	return 0;
}

/***************************   read1  ******************************/

read1()
{
	for(i=0;i<LEN;i++)
	{
		buf[i] = '\0';
	}
	fgets(buf,LEN,pdf);
	if (feof(pdf) != 0)
	{
		fprintf(stdout,"End of file %s\n",input1);
		return 0;
	}
	cnt1++;
	p1 = buf;
	return 0;
}
