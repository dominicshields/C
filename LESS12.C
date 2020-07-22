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
#include <dos.h>

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
int cnt, linecnt, linecntstore, i, len, hohoflag, endflag, lineno, y, gotoend;
int speckeyb, helpflag;
int scrn=20;
float version;
union scan
{
int c;
char ch[2];
} sc;

main(argc,argv)
int argc;
char *argv[];
{
	if(argc < 2)
	{
		fprintf(stdout,"Error : usage is less [-s#] [-l] <filename>\n");
		fprintf(stdout,"less help for help\n");
		exit(1);
	}

	version=1.2; 
	strcpy(verdate,"04/11/95");

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
			fprintf(stdout,"usage:  less [-s#] [-l] [filename]\n");
			fprintf(stdout,"options : -s# Screensize, no of lines, (default 20)\n");
			fprintf(stdout,"	: -l  Line numbering on\n");
			fprintf(stdout,"In the program : ?, F1 or an invalid key brings up a help screen\n");
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

		sc.c = get_key();
		if(sc.ch[0] == 0)
		{
			speckeyb = (int) (sc.ch[1]);
			specwhattodo();
		}
		else
		{
			keyb = (sc.ch[0]);
			whattodo();
		}

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
			back(scrn);
		}
		linecntstore = linecnt;
		fprintf(stdout,"End of File - %d lines - q to quit, b to go back # lines, ? for help\n",linecnt);

		while (sc.c = get_key())
		{
			if(sc.ch[0] == 0)
			{
				speckeyb = (int) (sc.ch[1]);
				specwhattodoatend();
			}
			else
			{
				keyb = (sc.ch[0]);
				whattodoatend();
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
		case '?'  : hohoflag++;helpflag++;explan();break;
		case '/'  : hohoflag++;lookfor();break;
		case 'b'  : hohoflag=0;back(scrn);break;
		case 'e'  : hohoflag=0;end();break;
		case 'g'  : hohoflag=0;fclose(infil_ptr);linecnt=0;cnt=0;openfiles();break;
		case 'i'  : info();break;
		case 'l'  : hohoflag=0;lineno=1;cls();fprintf(stdout,"Line Numbering On - Press a Key\n");break;
		case 'n'  : hohoflag=0;lookfor();break;
		case 'q'  : exit(1);break;
		case 'u'  : hohoflag=0;back(1);break;
		case 'v'  : hohoflag=0;edit();openfiles();break;
		default   : hohoflag++;fprintf(stdout,"Invalid Key %c, calling help",keyb);sleep(1);explan();break;
	}
	return(0);
}

whattodoatend()
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
		case '?'  : hohoflag++;helpflag++;explan();break;
		case '/'  : fprintf(stdout,"\aKey not available\n");break; 
		case 'b'  : hohoflag=0;back(scrn);break;
		case 'e'  : hohoflag=0;end();break;
		case 'g'  : hohoflag=0;fclose(infil_ptr);linecnt=0;openfiles();cnt=0;loop();break;
		case 'i'  : info();break;
		case 'l'  : hohoflag=0;lineno=1;cls();fprintf(stdout,"Line Numbering On - Press a Key\n");break;
		case 'n'  : fprintf(stdout,"\aKey not available\n");break; 
		case 'q'  : fprintf(stdout,"File %s contained %d lines\n",filename,linecntstore);exit(1);break;
		case 'u'  : hohoflag=0;back(1);break;
		case 'v'  : hohoflag=0;edit();openfiles();loop();break;
		default   : hohoflag++;fprintf(stdout,"Invalid Key %c, calling help",keyb);sleep(1);explan();break;
	}
	return(0);
}

specwhattodo()
/* Does the equivalent movements up and down for the special keys
   Page up = 73, Page down = 81, Up arrow = 72, Down arrow = 80,
   Home = 71, End = 79 */
{
	switch (speckeyb)
	{
		case 59  : hohoflag++;helpflag++;explan();break;
		case 81  : hohoflag=0;cnt=0;break;
		case 80  : hohoflag=0;cnt--;break;
		case 73  : hohoflag=0;back(scrn);break;
		case 79  : hohoflag=0;end();break;
		case 71  : hohoflag=0;fclose(infil_ptr);linecnt=0;cnt=0;openfiles();break;
		case 72  : hohoflag=0;back(1);break;
		default  : hohoflag++;fprintf(stdout,"Invalid Key, calling help");sleep(1);explan();break;
	}
	return(0);
}

