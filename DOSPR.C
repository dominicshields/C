/***************************************************************
pr.c
usage:  pr [-l] [-n#] [-t#] [-h] [-w] [file] 
options: -l	print without line numbers
	 -n#	print # lines per page (4-line header extra)
 	 -t#	set tab to # spaces (default 4)
	 -h	do not print a page header
	 -w#    pagewidth (default 256)	
***************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define MAXLINE 256
#define yes 1
#define no 0

FILE *infil_ptr;

int number = yes;
int lines_per_page = 55;
int tabspace = 4;
int headers = yes;
int tabstops[MAXLINE];
int i,len,firsttime,col,retval,across,width;
int pageno = 1;
int lineno = 1;
int tot_lineno = 1;
int real_lineno = 1;
 
char line[MAXLINE];
char datestr[40];
char holdtext[80];
char holdtext2[80];
char syscall[80];
char charno[5];
char *p1;
char *filename;

time_t tim;

main(argc,argv)
int argc;
char *argv[];
{
	if(argc < 2)
	{
		fprintf(stdout,"Error : usage is pr -lnth (opt) <filename). pr help for help\n");
		exit(1);
	}

	for (i=1;i<argc;i++)
	{
		if(strncmp(argv[i],"-l",2) == 0)
		{
			number = no;
		}
		if(strncmp(argv[i],"-n",2) == 0)
		{
			p1 = argv[i];
			p1 += 2;
			len = strcspn(p1," ");
			strncpy(charno,p1,len);
			charno[len]='\0';
			lines_per_page = atoi(charno);
		}
		if(strncmp(argv[i],"-t",2) == 0)
		{
			p1 = argv[i];
			p1 += 2;
			len = strcspn(p1," ");
			strncpy(charno,p1,len);
			charno[len]='\0';
			tabspace = atoi(charno);
		}
		if(strncmp(argv[i],"-w",2) == 0)
		{
			p1 = argv[i];
			p1 += 2;
			len = strcspn(p1," ");
			strncpy(charno,p1,len);
			charno[len]='\0';
			width = atoi(charno);
		}
		if(strncmp(argv[i],"-h",2) == 0)
		{
			headers = no;
		}
		if(strncmp(argv[i],"help",4) == 0)
		{
			strcpy(syscall,"echo @prompt $e[34;47m > domtemp.bat");
			system(syscall);
			strcpy(syscall,"domtemp");
			system(syscall);
			fprintf(stdout,"This program formats output in a similar way to the unix pr command.\n");
			fprintf(stdout,"usage:  pr [-l] [-n#] [-t#] [-h] [file]\n");
			fprintf(stdout,"options: -l  print without line numbers\n");
			fprintf(stdout,"	 -n# print # lines per page (default 55)\n");
			fprintf(stdout,"	 -t# set tab to # spaces (default 4)\n");
			fprintf(stdout,"	 -h do not print a page header\n");
			fprintf(stdout,"Rugware - Dominic - Version 1.1 19/10/95\n");
			fprintf(stdout,"May be used freely by anyone who's got a relative with a dodgy hairpiece\n");
			strcpy(syscall,"echo @prompt $e[37;40m > domtemp.bat");
			system(syscall);
			strcpy(syscall,"domtemp");
			system(syscall);
			remove("domtemp.bat");
			exit(1);
		}
	}

	filename=argv[argc-1];

	settabs();

	tim = time(NULL);
	sprintf(datestr,"%s",ctime(&tim));

	if((infil_ptr = fopen(filename,"r")) == NULL)
	{
		fprintf(stdout,"Error opening %s\n",filename);
		exit(1);
	}
	pr();
	fclose(infil_ptr);

	return(0);
}

pr()
{
	if(width == 0)
	{
		width = MAXLINE;
	}

	if (headers)
	{
		header(filename,pageno);
	}
	else
	{
		printf("\n\n\n\n\n");
	}

	while (fgets(line,MAXLINE-1,infil_ptr) != NULL)
	{
		if (lineno == 0)
		{
			if (headers)
			{
				header(filename,++pageno);
			}
			else
			{
				printf("\n\n\n\n\n");
			}
			lineno = 1;
			real_lineno = 1;
		}

		if (number)
		{
			printf("%8d  ",tot_lineno);
		}
		else
		{
			printf("       ");
		}
		lineno++;
		real_lineno++;
		tot_lineno++;

		detab(line);

		if (real_lineno > lines_per_page)
		{
			putchar('\f');
			lineno = 0;
			real_lineno = 0;
		}
	}
	/* form-feed after partial page */
	if (lineno > 0) putchar('\f');
	return(0);
}

header(file,page)
char *file;
int page;
{
	printf("\n    "); 
	for (i=0;i<75;++i)
	{
		putchar('-');
	}
	printf("\n    ");

	if(firsttime == 0)
	{
		strcpy(holdtext,file);
		strcat(holdtext,"           ");
		len = strlen(datestr);
		datestr[len-1]='\0';
		strcat(holdtext,datestr);
		strcat(holdtext,"                 Page ");
		firsttime++;
	}
	sprintf(holdtext2,"%s%4d\n",holdtext,page);
	printf("%s",holdtext2);
	printf("    ");
	for (i=0;i<75;++i)
	{
		putchar('-');
	}
	printf("\n\n");

	return(0);
}

detab(line)
char *line;
{
	col = 1;
	across = 8;

	for (i=0;i<strlen(line);++i)
	{
		if (line[i] == '\t')
		{
			do
			{
				putchar(' ');
				++col;
				across++;
			} while (!tabstops[col]);
		}
		else
		{
			putchar(line[i]);
			++col;
			across++;
		}
		if(across > width)
		{
			putchar('\n');
			real_lineno++;
			across = 0;
		}
	}

	return(0);
}

settabs()
{
	for (i=0;i<MAXLINE;++i)
	{
		tabstops[i] = (i % tabspace == 1);
	}

	return(0);
}
