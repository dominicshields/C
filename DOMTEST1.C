#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <dos.h>
#include <dir.h>
#include <time.h>

#define MAXLINE 256

FILE *inp;			/* printing input file */
FILE *outp;		/* printing output file */
FILE *infil_ptr;		/* read DIR file */
FILE *gainloss_ptr;		/* read gainloss file */
FILE *outfil_ptr;

char *input;
char *output;

char *filename;
char *p1;
char *p2;
char *p3;
char *p4;
char *s;
char buf[MAXLINE];
char bufin[MAXLINE];
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
char period[7];
char inqcode[4];
char *outfilename;

int flags, len, j, linelen, x, z;

main(argc,argv)
int argc;
char *argv[];
{
	filename=argv[argc-1];

	dirlook();

	return(0);
}

dirlook()
{
	s=filename;
	flags=fnsplit(s,drive,dir,file,ext);
	sprintf(path,"%s%s",drive,dir);
	printf("Drive %s\n",drive);
	printf("Dir %s\n",dir);
	sprintf(syscall,"DIR *.* /A:-D /B /L > c:\\@@@@@@@@.@@@");
	system(syscall);
	if ((infil_ptr = fopen("c:\\@@@@@@@@.@@@","r")) == NULL)
	{
		fprintf(stdout,"Error opening c:\\@@@@@@@@.@@@\n");
		exit(1);
	}


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
		if((strncmp(p3,"exe",3) != 0) && (strncmp(p3,"out",3) != 0))
		{
			/* LAYOUT IS gainloss_001_200403 */
			len = strlen(filenamebuf);
			p1 += (len - 11);
			strncpy(inqcode,p1,3);
			inqcode[3] = '\0';
			p1 += 4;
			strncpy(period,p1,6);
			period[6] = '\0';
			opengl();
			processgl();
		}
		for(j=0;j<80;j++)
		{
			filenamebuf[j]='\0';
		}
		fgets(filenamebuf,81,infil_ptr);
	}


	fclose(infil_ptr);
	remove("c:\\@@@@@@@@.@@@");
	return(0);
}

processgl()
{
	readgl();
	while(feof(gainloss_ptr) == 0)
	{
		p2 = buf;
		p4 = p2;
		linelen = strlen(p2);
		linelen--;
		p2 = p2 + linelen;
		p2[0] = '\0';
		strncat(p2,",",1);
		strcat(p2,inqcode);
		p2 += 3;
		strncat(p2,",",1);
		p2++;
		strcat(p2,period);
		p2 += 6;
	/*	fprintf(stdout,"%s\n",p3);*/
		fprintf(outfil_ptr,"%s\n",p4);
		readgl();
	}
	return(0);
}

opengl()
{
	x = strlen(filenamebuf);
	filenamebuf[x-1] = '\0';
	if ((gainloss_ptr = fopen(filenamebuf,"r+")) == NULL)
	{
		fprintf(stdout,"Error opening %s\n",filenamebuf);
		exit(1);
	}

	outfilename = filenamebuf;
	strcat(outfilename,".out");
	if ((outfil_ptr = fopen(outfilename,"w")) == NULL)
	{
		fprintf(stdout,"Error creating %s\n",outfilename);
		exit(1);
	}
	return(0);
}
readgl()
{
	fgets(buf,MAXLINE,gainloss_ptr);
	return(0);
}