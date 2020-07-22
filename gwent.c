/**************************************************************************
PROGRAM NAME : gwent
Layout of file
Blank Line
Gwent Cross Country League 2000-2001 : 14 October 2000 : Dare Valley Country Park, Aberdare

 Gwent Cross Country League 2000-2001

14 October 2000 : Dare Valley Country Park, Aberdare

 Senior Men  (306 Starters)

   Pos Athlete                   Club                                       Time

    1  A. Doel                   Neath Harriers                            30.00
**************************************************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dos.h>
#include <dir.h>
#include <time.h>

#define MAXLINE 512		/* check maximum length of input records */

FILE *inp;			/* printing input file */
FILE *outp;			/* printing output file */
FILE *infil_ptr;		/* read DIR file */
FILE *gl_ptr;
FILE *outfil_ptr;


char *input;
char *output;

char *filename;
char *p1;
char *p2;
char *p3;
char *p4;
char *s;
char *retval;
char buf[MAXLINE];
char bufin[MAXLINE];
char bufout[MAXLINE];
char filenamebuf[81];
char arr[MAXLINE][80];
char syscall[MAXLINE];
char syscomm[MAXLINE];
char okprcall[MAXLINE];
char find[MAXLINE];
char drive[3];
char dir[MAXLINE];
char file[15];
char path[80];
char drive[3];
char ext[4];
char *outfilename;
char season[10];
char fixturedate[50];
char venue[80];
char starters[8];
char pos[4];
char athlete[80];
char club[80];
char finishtime[6];
char points[20];
char agecat[3];

time_t tim;

int flags, len, j, linelen, x, z, cnt, linecnt, i, filecnt;

int dirlook(void);
int openoutput(void);
int processdir(void);
int system(char*);
int opengl(void);
int processgl(void);
int readgl(void);
int tidyup(void);
int clearbuf(void);
int exit(int);

main(argc,argv)
int argc;
char *argv[];
{
	filename=argv[argc-1];
	cnt = 0;

	dirlook();
	openoutput();
	processdir();
	tidyup();

	tim = time(NULL);
	fprintf(stdout,"%d files read\n%d records written %s\n",filecnt,cnt,ctime(&tim));
	return(0);
}


dirlook()
{
	s=filename;
	flags=fnsplit(s,drive,dir,file,ext);
	sprintf(path,"%s%s",drive,dir);
	printf("Drive %s\n",drive);
	printf("Dir %s\n",dir);
	sprintf(syscall,"DIR *.txt /A:-D /B /L > D:\\@@@@@@@@.@@@");
	system(syscall);
	if ((infil_ptr = fopen("D:\\@@@@@@@@.@@@","r")) == NULL)
	{
		fprintf(stdout,"Error opening D:\\@@@@@@@@.@@@\n");
		exit(1);
	}
	strcpy(syscall,"");
	return(0);
}

processdir()
{
	fgets(filenamebuf,81,infil_ptr);
	while(feof(infil_ptr) == 0)
	{
		z=0;
		p3 = filenamebuf;
		p1 = filenamebuf;
		while(p3[0] != '.' && z<60)
		{
			p3++;
			z++;
		}
		p3++;
		if(strncmp(p3,"exe",3) != 0)
		{
			opengl();
			processgl();
			fclose(gl_ptr);
		}
		for(j=0;j<80;j++)
		{
			filenamebuf[j]='\0';
		}
		fgets(filenamebuf,81,infil_ptr);
	}
	return(0);
}

