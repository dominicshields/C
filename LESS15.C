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
#include <dir.h>

#define MAXLINE 256  /* max record length - increase if necessary */

FILE *infil_ptr;

char *filename;
char *p1;
char *p2;
char *s;
char buf[MAXLINE];
char buf2[81];
char arr[MAXLINE][80];
char syscall[MAXLINE];
char find[MAXLINE];
char drive[3];
char dir[MAXLINE];
char file[15];
char path[80];
char ext[4];
int flags;
char keyb;
char charno[4];
char verdate[10];
int cnt, linecnt, linecntstore, i, len, hohoflag, endflag, lineno, y, gotoend;
int speckeyb, helpflag, sub, j, totfiles, recs, len1, len2;
int scrn=21;
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

	version=1.5; 
	strcpy(verdate,"11/11/95");

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
			fprintf(stdout,"usage:  less [-s#] [-l] [filename] DOS wildcards allowed\n");
			fprintf(stdout,"options : -s# Screensize, no of lines, (default 21)\n");
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

	dirlook();

	openfiles();

	loop();

	fclose(infil_ptr);

	return(0);
}

dirlook()
{
	s=filename;
	flags=fnsplit(s,drive,dir,file,ext);
	sprintf(path,"%s%s",drive,dir);
	sprintf(syscall,"DIR %s /A:-D /B /L > c:\\zzzzzzzz.zzz",filename);
	system(syscall);

	if ((infil_ptr = fopen("c:\\zzzzzzzz.zzz","r")) == NULL)
	{
		fprintf(stdout,"Error opening c:\\zzzzzzzz.zzz\n");
		exit(1);
	}

	fgets(buf2,81,infil_ptr);
	while(feof(infil_ptr) == 0)
	{
		j=0;
		p2 = buf2;
		while(p2[0] != '.' && j<8)
		{
			p2++;
			j++;
		}
		p2++;
		if((strncmp(p2,"exe",3) != 0) && (strncmp(p2,"com",3) != 0)
		   && (strncmp(p2,"obj",3) != 0) && (strncmp(p2,"bmp",3) != 0)    
		   && (strncmp(p2,"dll",3) != 0) && (strncmp(p2,"wav",3) != 0)    
		   && (strncmp(p2,"gif",3) != 0) && (strncmp(p2,"pcx",3) != 0)    
		   && (strncmp(p2,"zip",3) != 0) && (strncmp(p2,"lha",3) != 0)    
		   && (strncmp(p2,"mid",3) != 0) && (strncmp(p2,"rtl",3) != 0)    
		   && (strncmp(p2,"zzz",3) != 0))    
		{
			recs++;
			len1 = strlen(path);
			len2 = strcspn(buf2," ");
			strcpy(arr[sub],path);
			strncat(arr[sub],buf2,len2);
			len1 += len2-1;
			arr[sub][len1]='\0';
			sub++;
		}
		for(j=0;j<80;j++)
		{
			buf2[j]='\0';
		}
		fgets(buf2,81,infil_ptr); 
	}
	if(recs == 0)
	{
		fprintf(stdout,"There is no readable file of this name\n");
		exit(1);
	}
	if(sub > 1)
	{
		fprintf(stdout,"%d files to read\n",sub);
		sleep(2);
	}
	totfiles=sub;
	sub=0;
	fclose(infil_ptr);
	remove("c:\\zzzzzzzz.zzz"); 
	return(0);
}

openfiles()
{
	endflag=0;
	clrscr();

	if(sub >= totfiles)
	{
		fprintf(stdout,"No more files to browse\n",arr[sub]);
		exit(1);
/*		sub = totfiles - 1;
		sleep(1); */
	}
	
	if(totfiles > 1 && linecnt == 0)
	{
		fprintf(stdout,"File %s\n",arr[sub]);
		sleep(1);
	}

	if((infil_ptr = fopen(arr[sub],"r")) == NULL)
	{
		fprintf(stdout,"Error opening %s\n",filename);
		exit(1);
	}
	return(0);
}

