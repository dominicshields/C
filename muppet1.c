/**************************************************************************
PROGRAM NAME : muppet1
PURPOSE      : reformats the Census enumerator personnel file to be half useful
PARAMETERS   : inputfile outputfile
LAYOUT OF INPUT FILE : Bonkers Frankly
"Payee Number","00AA0101  "
""," "
"Staff Grade","05    "
""," "
"Date Appointed",20032001
""," "
"Surname","SOREMEKUN                       "
""," "
"First Name","Other Initials","FEUSAT              ","T      "
""," "
"Title","MISS"
""," "
"Date of Birth",26121975
""," "
"Sort Code","Account Number","60-30-09","67038956  "
""," "
"Additional Account"," "
""," "
"Home Address"," "
"",""," ","38 ANGRAVE COURT            "
"",""," ","SCRIVEN STREET              "
"",""," "," LONDON                     "
"",""," ","                            "
"",""," ","E8   4HY                    "
""," "
"Supplies Address"," "
"",""," "," "
"",""," "," "
"",""," "," "
"",""," "," "
"",""," "," "
""," "
"Home Phone","Work Phone","                    ","                    "
""," "
"Tax Code","NI Code","National Insurance No","BR     ","A","JE165785B"
""," "
"Car/Van Indicator","Motor Cycle"," "," "
""," "
"EW Nos.","ED Nos."," "," "
"","","","01"," ","FA01"
"","","","  "," ","    "
"","","","  "," ","    "
"","","","  "," ","    "
"Previous Employment Indicator","N"

LAYOUT OF OUTPUT FILE : (In one record - doh !)
Payee Number,Staff Grade,Date Appointed,Surname,First Name,Initials,Title,
Date of Birth,Sort Code,Account Number,Additional Account,Home Address1,Home Address2,
Home Address3,Home Address4,Postcode,Home Phone,Work Phone,Tax Code,NI Code,
National Insurance No,Car/Van Indicator,Motor Cycle,EW Nos,ED Nos,
Previous Employment Indicator
**************************************************************************/
#include <string.h>
#include <stdio.h>
#include <ctype.h>
		      /* file pointers */
FILE *inp;            /* input file    */
FILE *out;            /* output file   */

char *input;
char *output;

char *p1;
char *p2;

int posctr,i,firstrec,len,address,ewnos;
long readcnt=0;
long writecnt=0;
char buf[1000];       /* buffer area to read input file */
char hold[1000];
char ewhold[20];
char edhold[20];


