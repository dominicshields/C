/**************************************************************************
PROGRAM NAME : postcode  (source postcode.c)

PURPOSE      : not sure yet

PARAMETERS   : filename, fieldpos, reclen

OUTPUT       : ok postcodes , crap postcodes , counts

WARNING      : be careful when crossing busy roads


COMMENTS     : The left hand side of the postcode is referred to as the
	       outward code and the right hand side is referred to as the
	       inward code.
	       Postcodes are generally in one of four formats, these are
	       A1 1AA , A11 1AA , AA1 1AA , AA11 1AA
	       where A is a letter and 1 is a number, however in London
	       things are more complicated and the following are seen
	       A1A 1AA , A11A 1AA
	       The inward postcode is always the same.
	       In addition the letters  C I K M O V  are not permitted
	       to be present in the inward code.
	       The rules for the outward code are more complicated.
**************************************************************************/
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
		     /* file pointers */
FILE *inp;           /* input file    */
FILE *out;           /* OK file       */
FILE *err;           /* error file    */

char *p1;
char *input;

char buf[500];       /* buffer area to read input file */

int pos;
int reclen;
int spacepos;
int error;
long cnt1=0;
long cnt2=0;
long cnt3=0;
time_t tim;
			       /* ILLEGAL CHARACTERS */
char second[3]={'J','Z','0'};
char third[15]={'B','C','D','F','G','I','J','K','L','O','Q','S','U','W','Z'};
char fourth[14]={'C','D','F','G','I','J','K','L','O','Q','S','T','U','Z'};
char cikmov[6]={'C','I','K','M','O','V'};    /* INWARD CODE */