specwhattodoatend()
/* Does the equivalent movements up and down for the special keys
   Page up = 73, Page down = 81, Up arrow = 72, Down arrow = 80,
   Home = 71, End = 79 */
{
	switch (speckeyb)
	{
		case 81 : if(endflag==1)
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
		case 80 : if(endflag==1)
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
		case 59  : hohoflag++;helpflag++;explan();break;
		case 73  : hohoflag=0;back(scrn);break;
		case 79  : hohoflag=0;end();break;
		case 71  : hohoflag=0;fclose(infil_ptr);linecnt=0;openfiles();cnt=0;loop();break;
		case 72  : hohoflag=0;back(1);break;
		default  : hohoflag++;fprintf(stdout,"Invalid Key, calling help");sleep(1);explan();break;
	}
	return(0);
}

get_key()
{
union REGS r;
	r.h.ah = 0;
	return int86(0x16,&r,&r);
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

back(howmuch)
int howmuch;
{
	int newcnt;
	int newcnt2=1;
	endflag=0;
	
	if(gotoend == 0)
	{	
		linecnt -= howmuch;
	}
	gotoend=0;
	newcnt = linecnt;
	if(newcnt < howmuch)
	{
		newcnt=howmuch;
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
		fprintf(stdout,"Input a search word (case sensitive), then press return\n");
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
	fprintf(stdout,"?, F1 or invalid key	: Bring up this screen\n");
	fprintf(stdout,"Space bar or Page Down	: go forward [s] lines (default 20)\n");
	fprintf(stdout,"Tab key or Down Arrow	: go forward 1 line\n");
	fprintf(stdout,"/			: go into search mode\n");
	fprintf(stdout,"b or Page Up		: go back [s] lines (default 20)\n");
	fprintf(stdout,"e or End		: go to the end of the file\n");
	fprintf(stdout,"g or Home		: go to the start of the file\n");
	fprintf(stdout,"i			: display further program information\n");
	fprintf(stdout,"l			: display line numbers (when followed by an action)\n");
	fprintf(stdout,"n			: search for next occurrence of string\n");
	fprintf(stdout,"q			: quit\n");
	fprintf(stdout,"u or Up Arrow		: go back 1 line\n");
	fprintf(stdout,"v			: edit the file (for UNIX reasons)\n");
	fprintf(stdout,"\nThis program is Baltiware - I wrote this while waiting \n");
	fprintf(stdout,"for my girlfriend to come home with my King Veg Balti\n");
	fprintf(stdout,"with Pilau rice and a Peshwari nan. If you like Indian\n");
	fprintf(stdout,"food this could be the program for you. \n");
	fprintf(stdout,"\nDominic - Version %2.1f %s\n\n",version,verdate);
	fprintf(stdout,"Hit any key in the above list to continue - except \n");
	fprintf(stdout,"? or F1 for reasons that should be self-evident\n");

	if(hohoflag > 1 && helpflag > 0)
	{
		fprintf(stdout,"\nNot self-evident enough eh ?  }:-)\n");
		helpflag=0;
	}
	
	return(0);
}

info()
{
	cls();
	fprintf(stdout,"I wrote the original version of this program in about 30 minutes one evening\n");
	fprintf(stdout,"simply because I was looking at files in DOS with the abundant facilities\n");
	fprintf(stdout,"that DOS possesses (NOT) and suddenly snapped.\n");
	fprintf(stdout,"Type        : What is it good for ? Absolutely nothing\n");
	fprintf(stdout,"More < file : Better, but forwards only - no features\n");
	fprintf(stdout,"Edit        : Limited by size of file - risky - accidental edits\n");
	fprintf(stdout,"\nIf you disagree and think that the above commands are the most wonderful\n");
	fprintf(stdout,"devised by human ingenuity I suggest that you press 'q' right now followed\n");
	fprintf(stdout,"by DEL less.exe.\n");
	fprintf(stdout,"\nGood, that should have got rid of the 'reinventing the wheel' bores.\n");
	fprintf(stdout,"In UNIX there are a variety of built-in file browsers and editors, most\n");
	fprintf(stdout,"people who've tried all of them agree that less written by GNU is the best.\n");
	fprintf(stdout,"I have tried to incorporate the features I liked into this program.\n");
	fprintf(stdout,"I make no claim that the program is perfect because it isn't, but I'm not\n");
	fprintf(stdout,"like INGRES, if you spot a bug - tell me, if it really is a bug rather\n");
	fprintf(stdout,"than an intentional copy of a feature of less, I'll try and fix it ASAP.\n");
	fprintf(stdout,"One thing that everyone will notice is that going backwards is slower\n");
	fprintf(stdout,"than going forwards, this is a consequence of my deliberately not reading\n");
	fprintf(stdout,"the file into memory.\n");
	fprintf(stdout,"\nHit a valid key to continue - F1 or ? for help\n");

	return(0);
}
