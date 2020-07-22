/***************************************************************
less.c  : An attempted clone of the best UNIX file browser
usage   : less [-s#] [-l] [-help] filename
options : See the function - explan() or hit ?/F1 when running
          the program. 
Dominic : First written 25/10/95 - See variables version and 
          verdate for the version number and date.

Changes and reason :
Version 1.54 - Enabled Toggling of Line Numbering and 
made the line numbering command less tacky altogether
by getting rid of the need to press anything else.
Version 1.55 - Added a print facility and as a side effect
solved the problem of long lines resulting in the first part 
of the file getting chopped on occasion.
Also re-wrote some of the on-line documentation.
***************************************************************/
#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <dos.h>
#include <dir.h>
#include <time.h>

#define MAXLINE 256	/* max record length - increase if necessary */

FILE *inp;			/* printing input file */
FILE *outp;		/* printing output file */
FILE *infil_ptr;		/* read input file */

char *input;
char *output;

char *filename;
char *p1;
char *p2;
char *s;
char buf[MAXLINE];
char bufin[MAXLINE];
char buf2[81];
char arr[MAXLINE][80];
char syscall[MAXLINE];
char syscomm[MAXLINE];
char okprcall[MAXLINE];
char find[MAXLINE];
char drive[3];
char dir[MAXLINE];
char file[15];
char path[80];
char ext[4];
char keyb;
char charno[4];
char editor[80];
char verdate[10];

int i, j, y, len, len1, len2, sub, recs, totfiles, speckeyb,cnt1, linelen,linediv;
int flags, hohoflag, endflag, helpflag, lookflag, editor_flag, gotoend;
int scrn=21;

long cnt, linecnt, linecntstore, lineno;

float version;

time_t tim;

union scan
{
int c;
char ch[2];
} sc;

/*************************** MAIN ***************************/
main(argc,argv)
int argc;
char *argv[];
{
	if(argc < 2)
	{
		fprintf(stdout,"Error : usage is less [-s#] [-l] [-v] <filename>\n");
		fprintf(stdout,"less -help for help\n");
		exit(1);
	}

	version=1.55; 
	strcpy(verdate,"01/01/1998");

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
		if(strncmp(argv[i],"-help",5) == 0)
		{
			fprintf(stdout,"This program pages output in a similar way to the unix less command.\n");
			fprintf(stdout,"usage:  less [-s#] [-l] [-v] filename (DOS wildcards allowed)\n");
			fprintf(stdout,"options : -s# Screensize, no of lines, (default 21)\n");
			fprintf(stdout,"	: -l  Line numbering on\n");
			fprintf(stdout,"	: -v  Name your editor (default is C:\\DOS\\EDIT.COM)\n");
			fprintf(stdout,"Please note - no spaces between option and value\n");
			fprintf(stdout,"In the program : ?, F1 or an invalid key brings up a help screen\n");
			fprintf(stdout,"Dominic - Version %3.2f %s\n",version,verdate);
			exit(1);
		}
		if(strncmp(argv[i],"-l",2) == 0)
		{
			lineno=1;
		}
		if(strncmp(argv[i],"-v",2) == 0)
		{
			p1 = argv[i];
			p1 += 2;
			strcpy(editor,p1);
			editor_flag=1;
		}
	}

	filename=argv[argc-1];

	dirlook();

	openfiles();

	loop();

	fclose(infil_ptr);

	return(0);
}

/*************************** DIRLOOK ***************************/
dirlook()
{
	s=filename;
	flags=fnsplit(s,drive,dir,file,ext);
	sprintf(path,"%s%s",drive,dir);
	sprintf(syscall,"DIR %s /A:-D /B /L > c:\\@@@@@@@@.@@@",filename);
	system(syscall);

	if ((infil_ptr = fopen("c:\\@@@@@@@@.@@@","r")) == NULL)
	{
		fprintf(stdout,"Error opening c:\\@@@@@@@@.@@@\n");
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
		   && (strncmp(p2,"@@@",3) != 0))    
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
	remove("c:\\@@@@@@@@.@@@"); 
	return(0);
}