main(argc,argv)
int argc;
char *argv[];
{
   if(argc < 2)
   {
     fprintf(stderr,"Error, usage is muppet1 <inputfile> <outputfile> \n");
      exit(1);
   }

   input = argv[1];            /* input filename assigned to pointer */
   output = argv[2];           /* output filename assigned to pointer */

   if ((inp = fopen(input,"r")) == NULL)
   {
      fprintf(stderr,"Error opening %s does it exist?\n",input);
      exit(1);
   }

   if ((out = fopen(output,"w")) == NULL)
   {
      fprintf(stderr,"Error opening %s\n",output);
      exit(1);
   }

   read1();
   posctr = 0;
   firstrec = 0;
   address = 0;
   ewnos = 0;
 
   while ( feof(inp) == 0 ) 
   {
/* "Payee Number","00AA0101  " */   	
	if(strncmp(p1,"\"Payee Number",13) == 0)
	{
		address = 0;
		for(i=0;i<1000;i++)
		{
			hold[i]='\0';
		}
		for(i=0;i<20;i++)
		{
			ewhold[i]='\0';
			edhold[i]='\0';
		}
		p1 += 16;
		strncpy(hold,p1,8);
		strcat(hold,",");
		posctr = 1;
		goto lab;
   	}
/* "Staff Grade","05    " */   	
   	if(strncmp(p1,"\"Staff Grade",12) == 0)
	{
		p1 += 15;
		strncat(hold,p1,2);
		strcat(hold,",");
		posctr = 2;
		goto lab;
   	}
/* "Date Appointed",20032001 */ 	
   	if(strncmp(p1,"\"Date Appointed",15) == 0)
	{
		p1 += 17;
		strncat(hold,p1,2);
		strcat(hold,"/");
		p1 += 2;
		strncat(hold,p1,2);
		strcat(hold,"/");
		p1 += 2;
		strncat(hold,p1,4);
		strcat(hold,",");
		posctr = 3;
		goto lab;
   	}
/* "Surname","SOREMEKUN  */   	
	if(strncmp(p1,"\"Surname",8) == 0)
	{
		p1 += 11;
/*		len = strspn(p1,"   ");  doesn't work, nor does strcspn */
		strncat(hold,p1,30);
		strcat(hold,",");
		posctr = 4;
		goto lab;
   	}   	
/* "First Name","Other Initials","FEUSAT              ","T      " */   	
   	if(strncmp(p1,"\"First Name",11) == 0)
	{
		p1 += 31;
		strncat(hold,p1,20);
		strcat(hold,",");
		p1 += 23;
		strncat(hold,p1,7);
		strcat(hold,",");
		posctr = 5;
		goto lab;
   	}
/* "Title","MISS" */   	
   	if(strncmp(p1,"\"Title",6) == 0)
	{
		p1 += 9;
		len = strcspn(p1,"\"");
		strncat(hold,p1,len);
		strcat(hold,",");
		posctr = 6;
		goto lab;
   	}
/* "Date of Birth",26121975 */     	
   	if(strncmp(p1,"\"Date of Birth",14) == 0)
	{
		p1 += 16;
		strncat(hold,p1,2);
		strcat(hold,"/");
		p1 += 2;
		strncat(hold,p1,2);
		strcat(hold,"/");
		p1 += 2;
		strncat(hold,p1,4);
		strcat(hold,",");
		posctr = 7;
		goto lab;
   	}
/* "Sort Code","Account Number","60-30-09","67038956  " */
   	if(strncmp(p1,"\"Sort Code",10) == 0)
	{
		p1 += 30;
		strncat(hold,p1,8);
		strcat(hold,",");
		p1 += 11;
		strncat(hold,p1,10);
		strcat(hold,",");
		posctr = 8;
		goto lab;
   	}
/* "Additional Account"," "*/
   	if(strncmp(p1,"\"Additional Account",19) == 0)
	{
		p1 += 22;
		strncat(hold,p1,strlen(p1)-2);
		strcat(hold,",");
		posctr = 77;
		goto lab;
   	}
/* "Home Address */   	
   	if(strncmp(p1,"\"Home Address",13) == 0)
   	{
   		posctr = 9;
   		address = 1;
   		goto lab;
   	}
/* "",""," ","38 ANGRAVE COURT            "
   "",""," ","SCRIVEN STREET              "
   "",""," "," LONDON                     "
   "",""," ","                            "
   "",""," ","E8   4HY                    " */
   	if(address > 0)
   	{
   		p1 += 11;
		strncat(hold,p1,28);
		strcat(hold,",");
		posctr++;
		address++;
		if(address > 5)
		{
			address = 0;
		}
		goto lab;
	}	
/* "Home Phone","Work Phone","                    ","                    "   	*/
	if(strncmp(p1,"\"Home Phone",11) == 0)
	{
		p1 += 27;
		len = strcspn(p1,"   ");
		strncat(hold,p1,len);
		strcat(hold,",");
		p1 += 23;
		len = strcspn(p1,"   ");
		strncat(hold,p1,len);
		strcat(hold,",");
		posctr = 15;
		goto lab;
   	}   	
/* "Tax Code","NI Code","National Insurance No","BR     ","A","JE165785B" */
	if(strncmp(p1,"\"Tax Code",9) == 0)
	{
		p1 += 46;
		len = strcspn(p1," ");
		strncat(hold,p1,len);
		strcat(hold,",");
		p1 += 10;
		strncat(hold,p1,1);
		strcat(hold,",");
		p1 += 4;
		strncat(hold,p1,9);
		strcat(hold,",");
		posctr = 16;
		goto lab;
   	}   
/* "EW Nos.","ED Nos."," "," "
   "","","","01"," ","FA01"
   "","","","  "," ","    "
   "","","","  "," ","    "
   "","","","  "," ","    "   	 */
	if(strncmp(p1,"\"EW Nos.",8) == 0)
	{
		posctr = 17;
   		ewnos = 1;
   		goto lab;
   	}   
   	if(ewnos > 0 && ewnos < 4)
   	{
   		p1 += 10;
   		strncat(ewhold,p1,2);
		strcat(ewhold,";");
		p1 += 9;
   		strncat(edhold,p1,4);
		strcat(edhold,";");
		posctr++;
		ewnos++;
		goto lab;
	}
	if(ewnos == 4)
	{
		p1 += 10;
   		strncat(ewhold,p1,2);
		p1 += 9;
   		strncat(edhold,p1,4);
		posctr++;
		ewnos=0;
		strcat(hold,ewhold);
		strcat(hold,",");
		strcat(hold,edhold);
		strcat(hold,",");
		goto lab;
	}
/* "Previous Employment Indicator","N"	*/
	if(strncmp(p1,"\"Previous Employment Indicator",30) == 0)
	{
		p1 += 33;
		strncat(hold,p1,1);
		writerec();
   	}
   	
    lab:
    read1();
   }
   
  fprintf(stdout,"\nRecords Read  = %ld\n",readcnt);
  fprintf(stdout,"Records Written = %ld\n",writecnt);
  fclose(inp);
  fclose(out);

   return 0;

}

/*---------------------------- READ1 ----------------------------*/

read1()
{
  fgets(buf,1000,inp);
  p1=buf;
  readcnt++;
  return 0;	
}

/*-------------------------- WRITEREC ----------------------------*/

writerec()
{
	fprintf(out,"%s\n",hold);
	writecnt++;
	return 0;
}