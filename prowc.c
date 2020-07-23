/*********************************************************
** Word Count                                           **
** Copyright 1998 Harry M. Hardjono                     **
**                                                      **
** 18 Aug 1998: version 1.0                             **
**   - Fixed long standing bug in ProcessFile() (sigh)  **
**   - Added some define & enum                         **
**   - Dedicated Error Trap Function                    **
**   - Route exit from Help & version screen            **
**                                                      **
** 17 Aug 1998: version 0.9                             **
**   - use struct for data structure                    **
**   - infilename length reduced to 256                 **
**   - faster, more complicated ProcessFile()           **
**   - More clean-up                                    **
**   - added silent & total option                      **
**   - built-in wc is only 25% faster                   **
**   - ASCII & EBCDIC compatible                        **
**                                                      **
** 14 Aug 1998: version 0.8                             **
**   - More boxes                                       **
**   - General Clean Up                                 **
**   - GetVars algorithm change!                        **
**                                                      **
** 13 Aug 1998: version 0.7                             **
**   - Fixed help screen bug                            **
**   - Reformat the program's look                      **
**   - Add beautification lines                         **
**   - Use fgetc(stdin) for stdin                       **
**   - Fixed inspace = 1;                               **
**   - Fixed stdin overflow                             **
**                                                      **
** 13 Aug 1998: version 0.6                             **
**   - complete with prompts and help screens           **
**   - initial professional version                     **
**                                                      **
*********************************************************/

/*********************************************************
* *** *** *** *** *** *** *** *** *** *** *** *** *** ***
*  *   *   *   *   *   *   *   *   *   *   *   *   *   *
*/

/*====================================================
** I N C L U D E S
**==================================================*/
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/*====================================================
** E N U M S
**==================================================*/
enum warn_flag {
  UNKNOWNMODE,
  FILEOPENERROR
};

enum warn_type {
  STRING,
  CHAR
};

enum error_flag {
  INTERNALERROR
};


/*====================================================
** D E F I N E S
**==================================================*/
#define BUFSIZE       4096
#define FILENAMESIZE  256


/*====================================================
** G L O B A L S
**==================================================*/

/*----------------------------------------------------
** Alphabets, Line, Word, Byte (char)
**
** Note:
**   - Assume byte and char are the same.
**
**--------------------------------------------------*/
struct ALWB_mode {
  int alpha;
  int lines;
  int words;
  int chars;
  int silen;
  int total;
};

struct ALWB_count {
  unsigned long alpha[256];
  unsigned long lines;
  unsigned long words;
  unsigned long chars;
};

struct ALWB_mode   mode;
struct ALWB_count  count, total;


/*----------------------------------------------------
** Look up tables
**--------------------------------------------------*/
char ins[256], iss[256];                  /* !isspace, isspace */


/*----------------------------------------------------
** Misc
**--------------------------------------------------*/
unsigned char  infilename[FILENAMESIZE];  /* Input file name */
int            argccount;                 /* argc counter */


/*====================================================
** P R O T O T Y P E S
**==================================================*/
int  GetVars (int argc, char *argv[]);
int  ProcessStdin (void);
int  ProcessFile (void);
void ShowHelp (void);
void ShowVersion(void);
int  DisplayResult (struct ALWB_mode *m, struct ALWB_count *c);
int  Warn(enum warn_flag flag, enum warn_type type, unsigned char *info);
int  Error(enum error_flag ErrorFlag, unsigned char *id);


/**************************************************************
***   ***   ***   ***   ***   ***   ***   ***   ***   ***   ***
***   ***   ***   ***   ***   ***   ***   ***   ***   ***   ***
*/

