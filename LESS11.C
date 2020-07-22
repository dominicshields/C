/***************************************************************
less.c  : An attempted clone of the best UNIX file browser
usage   : less [-s#] [-l] [help] filename
options : See the last function - explan() or hit ? when running
          the program. 
Dominic : First written 25/10/95
***************************************************************/
#include <stdio.h>
#include <conio.h>
#include <ctype.h>

#define MAXLINE 256  /* max record length - increase if necessary */

FILE *infil_ptr;

char *filename;
char *p1;
char buf[MAXLINE];
char syscall[MAXLINE];
char find[MAXLINE];
char keyb;
char charno[4];
char verdate[10];
int cnt, linecnt, i, len, hohoflag, endflag, lineno, y, gotoend;
int scrn=20;
float version;

main(argc,argv)
int argc;
char *argv[];
{
	if(argc < 2)
	{
		fprintf(stdout,"Error : usage is less [-s#] [-l] <filename>\n");
		exit(1);
	}

	version=1.1; 
	strcpy(verdate,"27/10/95");

	for (i=1;i<argc;i++)
	{
		if(strncmp(argv[i],"-s",2) == 0)
		{
			p1 = argv[i];
			p1 += 2;
			len = strcspn(p1," ");
			strncpy(charno,p1,len);
			charno[len]='\0';
			scrn = atoi(charno);
		}
		if(strncmp(argv[i],"help",4) == 0)
		{
			fprintf(stdout,"This program pages output in a similar way to the unix less command.\n");
			fprintf(stdout,"usage:  less [-s#] [-l] [file]\n");
			fprintf(stdout,"options : -s# Screensize, no of lines, (default 20)\n");
			fprintf(stdout,"	: -l  Line numbering on\n");
			fprintf(stdout,"Dominic - Version %2.1f %s\n",version,verdate);
			exit(1);
		}
		if(strncmp(argv[i],"-l",2) == 0)
		{
			lineno=1;
		}
	}

	filename=argv[argc-1];
	
	openfiles();

	loop();

	fclose(infil_ptr);

	return(0);
}

openfiles()
{
	cls();

	if((infil_ptr = fopen(filename,"r")) == NULL)
	{
		fprintf(stdout,"Error opening %s\n",filename);
		exit(1);
	}
	return(0);
}

loop()
{
	cls();

	while(feof(infil_ptr) == 0)
	{
		while(cnt<=scrn)
		{
			read1();
			if(lineno == 1)
			{
				fprintf(stdout,"%d ",linecnt);
			}
			fprintf(stdout,"%s",buf);
			cnt++;
		}

		keyb = getch();
		whattodo();
	}
	return(0);
}

read1()
{
	fgets(buf,MAXLINE,infil_ptr);
	if(feof(infil_ptr) != 0)
	{
		endflag=1;
		if(gotoend == 1)
		{
			back();
		}
		fprintf(stdout,"End of File - %d lines - q to quit, b to go back # lines, ? for help\n",linecnt);
		while (keyb = getch())
		{
			switch (tolower(keyb))
			{
				case ' '  : if(endflag==1)
				            {
						fprintf(stdout,"\aKey not available\n");
					    }
					    else
					    {
						hohoflag=0;
						cnt=0;
						loop();
					    };
					    break;
				case '\t' : if(endflag==1)
				            {
						fprintf(stdout,"\aKey not available\n");
					    }
					    else
					    {
						hohoflag=0;
						cnt--;
						loop();
					    };
					    break;
				case '?'  : hohoflag++;explan();break;
				case '/'  : fprintf(stdout,"\aKey not available\n");break; 
				case 'b'  : hohoflag=0;back();break;
				case 'e'  : hohoflag=0;end();break;
				case 'g'  : hohoflag=0;fclose(infil_ptr);linecnt=0;openfiles();cnt=0;loop();break;
				case 'l'  : hohoflag=0;lineno=1;cls();fprintf(stdout,"Line Numbering On - Press a Key\n");break;
				case 'n'  : fprintf(stdout,"\aKey not available\n");break; 
				case 'q'  : fprintf(stdout,"File %s contained %d lines\n",filename,linecnt);exit(1);break;
				case 'v'  : hohoflag=0;edit();openfiles();loop();break;
				case '\0' : hohoflag=0;fprintf(stdout,"Please don't use any function keys ... exiting");sleep(1);exit(1);break;
				default   : hohoflag=0;fprintf(stdout,"Invalid Key %c, calling help",keyb);sleep(1);explan();break;
			}
		}
	}
	linecnt++;
	return(0);
}

