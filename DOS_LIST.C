/*********************************************************************
*                                                                    * 
*  Program    :  cp_list.c                                           * 
*                                                                    * 
*  Description: prints a numbered program listing                    * 
*                                                                    * 
*  Parameters : i) name of program file                              * 
*                                                                    * 
*              ii) name of printer (optional - default is d235_h3b4) * 
*                                                                    * 
*  Programmer : rjd                                                  * 
*                                                                    * 
*  Date       : Oct 1993                                             * 
*                                                                    * 
*********************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

FILE *fopen();

main (argc,argv)

int argc;
char ** argv;

{
   char buff [81];       /* buffer for input text              */
   char buff1 [81];      /* buffer for revised input line      */
   char outp [130];      /* buffer for output text             */
   char u_print [250];   /* unix print command                 */
   char font1 [30];      /* font for title                     */
   char font2 [30];      /* font for main listing              */
   char s_line [30];     /* shaded line                        */
   char obj_type[13];    /* tells what type the listing is     */
   char b_char;          /* current character of input line    */
   char hname[80];	 /* holds full hierarchic filename     */

   char *t_count;        /* pointer to check for / in argv[1]  */
   char *f_name;         /* pointer to file name without path  */
   char *newname;        /* pointer to file name without path  */
   char *print_id;       /* printer id                         */
   char *pwd;		 /* pointer to ENV pathname	       */
    
   short l_count;        /* program line number                */
   short p_count;        /* page number                        */
   short pl_count;       /* counter for lines on current page  */
   short pl_max;         /* max lines per page                 */ 
   short ret_val;        /* return value from system call      */
   short last_long;      /* flag if last line over 80 chars    */
   short no_outp;        /* flag used to suppress output line  */ 
   short pg_throw;       /* flag if last line had ^L           */

   unsigned int s_len;   /* length of input string             */ 
   unsigned int b_val;   /* ascii value for current character  */       
   unsigned int b_count; /* counter for inspecting input line  */
   unsigned int i;       /* for loop counter                   */

   time_t p_time;        /* variable for system time           */

   FILE *fp1;            /* input file pointer                 */
   FILE *fp2;            /* output file pointer                */

   /* set max lines per page and printer fonts */

   pl_max=58;
   strcpy(font1, "\033(10U\033(s1p15vsb3T");
   strcpy(font2, "\033(10U\033(sp12h10vsb3T");
   strcpy(s_line, "\033*c2250a20b15g2P\033=\n");

   /* check parameter supplied */

   if(argv[1]==NULL)
   {
      printf("ERROR no file specified\n");
      printf("Usage: cp_list file_name [printer_name]\n");
      exit(1);
   }

   /* get default printer name if one is set */