processgl()
{
	readgl();
	while (feof(gl_ptr) == 0)
	{
		p2 = buf;
		p4 = p2;

		if(strlen(buf) > 15 && linecnt > 3 && linecnt != 6 && linecnt != 8 && linecnt != 9 )
		{

			if(linecnt == 4)
			{
				i = strcspn(p2,"2");
				p2 += i;
				strncpy(season,p2,9);
				strcat(season,"");
			}
			if(linecnt == 5)
			{
			  for(i = 0;i < 20;i++)
				{
					fixturedate[i] = '\0';
				}
				i = strcspn(p2,":");
			  strncpy(fixturedate,p2,i-1);
			  strcat(fixturedate,"");
																		printf("fixturedate = %s\n",fixturedate);
			  p2 += i;
			  p2 += 2;
			  strcpy(venue,"");
			  strcpy(venue,p2);
			  strcat(venue,"");
			  venue[strlen(venue) -1] = '\0';
			  if(strcmp(venue,"Bath University ") == 0)
			  {
			  	strcat(venue,",Bath");
			  }
			  if(strcmp(venue,"University of Bath ") == 0)
			  {
			  	strcat(venue,",Bath");
			  }
			  if(strcmp(venue,"Pembrey Country Park ") == 0)
			  {
			  	strcat(venue,",Llanelli");
			  }
			  if(strcmp(venue,"Parc Bryn Bach ") == 0)
			  {
			  	strcat(venue,",Tredegar");
			  }
			  if(strcmp(venue,"Brecon Leisure Centre ") == 0)
			  {
			  	strcat(venue,",Penlan");
			  }
			  if(strcmp(venue,"Monmouth Showground ") == 0)
			  {
			  	strcat(venue,",Monmouth");
			  }		
			  if(strcmp(venue,"Pontypool Park ") == 0)
			  {
			  	strcat(venue,",Pontypool");
			  }		  
			  
			}
/*			if(linecnt == 7)
			{
				i = strcspn(p2,"(");
				p2 += i;
				p2++;
				strcpy(starters,"");
				i = strcspn(p2,"S");
				strncpy(starters,p2,i-1);
			  strcat(starters,"");
			}   */
			if(linecnt > 10 && strlen(buf) > 50)
			{
				p2 += 2;
				strncpy(pos,p2,3);
				strcat(pos,"");
				p2 += 5;
				strncpy(athlete,p2,21);
 				strcat(athlete,"");
				i = strcspn(p2,"(");
				if(i < 25)
				{
					p2 += i;
					p2++;
					strncpy(agecat,p2,2);  /* stupid ind issue to think about */
					strcat(agecat,"");
				}
				else
				{
					strcpy(agecat,"SM");
				}
				p2 = p4;   /* reset pointer to beginning of line*/
				p2 += 33;   /* Club always starts at byte 34*/
				strncpy(club,p2,40);
 				strcat(club,"");
				p2 = p4;   /* reset pointer to beginning of line*/
				p2 += 75;   /* Time always starts at byte 76*/
				strncpy(finishtime,p2,5);
				strcat(finishtime,"");
				finishtime[2] = ':';
				p2 += 5;
				strcpy(points,p2);

				/* Build the output string and write only when past the first 10 lines*/
				clearbuf();
				strcpy(bufout,season);
				strcat(bufout,",");
				strcat(bufout,fixturedate);
				strcat(bufout,",");
				strcat(bufout,venue);
				strcat(bufout,",");
/*				strcat(bufout,starters);
				strcat(bufout,","); */
				strcat(bufout,pos);
				strcat(bufout,",");
				strcat(bufout,athlete);
				strcat(bufout,",");
				strcat(bufout,agecat);
				strcat(bufout,",");
				strcat(bufout,club);
				strcat(bufout,",0:");
				strcat(bufout,finishtime);
				strcat(bufout,",");
				strcat(bufout,points);

				fprintf(outfil_ptr,"%s",bufout);
				cnt++;
			}
		}

		readgl();
	}

	return(0);
}

opengl()
{
	x = strlen(filenamebuf);
	filenamebuf[x-1] = '\0';
	if ((gl_ptr = fopen(filenamebuf,"r+")) == NULL)
	{
		fprintf(stdout,"Error opening %s\n",filenamebuf);
		exit(1);
	}
	filecnt++;
	linecnt = 0;
	return(0);
}

openoutput()
{
	outfilename = "GwentLeagueRes.csv";
	if ((outfil_ptr = fopen(outfilename,"w")) == NULL)
	{
		fprintf(stdout,"Error creating %s\n",outfilename);
		exit(1);
	}
	fprintf(outfil_ptr,"Season,Date,Venue,Town,Position,Athlete,Category,Club,Time,Points\n");

	return(0);
}

readgl()
{
	fgets(buf,MAXLINE,gl_ptr);
	linecnt++;
	return(0);
}

tidyup()
{
	fclose(infil_ptr);
	fclose(outfil_ptr);
	return(0);
}

clearbuf()
{
	for(i = 0;i < MAXLINE;i++)
	{
		bufout[i] = '\0';
	}
	return(0);
}