/***************************************************************
zip.c   : Produces a self-extracting zip in one go
          i.e. calls pkzip then zip2exe
usage   : zip filename
options :
Dominic : 11/4/96
***************************************************************/
#include <stdio.h>
#include <dos.h>
#include <dir.h>

char *filename;
char *filename2;
char drive[3];
char dir[80];
char file[15];
char path[80];
char syscall[80];
char ext[5];
char zipdel[13];
int flags;

main(argc,argv)
int argc;
char *argv[];
{
	if(argc < 2)
	{
		fprintf(stdout,"Error : usage is zip <filename>\n");
		fprintf(stdout,"This program will create a self-extracting zip (filename.exe)\n");
		fprintf(stdout,"from the file you pass in - NB you must have pkzip and zip2exe\n");
		fprintf(stdout,"in the path or in the same directory.\n");
		fprintf(stdout,"This program is Baltiware    Dominic 11/4/96\n");
		exit(1);
	}
	filename = argv[1];
	filename2=filename;
	flags=fnsplit(filename2,drive,dir,file,ext);

	if((strncmp(ext,".EXE",3) == 0 ) || (strncmp(ext,".exe",3) == 0 )) 
	{
		fprintf(stdout,"No can do Buster ! I'll overwrite the original .EXE file ... exiting\n");
		exit(1);
	}

	if((strncmp(ext,".ZIP",3) == 0 ) || (strncmp(ext,".zip",3) == 0 )) 
	{
		fprintf(stdout,"Sorry, I don't work on files already zipped ... exiting\n");
		exit(1);
	}

	sprintf(syscall,"pkzip %s.zip %s",file,filename);
	system(syscall);
	sprintf(syscall,"zip2exe %s.zip",file);
	system(syscall);
	sprintf(zipdel,"%s.zip",file);
	remove(zipdel);

	return(0);

}