loop()
{
	clrscr();

	while(feof(infil_ptr) == 0)
	{
		while(cnt<scrn)
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
		fprintf(stdout,"End of File %s - %d lines - q to quit, F1 or ? for help\n",arr[sub],linecnt);
		if(totfiles > 1)
		{
			fprintf(stdout,"%d more files available to browse\n",(totfiles-sub)-1);
		}

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
		case '\r' : hohoflag=0;cnt--;break;
		case '?'  : hohoflag++;helpflag++;explan();break;
		case '/'  : hohoflag++;lookfor();break;
		case '+'  : if(totfiles > 1)
			    {
				hohoflag++;
				fclose(infil_ptr);
				linecnt=0;
				cnt=0;
				sub++;
				openfiles();
			    }
			    else
			    {
				fprintf(stdout,"\aKey not available\n");
			    };
			    break;
		case '-'  : if(totfiles > 1 && sub > 0)
			    {
				hohoflag++;
				fclose(infil_ptr);
				linecnt=0;
				cnt=0;
				sub--;
				openfiles();
			    }
			    else
			    {
				fprintf(stdout,"\aKey not available");
				if(totfiles > 1)
				{
					fprintf(stdout," - You are in the first file");
				}
				fprintf(stdout,"\n");
			    };
			    break;
		case 'b'  : hohoflag=0;back(scrn);break;
		case 'e'  : hohoflag=0;end();break;
		case 'g'  : hohoflag=0;fclose(infil_ptr);linecnt=0;cnt=0;openfiles();break;
		case 'i'  : hohoflag=0;info();break;
		case 'l'  : hohoflag=0;lineno=1;clrscr();fprintf(stdout,"Line Numbering On - Press a Key\n");break;
		case 'n'  : hohoflag=0;lookfor();break;
		case 'q'  : exit(1);break;
		case '\x1b' : exit(1);break;
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
		case '\r' : if(endflag==1)
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
		case '+'  : if(totfiles > 1)
			    {
				hohoflag++;
				fclose(infil_ptr);
				linecnt=0;
				cnt=0;
				sub++;
				openfiles();
				loop(); 
			    }
			    else
			    {
				fprintf(stdout,"\aKey not available\n");
			    };
			    break;
		case '-'  : if(totfiles > 1 && sub > 0)
			    {
				hohoflag++;
				fclose(infil_ptr);
				linecnt=0;
				cnt=0;
				sub--;
				openfiles();
				loop();
			    }
			    else
			    {
				fprintf(stdout,"\aKey not available");
				if(totfiles > 1)
				{
					fprintf(stdout," - You are in the first file");
				}
				fprintf(stdout,"\n");
			    };
			    break;
		case 'b'  : hohoflag=0;back(scrn);break;
		case 'e'  : hohoflag=0;end();break;
		case 'g'  : hohoflag=0;fclose(infil_ptr);linecnt=0;openfiles();cnt=0;loop();break;
		case 'i'  : hohoflag=0;info();break;
		case 'l'  : hohoflag=0;lineno=1;clrscr();fprintf(stdout,"Line Numbering On - Press a Key\n");break;
		case 'n'  : if(endflag==1)
		            {
				fprintf(stdout,"\aKey not available\n");
			    }
			    else
			    {
				hohoflag=0;
				lookfor();
				loop();
			    };
			    break;
		case 'q'  : fprintf(stdout,"File %s contained %d lines\n",arr[sub],linecntstore);exit(1);break;
		case '\x1b'  : fprintf(stdout,"File %s contained %d lines\n",filename,linecntstore);exit(1);break;
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
		case 60  : hohoflag=0;thanks();break;
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
		case 60  : hohoflag=0;thanks();break;
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

lookfor()
{
	if(keyb == '/')
	{
		fprintf(stdout,"Input a search word (case sensitive), then press return\n");
		gets(find);
		fprintf(stdout,"Searching for %s\n",find);
		linecntstore=(linecnt-scrn);
	}
	else
	{
		linecntstore=(linecnt-(scrn-(scrn/2)));
	}
	
	fclose(infil_ptr);
	linecnt=0;
	cnt=0;
	openfiles();
	while(linecnt < linecntstore)
	{
		read1();
	}
	while(1 == 1)
	{
		read1();
		p1 = buf;
		for(y=0;y<strlen(buf);y++)
		{
			if (strncmp(p1,find,strlen(find)) == 0)
			{
				clrscr();
				if(lineno == 1)
				{
					fprintf(stdout,"%d ",linecnt);
				}
				fprintf(stdout,"%s",buf);
				cnt=scrn/2;
				return(0);
			}
			p1++;
		}
	}
}

explan()
{
	clrscr();
	fprintf(stdout,"less - a pale imitation of the UNIX command for MS-DOS\n");
	fprintf(stdout,"?, F1 or invalid key	: Bring up this screen\n");
	fprintf(stdout,"F2 			: Acknowlegements\n");
	fprintf(stdout,"Space bar or Page Down	: go forward [s] lines (default 21)\n");
	fprintf(stdout,"Enter key or Down Arrow	: go forward 1 line\n");
	fprintf(stdout,"/			: go into search mode\n");
	fprintf(stdout,"+			: go to next file (only if you used wildcards)\n");
	fprintf(stdout,"-			: go to previous file (only if you used wildcards)\n");
	fprintf(stdout,"b or Page Up		: go back [s] lines (default 21)\n");
	fprintf(stdout,"e or End		: go to the end of the file\n");
	fprintf(stdout,"g or Home		: go to the start of the file\n");
	fprintf(stdout,"i			: display further program information\n");
	fprintf(stdout,"l			: display line numbers (when followed by an action)\n");
	fprintf(stdout,"n			: search for next occurrence of string (in search mode)\n");
	fprintf(stdout,"q or esc		: quit\n");
	fprintf(stdout,"u or Up Arrow		: go back 1 line\n");
	fprintf(stdout,"v			: edit the file (for UNIX reasons)\n");
	fprintf(stdout,"\nThis program is Baltiware\n");
	fprintf(stdout,"\nDominic - Version %2.1f %s\n\n",version,verdate);
	fprintf(stdout,"Hit any key in the above list to continue - except \n");
	fprintf(stdout,"? or F1 for reasons that should be self-evident\n");

	if(hohoflag > 1 && helpflag > 0)
	{
		fprintf(stdout,"\nNot self-evident enough eh ?  }:-)\n");
		helpflag=0;
	}
	helpflag=0;
	
	return(0);
}

info()
{
	clrscr();
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
	fprintf(stdout,"One thing that you may notice is that going backwards is slower\n");
	fprintf(stdout,"than going forwards, this is a consequence of my deliberately not reading\n");
	fprintf(stdout,"the file into memory and having to clear the screen.\n");
	fprintf(stdout,"\nHit a valid key to continue - F1 or ? for help\n");

	return(0);
}

thanks()
{
	clrscr();
	fprintf(stdout,"I'd like to thank the following people for NOT saying (inaccurately)\n");
	fprintf(stdout,"'What is the point of writing a program that duplicates things that DOS or\n");
	fprintf(stdout,"Windows already does ?' and being willing to test it and to offer suggestions :\n\n");
	fprintf(stdout,"Ian Fisher\n");
	fprintf(stdout,"Mark (UNIX) Harris\n");
	fprintf(stdout,"Paul Kindred\n");
	fprintf(stdout,"Chris Norman\n");
	fprintf(stdout,"Mike Villars\n");
	fprintf(stdout,"Graham Willis\n");
	fprintf(stdout,"\nHit a valid key to continue - F1 or ? for help\n");
	return(0);
}
