/**************************************************************************
PROGRAM NAME : reclen (source reclen.c)
PURPOSE      :
PARAMETERS   : Filename
OUTPUT       :
COMMENTS     :
AUTHOR       : Dominic Shields Mar 2004
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>

#define MAXLINE 8192

FILE *infil_ptr;
char *filename;
char buf[MAXLINE];
struct stat stat_p;
time_t tim;
long reccnt, longreclen, shortreclen, longrecnumber, shortrecnumber;
double totalfilesize, avereclen;

char * format_time(time_t cal_time);
void file_stat(char * filename);

main(argc,argv)
int argc;
char *argv[];
{
	if(argc < 2)
	{
		fprintf(stderr,"Error, usage is reclen <inputfile>\n");
		exit(1);
  	}
	filename=argv[1];
	openinp();

	shortreclen = MAXLINE; /* There is a reason for this */
	processrecs();

 file_stat(filename);

	tim = time(NULL);
	fprintf(stdout,"File %s contains %ld records\nLongest record (%ld) is %ld bytes\nShortest non-zero record (%ld) is %ld bytes\nAverage record length is %.2lf bytes\n",filename,reccnt,longrecnumber,longreclen,shortrecnumber,shortreclen,totalfilesize/reccnt);
	fclose(infil_ptr);
	return(0);
}

processrecs()
{
	readinp();
	while (feof(infil_ptr) == 0)
	{
		if(strlen(buf) > longreclen)
		{
			longreclen = strlen(buf);
			longrecnumber = reccnt;
		}
		if(strlen(buf) < shortreclen && strlen(buf) != 1)
		{
			shortreclen = strlen(buf);
			shortrecnumber = reccnt;
		}
		totalfilesize = totalfilesize + strlen(buf);
		readinp();
	}

	return(0);
}

openinp()
{
	if ((infil_ptr = fopen(filename,"r")) == NULL)
	{
		fprintf(stdout,"Error opening %s\n",filename);
		exit(1);
	}
	return(0);
}

readinp()
{
	fgets(buf,MAXLINE,infil_ptr);
	reccnt++;
	return(0);
}

void file_stat(char * filename)
{
  struct stat stat_p;		
  if ( -1 ==  stat (filename, &stat_p))
  {
    printf(" Error occoured attempting to stat %s\n", filename);
    exit(0);
  }
  printf("File Created       %s", format_time(stat_p.st_ctime));	
  printf("File Last Modified %s", format_time(stat_p.st_mtime));
  printf("File size is %d bytes\n", stat_p.st_size);
}

char *format_time(time_t cal_time)
{
  struct tm *time_struct;
  static char string[40];
	time_struct=localtime(&cal_time);
  strftime(string, sizeof string, "%d/%m/%Y %X\n", time_struct);
  return(string);
}