/*====================================================
** M A I N
**
** Function called:
**   - GetVars()
**   - ProcessStdin()
**   - ProcessFile()
**   - DisplayResult()
**
** Global variables used:
**   - struct mode
**   - struct count
**   - struct total
**   - infilename[]
**   - argccount
**
** Note:
**   - argccount is incremented in GetVars
**
**==================================================*/
int main(int argc, char *argv[]) {
  int          i;
  unsigned int processinput;              /* num of argv processed */

  /*--------------------------------------------------
  ** Setup
  **------------------------------------------------*/
  for (i=0;i<256;i++)  ins[i] = !(iss[i] = (isspace(i)) ? 1 : 0);
  mode.alpha = mode.silen = 0;
  mode.lines = mode.words = mode.chars = mode.total = 1;
  for (i=0;i<256;i++) total.alpha[i]=0;
  total.lines = total.words = total.chars = 0;
  argccount = processinput = 0;

  /*--------------------------------------------------
  ** Main Loop
  **------------------------------------------------*/
  while (argccount < (argc-1) || !processinput) {

    /*------------------------------------------------
    ** Initialize
    **----------------------------------------------*/
    for (i=0;i<256;i++) count.alpha[i]=0;
    count.lines = count.words = count.chars = 0;
    infilename[0] = '\0';
    processinput++;

    /*------------------------------------------------
    ** Get Variables
    **----------------------------------------------*/
    if (GetVars (argc, argv)<0)        break;
    else if (!strcmp(infilename,"-"))  ProcessStdin();
         else                          ProcessFile();

    /*------------------------------------------------
    ** Set Counters
    **----------------------------------------------*/
    for (i=0;i<256;i++) count.chars += count.alpha[i];
    count.lines = count.alpha['\n'];

    for (i=0;i<256;i++) total.alpha[i] += count.alpha[i];
    total.lines += count.lines;
    total.words += count.words;
    total.chars += count.chars;


    /*------------------------------------------------
    ** Display Result
    **----------------------------------------------*/
    if (!mode.silen)  DisplayResult (&mode, &count);

  } /* End of while (argccount<argc-1) */

  /*------------------------------------------------
  ** Show Total
  **----------------------------------------------*/
  if (processinput > 1 && infilename[0] != '\0') {
    strcpy(infilename,"total");
    if (mode.total)  DisplayResult (&mode, &total);
  }
  return 0;
} /* End of main() */