main(argc,argv)
int argc;
char *argv[];
{
   int reply;

   if(argc < 3)
   {
     fprintf(stderr,"Error, usage is postcode <filename> <fieldpos> <reclen>\n");
      exit(1);
   }

   tim = time(NULL);
   fprintf(stderr,"\nProgram Begins  %s\n",ctime(&tim));

   input = argv[1];            /* filename 1 assigned to pointer */
   pos = *argv[2];
   reclen = *argv[3];

   pos--;
   reclen++;

   if ((inp = fopen(input,"r")) == NULL)
   {
      fprintf(stderr,"Error opening %s does it exist?\n",input);
      exit(1);
   }

   if ((out = fopen("pcodesok","w")) == NULL)
   {
      fprintf(stderr,"Error opening file postcodesok\n");
      exit(1);
   }

   if ((err = fopen("pcodeserrs","w")) == NULL)
   {
      fprintf(stderr,"Error opening file pcodeserrs\n");
      exit(1);
   }

   read1();

   while ( (feof(inp) == 0) )
   {
     p1 = buf;
     p1 = p1 + pos;
     fprintf(stdout,"%s\n",p1);
     findspace();
     if (error == 1)
     {
       chkpost();
     }
     else
     {
       crap();
     }
     read1();
   }

  fclose(inp);
  fclose(out);


  fprintf(stdout,"Records read from input file      = %ld\n",cnt1);
  fprintf(stdout,"Number of postcodes OK            = %ld\n",cnt2);
  fprintf(stdout,"Number of incorrect postcodes     = %ld\n",cnt3);

  tim = time(NULL);
  fprintf(stderr,"Program Ends OK  %s\n",ctime(&tim));

  return 1;

 }

 /***************************   read1  ******************************/

 read1()
 {
  int reply;

  reply = fread(buf,reclen,1,inp);
  if (feof(inp) != 0)
  {
   fprintf(stderr,"End of file %s\n",input);
   return 0;
  }
  cnt1++;

  if (reply != 1)
  {
   fprintf(stderr,"Error reading %s\n",input);
   exit(1);
  }
  return 1;
 }


  /**************************  findspace  ******************************/

  findspace()
  {
   for(spacepos=2;spacepos<5;spacepos++)
   {
     if(isspace(p1[spacepos]) > 0) 
     {
       error = 1;
       return;
     }
   }
  error = 0;
  return;
  }
  /***************************  chkpost *******************************/

  chkpost()
  {
   int i;
fprintf(stdout,"in chkpost postcode = %s\n",p1);
   if (isalpha(p1[0]) == 0)
   {
      crap();
      return 0;
   }
   if (isalnum(p1[1]) == 0)
   {
      crap();
      return 0;
   }
   for(i=0;i<3;i++)
   {
     if(p1[1] == second[i])
     {
	crap();
	return 0;
     }
   }                                  /*****************************/
   if (spacepos == 2)                 /* POSTCODE IN FORMAT A1 1AA */
   {                                  /*****************************/
      if (isdigit(p1[3]) == 0)
      {
	 crap();
	 return 0;
      }
      if (isalpha(p1[4]) == 0)
      {
	 crap();
	 return 0;
      }
      if (isalpha(p1[5]) == 0)
      {
	 crap();
	 return 0;
      }
      testinw(4);                     /*  RANGE CHECK ON INWARD CODE */
      if (error == 0)
      {
	 return 0;
      }
   }
				      /******************************/
   if (spacepos == 3)                 /* POSTCODE IN FORMAT A11 1AA */
   {                                  /* OR AA1 1AA OR A1A 1AA      */
      if (isalnum(p1[2]) == 0)        /******************************/
      {
      crap();
      return 0;
      }

      test3();                        /* CHECK THAT 3rd CHAR IN RANGE */
      if (error == 0)
      {
	 return 0;
      }

      if (isdigit(p1[4]) == 0)
      {
	 crap();
	 return 0;
      }

      if (isalpha(p1[5]) == 0)
      {
	 crap();
	 return 0;
      }
      if (isalpha(p1[6]) == 0)
      {
	 crap();
	 return 0;
      }

      testinw(5);                     /* RANGE CHECK ON INWARD CODE */
      if (error == 0)
      {
	 return 0;
      }
   }
				      /*******************************/
   if (spacepos == 4)                 /* POSTCODE IN FORMAT AA11 1AA */
   {                                  /* OR AA1A 1AA                 */
      if (isalnum(p1[2]) == 0)        /*******************************/
      {
      crap();
      return 0;
      }

      test3();                        /* CHECK THAT 3rd CHAR IN RANGE */
      if (error == 0)
      {
	 return 0;
      }

      if (isalnum(p1[3]) == 0)
      {
	 crap();
	 return 0;
      }

      test4();                        /* CHECK THAT 4th CHAR IN RANGE */
      if (error == 0)
      {
	 return 0;
      }

      if (isdigit(p1[5]) == 0)
      {
	 crap();
	 return 0;
      }
      if (isalpha(p1[6]) == 0)
      {
	 crap();
	 return 0;
      }
      if (isalpha(p1[7]) == 0)
      {
	 crap();
	 return 0;
      }
      testinw(6);                     /* RANGE CHECK ON INWARD CODE */
      if (error == 0)
      {
	 return 0;
      }

   }
      writ();
      return 1;
  }

  /****************************  test3  *******************************/

  test3()
  {
   int i;

   for(i=0;i<15;i++)
   {
	if(p1[2] == third[i])
	{
	  crap();
	  error = 0;
	  return 0;
	}
   }
   error = 1;
   return 1;
  }

  /****************************  test4  *******************************/

  test4()
  {
   int i;

   for(i=0;i<14;i++)
   {
	if(p1[3] == fourth[i])
	{
	  crap();
	  error = 0;
	  return 0;
	}
   }
   error = 1;
   return 1;

  }

  /****************************  testinw  ****************************/

  testinw(num)
  int num;
  {
  int i;
  int num2 = num;
  num2++;
  for(i=0;i<6;i++)
      {
	if(p1[num] == cikmov[i] || p1[num2] == cikmov[i])
	{
	  crap();
	  error = 0;
	  return 0;
	}
      }
  error = 1;
  return 1;
  }


  /***************************   writ  ******************************/

  writ()
  {
   int reply;
   reply = fwrite(buf,reclen,1,out);
   if (reply != 1)
   {
    fprintf(stderr,"Error writing to file postcodesok\n");
    exit(1);
   }
   cnt2++;
   return 1;
  }


  /****************************  crap  *******************************/

  crap()
  {
   int reply;
   reply = fwrite(buf,reclen,1,err);
   if (reply != 1)
   {
    fprintf(stderr,"Error writing to file pcodeserrs\n");
    exit(1);
   }
   cnt3++;
   return 1;
  }