/*************************** OPENFILES ***************************/
openfiles()
{
	endflag=0;
	clrscr();

	if(sub >= totfiles)
	{
		fprintf(stdout,"No more files to browse\n",arr[sub]);
		exit(1);
	}
	
	if(totfiles > 1 && linecnt == 0 && lookflag == 0)
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

/*************************** LOOP ***************************/
loop()
{
	clrscr();

	while(feof(infil_ptr) == 0)
	{
		while(cnt<scrn)
		{
			read1();
			linediv = (linelen/70);	/* COPES WITH LINE WRAPPING PROBLEMS */
			if(linediv > 0)
			{
				cnt += linediv;
			}
			if(lineno == 1)
			{
				fprintf(stdout,"%ld ",linecnt);
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

/*************************** READ1 ***************************/
read1()
{
	fgets(buf,MAXLINE,infil_ptr);
	linelen = strlen(buf);
	if(feof(infil_ptr) != 0)
	{
		endflag=1;
		if(gotoend == 1)
		{
			back(scrn);
		}
		linecntstore = linecnt;
/* lincnt replaced with lincntstore */
		fprintf(stdout,"End of File %s - %ld lines - q to quit, F1 or ? for help\n",arr[sub],linecntstore);
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

/*************************** WHATTODO ***************************/
whattodo()
{
	switch (tolower(keyb))
	{
		case ' '  : hohoflag=0;cnt=0;break;
		case '\r' : hohoflag=0;cnt--;break;
		case '?'  : hohoflag++;helpflag++;explan();break;
		case '/'  : hohoflag=0;lookfor();break;
		case '+'  : if(totfiles > 1)
			    {
				hohoflag=0;
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
				hohoflag=0;
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
		case 'l'  : if(lineno == 0)
			{
				hohoflag=0;
				lineno=1;
				clrscr();
				back(scrn);
				cnt=0;break;
			}
			else
			{
				hohoflag=0;
				lineno=0;
				clrscr();
				back(scrn);
				cnt=0;break;
			}
		case 'n'  : hohoflag=0;lookfor();break;
		case 'p'  : hohoflag=0;okpr();break;
		case 'q'  : exit(1);break;
		case '\x1b' : exit(1);break;
		case 'u'  : hohoflag=0;back(1);break;
		case 'v'  : hohoflag=0;edit();openfiles();break;
		default   : hohoflag++;fprintf(stdout,"Invalid Key %c, calling help",keyb);sleep(1);explan();break;
	}
	return(0);
}

/*************************** WHATTODOATEND ***************************/
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
				hohoflag=0;
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
				hohoflag=0;
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
		case 'l'  : if(lineno == 0)
			{
				hohoflag=0;
				lineno=1;
				clrscr();
				back(scrn);
				cnt=0;
				loop();
				break;
			}
			else
			{
				hohoflag=0;
				lineno=0;
				clrscr();
				back(scrn); 
				cnt=0;
				loop();
				break;
			}		
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
		case 'p'  : hohoflag=0;okpr();break;
		case 'q'  : fprintf(stdout,"File %s contained %ld lines\n",arr[sub],linecntstore);exit(1);break;
		case '\x1b'  : fprintf(stdout,"File %s contained %ld lines\n",filename,linecntstore);exit(1);break;
		case 'u'  : hohoflag=0;back(1);break;
		case 'v'  : hohoflag=0;edit();openfiles();loop();break;
		default   : hohoflag++;fprintf(stdout,"Invalid Key %c, calling help",keyb);sleep(1);explan();break;
	}
	return(0);
}

/*************************** SPECWHATTODO ***************************/
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
/* CNT = 0 INSERTED */
		case 79  : hohoflag=0;cnt=0;end();break;
		case 71  : hohoflag=0;fclose(infil_ptr);linecnt=0;cnt=0;openfiles();break;
		case 72  : hohoflag=0;back(1);break;
		default  : hohoflag++;fprintf(stdout,"Invalid Key, calling help");sleep(1);explan();break;
	}
	return(0);
}

/*************************** SPECWHATTODOATEND ***************************/
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


/*************************** GET_KEY ***************************/
get_key()
{
union REGS r;
	r.h.ah = 0;
	return int86(0x16,&r,&r);
}


/*************************** EDIT ***************************/
edit()
{
	cnt=0;
	fclose(infil_ptr);
	if(editor_flag != 0)
	{
		fprintf(stdout,"Looking for editor %s\n",editor);
		sleep(2);
		sprintf(syscall,"%s %s",editor,filename);
	}
	else
	{
		sprintf(syscall,"C:\\DOS\\EDIT %s",filename);
	}
	system(syscall);
	linecnt=0;
	return(0);
}

/*************************** BACK ***************************/
back(howmuch)
int howmuch;
{
	long newcnt;
	long newcnt2=1;
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
/*********** latest change here *****************/
		linediv = (linelen/79);	/* COPES WITH LINE WRAPPING PROBLEMS */
			if(linediv > 0)
			{
				newcnt2 += linediv;
			}
		if(newcnt2>=(newcnt-scrn))
		{
			if(lineno == 1)
			{
				fprintf(stdout,"%ld ",newcnt2);
			}
			fprintf(stdout,"%s",buf);
		}
		newcnt2++;
	}
	linecnt=--newcnt2;
	return(0);
}

/*************************** END ***************************/
end()
{
	gotoend=1;
	while(feof(infil_ptr) == 0)
	{
		read1();
	}	
	return(0);
}

/*************************** LOOKFOR ***************************/
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
	lookflag++;
	openfiles();
	lookflag=0;
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
					fprintf(stdout,"%ld ",linecnt);
				}
				fprintf(stdout,"%s",buf);
				cnt=scrn/2;
				return(0);
			}
			p1++;
		}
	}
}