/*====================================================
** G E T V A R S
**
** Function called:
**   - ShowHelp()
**   - ShowVersion()
**
** Global variables used:
**   - struct mode
**   - infilename[]
**   - argccount
**
**==================================================*/
int GetVars (int argc, char *argv[]) {
  int i, argvlen;

  while (++argccount < argc) {
    if      (!strcmp(argv[argccount],"--lines"))        mode.lines = 1;
    else if (!strcmp(argv[argccount],"--words"))        mode.words = 1;
    else if (!strcmp(argv[argccount],"--bytes"))        mode.chars = 1;
    else if (!strcmp(argv[argccount],"--chars"))        mode.chars = 1;
    else if (!strcmp(argv[argccount],"--alphas"))       mode.alpha = 1;
    else if (!strcmp(argv[argccount],"--silent"))       mode.silen = 1;
    else if (!strcmp(argv[argccount],"--total"))        mode.total = 1;
    else if (!strcmp(argv[argccount],"--nolines"))      mode.lines = 0;
    else if (!strcmp(argv[argccount],"--nowords"))      mode.words = 0;
    else if (!strcmp(argv[argccount],"--nobytes"))      mode.chars = 0;
    else if (!strcmp(argv[argccount],"--nochars"))      mode.chars = 0;
    else if (!strcmp(argv[argccount],"--noalphas"))     mode.alpha = 0;
    else if (!strcmp(argv[argccount],"--nosilent"))     mode.silen = 0;
    else if (!strcmp(argv[argccount],"--nototal"))      mode.total = 0;
    else if (!strcmp(argv[argccount],"--help")) {
            ShowHelp();
            goto EXIT_RET;
    }
    else if (!strcmp(argv[argccount],"--version")) {
            ShowVersion();
            goto EXIT_RET;
    }
    else {
      argvlen = strlen(argv[argccount]);  /* Assume argvlen >=1 */
      if      (argv[argccount][0] != '-')  goto FILE_RET;
      else if (argvlen == 1)               goto STDIN_RET;
      else {
        for (i=1; i<argvlen;i++) {
          switch(argv[argccount][i]) {
            case 'l':   mode.lines = 1;     break;
            case 'w':   mode.words = 1;     break;
            case 'b':   mode.chars = 1;     break;
            case 'c':   mode.chars = 1;     break;
            case 'a':   mode.alpha = 1;     break;
            case 's':   mode.silen = 1;     break;
            case 't':   mode.total = 1;     break;
            case 'L':   mode.lines = 0;     break;
            case 'W':   mode.words = 0;     break;
            case 'B':   mode.chars = 0;     break;
            case 'C':   mode.chars = 0;     break;
            case 'A':   mode.alpha = 0;     break;
            case 'S':   mode.silen = 0;     break;
            case 'T':   mode.total = 0;     break;
            case 'h':   ShowHelp();
                        goto EXIT_RET;  /* break; */
            case 'v':   ShowVersion();
                        goto EXIT_RET;  /* break; */
            default :   Warn(UNKNOWNMODE,CHAR,&argv[argccount][i]);
                        break;
          }
        } /* for (i=1;i<argvlen;... */
      } /* if (argv[argcccount][0] == '-' && ... */
    }
  } /* End of while (++argccount<argc-1) */


  /*------------------------------------------------
  ** Return values
  **----------------------------------------------*/
  STDIN_RET:
  infilename[0]='-';
  infilename[1]='\0';
  return 0;

  FILE_RET:
  strncpy(infilename,argv[argccount],FILENAMESIZE);
  infilename[FILENAMESIZE-1] = '\0';
  return 1;

  EXIT_RET:
  infilename[0]='\0';
  return -1;
} /* End of GetVars() */


/*====================================================
** P R O C E S S S T D I N
**
** Global variables used:
**   - struct count
**
**==================================================*/
int ProcessStdin (void) {
  int i, inspace;

  /*------------------------------------------------
  ** Process Data
  **
  ** Note:
  **   - Assume fgetc return value ranges 0..255.
  **     Change to unsigned char as needed.
  **
  **----------------------------------------------*/
  inspace = 1;
  for (;;) {
    i = fgetc(stdin);
    if (feof(stdin) || ferror(stdin)) break;
    count.alpha[i]++;
    if (inspace && ins[i]) count.words++;
    inspace = iss[i];
  }
  clearerr(stdin);

  return 0;
} /* End of ProcessStdin */


/*====================================================
** P R O C E S S F I L E
**
** Global variables used:
**   - struct count
**   - infilename[]
**
**==================================================*/
int ProcessFile (void) {
  FILE *infile;
  static unsigned char buffer[BUFSIZE];
  int i, inspace;
  unsigned char *head, *tail;

  /*------------------------------------------------
  ** Open File
  **----------------------------------------------*/
  if ((infile=fopen(infilename,"rb")) == NULL) {
    Warn (FILEOPENERROR,STRING,infilename);
    return 1;
  }

  /*------------------------------------------------
  ** Process Data
  **----------------------------------------------*/
  inspace = 1;
  while (i=fread(buffer,1,BUFSIZE,infile)) {
    tail = &buffer[i];
    for (head = &buffer[0]; head<tail; head++) {
      ++count.alpha[*head];
      if (inspace && ins[*head]) {
          ++count.words;
          inspace = 0;
      } else {
        inspace = iss[*head];
      }
    }
  }  /* while (i=fread()) */

  /*------------------------------------------------
  ** Close File
  **----------------------------------------------*/
  fclose (infile);

  return 0;
} /* End of ProcessFile */