/*   print_id=getenv("LPDEST"); */
   
   /* open input file - abandon if failure */

   fp1=fopen(argv[1], "r");
   if (fp1==0)
   {
      printf("ERROR - Could not find input file: %s\n", argv[1]); 
      exit(2);
   }

   /* open output file - abandon if failure */

   fp2=fopen("cp_list.lst", "w");
   if (fp2==0)
   {
      printf("ERROR - Could not create listing file: cp_list.lst\n"); 
      fclose(fp1);
      exit(3);
   }

   /* get name of file without path */

   f_name=argv[1]; 
   while ((t_count=strstr(f_name,"/"))!=NULL)
   {
      f_name=++t_count;
   }
   newname = f_name;
   strcpy(obj_type,"file");
   for(i=1;i<=strlen(f_name);i++)
   {
     if( strcmp(newname,".c") == 0)
     {
	strcpy(obj_type,"C program");
     }
     if( strcmp(newname,".sc") == 0)
     {
   	strcpy(obj_type,"Esql program");
     }
     if( strcmp(newname,".osq") == 0)
     {
        strcpy(obj_type,"Abf program");
     }
     if( strcmp(newname,".rw") == 0)
     {
        strcpy(obj_type,"Reportwriter");
     }

     newname++;
   }

   if(argv[2]==NULL)
   {
      if (print_id==NULL)
      {
         printf("Listing %s: %s to printer: default\n", obj_type, f_name); 
      }
      else
      {
         printf("Listing %s: %s to printer: %s\n", obj_type, f_name, print_id); 
      }
   }
   else
   {
      printf("Listing %s: %s to printer: %s\n", obj_type, f_name, argv[2]); 
   }

   /* get system time */

   p_time=time(NULL);

   /* initialise printer \ write header of page one */

   sprintf(outp, 
   "\033E%sListing of %s: \033(s3B%s%s\015\033(sB%88s\015",
   font1, obj_type, f_name, font2, ctime(&p_time));
   fputs(outp,fp2);
   fputs(s_line,fp2);

   /* initialise page\line counts, page number and flags */

   p_count=1;
   l_count=1;
   pl_count=0;
   last_long=0;
   pg_throw=0;

   /* 'read ahead' of text file */

   fgets(buff,81,fp1);

   /* loop to read up to end of text file or an error occurs */

   while (feof(fp1)==0 && ferror(fp1)==0 && ferror(fp2)==0)
   {
       /* set flag for output */

       no_outp=0;

       /* position to bottom of page if last line included ^L */
       
       if(pg_throw==1)
       {
	  pg_throw=0;

          while (pl_count<pl_max)
          {
              pl_count++;
              fputs("\n",fp2);
          }
       }

       /* search and replace control characters in input line */
       /* initialise string to hold revised line/char count   */

       strcpy(buff1, "\0");
       s_len=strlen(buff);
       b_count=0;

       while (b_count<s_len)
       {
	  /* get ascii value of character */

	  b_val=(unsigned int)buff[b_count];

	  /* check if a control character other than newline */

	  if (b_val<32&&b_val!=10)
          {
             /* replace tab characters with spaces */

             if(b_val==9)
             {
	        strcat(buff1, "        ");
             }
             else
	     {
	        /* calculate character for display as ^char */

	        b_val=b_val+64;
	        b_char=(char)b_val;

                /* append ^char to revised line */
             
	        sprintf(buff1,"%s^%c\0", buff1, b_char);

                /* flag page throw characters (^L) */

                if (b_val==76)
                {
                   pg_throw=1;
                }
              }
           }
           else
	   {
	     /* append printable character to revised line */

	     sprintf(buff1, "%s%c", buff1, buff[b_count]);
           }
	   b_count++;
        }

       /* check if end of page */

       if (pl_count==pl_max)
       { 
	  /* write page footer and page throw */

          fputs(s_line,fp2);
	  sprintf(outp, "%80s Page: %d\n\014", " ", p_count);
          fputs(outp,fp2);

	  /* write header of next page */

          sprintf(outp, 
          "%sListing of %s: \033(s3B%s\033(sB (cont.)%s\n", 
	  font1, obj_type, f_name, font2);
          fputs(outp,fp2);
          fputs(s_line,fp2);

	  /* reinitialise page line count/update page number */

          pl_count=0;
          p_count++;
       }

       /* check if current line is a long line ie. no newline */
       /* select output depending on type of line */

       if ((strstr(buff1,"\n"))!=NULL)
       {
	  if (last_long==1)
	  {
	     last_long=0;

             if (strcmp(buff1,"\n")==0)
	     {
		/* input is newline character only at end of long line */
		/* set flag to suppress output line                    */

		no_outp=1;
	     }
	     else
	     {
	        /* last line of long line - no line number */

                sprintf(outp,"%5s: %s", " ", buff1);
	     }
	  }
	  else
	  {
	    /* normal short line  - line number */

             sprintf(outp,"%5d: %s",l_count,buff1);
	  }

	  /* increment program line count */

          l_count++;
       }
       else
       {
	  if (last_long==0)
	  {
	     /* first line of long line - line number */

             sprintf(outp,"%5d: %s\n",l_count ,buff1);
	     last_long=1;
	  }
	  else
	  {
	     /* middle line of long line  - no line number */

             sprintf(outp,"%5s: %s\n"," ",buff1);
	  }
       }

       /* write output line \increment page line count unless */
       /* newline only at end of line over 80 characters      */
       
       if(no_outp==0)
       {
          fputs(outp,fp2);
          pl_count++;
       }

       /* get next line from input file */

       fgets(buff,81,fp1);
   }

   /* check if an error in reading input file */

   if (ferror(fp1)!=0)
   {
      fclose(fp1);
      fclose(fp2);
      printf("ERROR - failure in reading input file: %s\n", argv[1]); 
      exit(4);
   }
   
   /* check if an error in writing to listing file */

   if (ferror(fp2)!=0)
   {
      fclose(fp1);
      fclose(fp2);
      printf("ERROR - failure in writing to listing file: cp_list.lst"); 
      exit(5);
   }

   /* write end of listing message depending on space left */

   pwd=getenv("PWD");     
   strcpy(hname,pwd);       /*  I KNOW THIS IS CRAP CODING            */ 
   strcat(hname,"/");       /*  BUT WOULD IT WORK IN A SENSIBLE WAY ? */
   strcat(hname,f_name);    /*  NO !    DJS  13/12/93                 */

   if (pl_count<(pl_max-2))
   {
      pl_count+=2;
      fprintf(fp2,"\nEnd of listing of %s",hname);
      sprintf(outp, "%-75s Page: %d\033E", " ", p_count);      
   }
   else
   {
     sprintf(outp, "%18s %-62s Page: %d\033E", "End of listing of ",hname,p_count); 
   }

   /* position to bottom of page */

   while (pl_count<pl_max)
   {
       pl_count++;
       fputs("\n",fp2);
   }

   /* write page footer and printer reset */
 
   fputs(s_line,fp2);
   fputs(outp,fp2);

   /* close files */

   fclose(fp1);
   fclose(fp2);

   /* create unix print command */
/*
   if (argv[2]==NULL)
   {
      strcpy(u_print, "lp -c -onobanner cp_list.lst");
   }
   else
   {
      sprintf(u_print, "lp -c -onobanner -d%s cp_list.lst", argv[2]);
   }

   ret_val=system(u_print); 

   if (ret_val!=0)
   {
      printf("ERROR - failure in printing listing file\n");
   }
   
   remove("cp_list.lst"); */  
}