/*************************** OKPR ***************************/
okpr()
{
   tim = time(NULL);
 
   input = arr[sub];
 
   if ((inp = fopen(input,"r")) == NULL)
   {
      fprintf(stderr,"Error opening %s does it exist?\n",input);
      exit(1);
   }

   if ((outp = fopen("lesslpt.txt","w")) == NULL)
   {
      fprintf(stderr,"Error opening %s \n",output);
      exit(1);
   } 

   fprintf(outp,"\tFile %s \tPrinted at  %s\n\n",input,ctime(&tim));
   readpr();
   while (feof(inp) == 0)
   {
	fprintf(outp,"%d\t%s",cnt1,bufin);
	readpr();
   }

   fclose(inp);
   fclose(outp);
   strcpy(syscomm,"PRINT /d:lpt1 lesslpt.txt");
   system(syscomm);
   return 0;
 }

/*************************** READPR ***************************/
 readpr()
 {
  fgets(bufin,MAXLINE,inp);
  if (feof(inp) != 0)
  {
   return 0;
  }
  cnt1++;
  return 0;
 }

/*************************** EXPLAN ***************************/
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
	fprintf(stdout,"l			: toggles display of line numbers on/off\n");
	fprintf(stdout,"n			: search for next occurrence of string (in search mode)\n");
	fprintf(stdout,"p			: print the file (with line numbers and a title)\n");
	fprintf(stdout,"q or esc		: quit\n");
	fprintf(stdout,"u or Up Arrow		: go back 1 line\n");
	fprintf(stdout,"v			: edit the file - default editor is C:\\DOS\\EDIT.COM\n");
	fprintf(stdout,"\nThis program is Baltiware\n");
	fprintf(stdout,"Dominic - Version %3.2f %s\n\n",version,verdate);
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

/*************************** INFO ***************************/
info()
{
	clrscr();
	fprintf(stdout,"I wrote the original version of this program in C in about 30 minutes\n");
	fprintf(stdout,"one evening simply because I was looking at files in DOS using type\n");
	fprintf(stdout,"and thought that I couldn't do a worse job.\n");
	fprintf(stdout,"In UNIX there are a variety of built-in file browsers and editors, most\n");
	fprintf(stdout,"people who've tried all of them agree that less written by GNU is the best.\n");
	fprintf(stdout,"I have tried to incorporate the features I liked into this program.\n");
	fprintf(stdout,"I make no claim that the program is perfect because it isn't.\n");
	fprintf(stdout,"If you spot a bug - tell me, if it really is a bug rather\n");
	fprintf(stdout,"than an intentional copy of a feature of less, I'll try and fix it.\n");
	fprintf(stdout,"One thing that you may notice is that going backwards is slower\n");
	fprintf(stdout,"than going forwards, this is a consequence of my deliberately not reading\n");
	fprintf(stdout,"the file into memory and having to clear the screen.\n");
	fprintf(stdout,"If you don't see the point of this program as you think *everything*\n");
	fprintf(stdout,"should be done in Windows then delete it.\n\n");
	fprintf(stdout,"\nHit a valid key to continue - F1 or ? for help\n");

	return(0);
}

/*************************** THANKS ***************************/
thanks()
{
	clrscr();
	fprintf(stdout,"Thanks to\n\n");
	fprintf(stdout,"Ian Fisher\n");
	fprintf(stdout,"Mark (UNIX) Harris\n");
	fprintf(stdout,"Paul Kindred\n");
	fprintf(stdout,"Chris Norman\n");
	fprintf(stdout,"Mike Villars\n");
	fprintf(stdout,"Graham Willis\n\n");
	fprintf(stdout,"for being willing to test it and to offer suggestions :\n\n");	
	fprintf(stdout,"\nHit a valid key to continue - F1 or ? for help\n");
	return(0);
}