/*====================================================
** D I S P L A Y R E S U L T
**
** Global variables used:
**   - infilename[]
**
**==================================================*/
int DisplayResult (struct ALWB_mode *m, struct ALWB_count *c) {

  unsigned long other;
  int i;

  /*------------------------------------------------
  ** Start Here
  **----------------------------------------------*/
  if (m->lines)  printf ("lines: %lu\t",c->lines);
  if (m->words)  printf ("words: %lu\t",c->words);
  if (m->chars)  printf ("chars: %lu\t",c->chars);
  printf ("file : %s\n",infilename);

  /*------------------------------------------------
  ** Display Alphas
  **----------------------------------------------*/
  if (m->alpha) {
    other = 0;
    for (i=0;i<256;i++) if (!isalpha(i)) other += c->alpha[i];
    for (i=0;i<256;i++) if (islower(i))  c->alpha[toupper(i)] += c->alpha[i];
    for (i=0;i<256;i++) if (isupper(i))  printf ("%c %ld\n",i, c->alpha[i]);
    printf ("Other: %lu\n",other);
  }

  return 0;
} /* End of DisplayResult */



/*====================================================
** S H O W H E L P
**==================================================*/
void ShowHelp (void) {
  puts("Usage: wc [OPTION]... [FILE]...\n");
  puts("Print line, word, byte, and alphabet counts for each FILE,");
  puts("and a total line if more than one FILE is specified.");
  puts("With no FILE, or when FILE is -, read standard input.");
  puts("-c, --chars     print the byte counts");
  puts("-b, --bytes     print the byte counts");
  puts("-l, --lines     print the newline counts");
  puts("-w, --words     print the word counts");
  puts("-a, --alphas    print the alphabet counts");
  puts("-s, --silent    display grand total only");
  puts("-t, --total     display grand total");
  puts("-C, --nochars   disable byte counts");
  puts("-B, --nobytes   disable byte counts");
  puts("-L, --nolines   disable newline counts");
  puts("-W, --nowords   disable word counts");
  puts("-A, --noalphas  disable alphabet counts");
  puts("-S, --nosilent  display statistics for each file");
  puts("-T, --nototal   disable grand total");
  puts("-h, --help      display this help and exit");
  puts("-v, --version   display version info and exit");
} /* End of ShowHelp() */


/*====================================================
** S H O W V E R S I O N
**==================================================*/
void ShowVersion(void) {
  puts("Word Count 1.0");
  puts("Copyright 1998 Harry M. Hardjono");
  puts("For bug reports: dev>NULL;");
} /* End of ShowVersion() */



/**  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
**  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
*  **  **  **  **  **  **  **  **  **  **  **  **  **  **  **
*/



/*====================================================
** W A R N
**==================================================*/
int Warn(enum warn_flag WarnFlag, enum warn_type WarnType, unsigned char *info) {
  switch (WarnFlag) {
    case UNKNOWNMODE:
         switch (WarnType) {
           case CHAR: printf ("Unknown mode: %c\n-h for help.\n",*info);
                      break;
           default  : Error  (INTERNALERROR, "UNKNOWNMODE - WarnType");
                      break;
         }
         break;
    case FILEOPENERROR:
         switch (WarnType) {
           case STRING: printf ("File open error: %s\n", info);
                        break;
           default    : Error  (INTERNALERROR, "FILEOPENERROR - WarnType");
                        break;
         }
         break;
    default:
         Error (INTERNALERROR, "Invalid WarnFlag");
         break;
  }
 
  return 0;
} /* End of Warn() */
 
/*====================================================
** E R R O R
**==================================================*/
int Error(enum error_flag ErrorFlag, unsigned char id[]) {
  switch (ErrorFlag) {
    case INTERNALERROR:
         printf ("Internal error: %s\n", id);
         break;
    default:
         puts ("Internal error: Invalid ErrorFlag");
         break;
  }
 
  exit(ErrorFlag);
} /* End of Error() */