whattodo()
{
	switch (tolower(keyb))
	{
		case ' '  : hohoflag=0;cnt=0;break;
		case '\t' : hohoflag=0;cnt--;break;
		case '?'  : hohoflag++;explan();break;
		case '/'  : hohoflag++;lookfor();break;
		case 'b'  : hohoflag=0;back();break;
		case 'e'  : hohoflag=0;end();break;
		case 'g'  : hohoflag=0;fclose(infil_ptr);linecnt=0;cnt=0;openfiles();break;
		case 'l'  : hohoflag=0;lineno=1;cls();fprintf(stdout,"Line Numbering On - Press a Key\n");break;
		case 'n'  : hohoflag=0;lookfor();break;
		case 'q'  : exit(1);break;
		case 'v'  : hohoflag=0;edit();openfiles();break;
		case '\0' : hohoflag=0;fprintf(stdout,"Please don't use any function keys ... exiting");sleep(1);exit(1);break;
		default   : hohoflag=0;fprintf(stdout,"Invalid Key %c, calling help",keyb);sleep(1);explan();break;
	}
	return(0);
}

edit()
{
	cnt=0;
	fclose(infil_ptr);
	sprintf(syscall,"C:\\DOS\\EDIT %s",filename);
	system(syscall);
	linecnt=0;
	return(0);
}

back()
{
	int newcnt;
	int newcnt2=1;
	endflag=0;

	cls();
	
	if(gotoend == 0)
	{	
		linecnt -= scrn;
	}
	gotoend=0;
	newcnt = linecnt;
	if(newcnt < scrn)
	{
		newcnt=scrn;
	}

	fclose(infil_ptr);
	
	openfiles();
	
	while(newcnt2<=newcnt)
	{
		read1();
		if(newcnt2>=(newcnt-scrn))
		{
			if(lineno == 1)
			{
				fprintf(stdout,"%d ",newcnt2);
			}
			fprintf(stdout,"%s",buf);
		}
		newcnt2++;
	}
	linecnt=--newcnt2;
	return(0);
}

end()
{
	gotoend=1;
	while(feof(infil_ptr) == 0)
	{
		read1();
	}	
	return(0);
}

cls()
{
	strcpy(syscall,"CLS");
	system(syscall);
	return(0);
}

lookfor()
{
	if(keyb == '/')
	{
		fprintf(stdout,"Input a search word, then press return\n");
		gets(find);
		fprintf(stdout,"Searching for %s\n",find);
	}
	while(1 == 1)
	{
		read1();
		p1 = buf;
		for(y=0;y<strlen(buf);y++)
		{
			if (strncmp(p1,find,strlen(find)) == 0)
			{
				cls();
				if(lineno == 1)
				{
					fprintf(stdout,"%d ",linecnt);
				}
				fprintf(stdout,"%s",buf);
				cnt=1;
				return(0);
			}
			p1++;
		}
	}
	return(0);
}

explan()
{
	cls();
	fprintf(stdout,"less - a pale imitation of the UNIX command for MS-DOS\n");
	fprintf(stdout,"The functions are as follows :\n");
	fprintf(stdout,"? or invalid key : Bring up this screen\n");
	fprintf(stdout,"Space bar        : go forward [s] lines (default 20)\n");
	fprintf(stdout,"Tab key          : go forward 1 line\n");
	fprintf(stdout,"/                : go into search mode\n");
	fprintf(stdout,"b	         : go back [s] lines (default 20)\n");
	fprintf(stdout,"e 	         : go to the end of the file\n");
	fprintf(stdout,"g 	         : go to the start of the file\n");
	fprintf(stdout,"l 	         : display line numbers (when followed by an action)\n");
	fprintf(stdout,"n 	         : search for next occurrence of string\n");
	fprintf(stdout,"q 	         : quit\n");
	fprintf(stdout,"v 	         : edit the file (for UNIX reasons)\n");
	fprintf(stdout,"\nThis program is Baltiware - I wrote this while waiting \n");
	fprintf(stdout,"for my girlfriend to come home with my King Veg Balti\n");
	fprintf(stdout,"with Pilau rice and a Peshwari nan. If you like Indian\n");
	fprintf(stdout,"food this could be the program for you. \n");
	fprintf(stdout,"\nDominic - Version %2.1f %s\n\n",version,verdate);
	fprintf(stdout,"Oh .. I nearly forgot, hit any key in the above list\n");
	fprintf(stdout,"to continue - except ? for reasons that should be\n");
	fprintf(stdout,"self-evident\n");

	if(hohoflag > 1)
	{
		fprintf(stdout,"\nNot self-evident enough eh ?  }:-)\n");
	}
	
	return(0);
}
