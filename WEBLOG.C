/**********************************
 weblog.c 
reformats the access logs so that zmerge can do the bizz 
**********************************/
#include <stdio.h>
#include <time.h>
#include <dos.h>

FILE *inp;
FILE *outp;

char *p1;
char *input;
char *output;

char buf[500];
char site[3];
char ip[16];
char fulldate[25];
char dateon[15];
char timeon[9];
char getpost[5];
char command[200];
char unknown[15];
char daychar[3];
char mon[4];
char yearchar[5];
char filename[20];
struct date d;

int cnt1,cnt2,len,p1cnt;

main(argc,argv)
int argc;
char *argv[];
{
	if(argc != 1)
	{
		fprintf(stderr,"Error, usage is weblog with no params\n");
		exit(1);
	}

	getdate(&d);
   		
	switch (d.da_mon)
	{
		case 1 : strcpy(mon,"Jan");break;
		case 2 : strcpy(mon,"Feb");break;
		case 3 : strcpy(mon,"Mar");break;
		case 4 : strcpy(mon,"Apr");break;
		case 5 : strcpy(mon,"May");break;
		case 6 : strcpy(mon,"Jun");break;
		case 7 : strcpy(mon,"Jul");break;
		case 8 : strcpy(mon,"Aug");break;
		case 9 : strcpy(mon,"Sep");break;
		case 10 : strcpy(mon,"Oct");break;
		case 11 : strcpy(mon,"Nov");break;
		case 12 : strcpy(mon,"Dec");break;
	}
	sprintf(daychar,"%02d",d.da_day);
	sprintf(yearchar,"%d",d.da_year);
	p1=yearchar;
	p1+=2;
	strcpy(yearchar,p1);
	
/* change the sprintf to make a filename in the format
   access.log.MonDDYY - this will only work in NT
*/
	sprintf(filename,"%s%s%s",mon,daychar,yearchar);
	fprintf(stdout,"%s\n",filename);
	
	input = filename;

	if ((inp = fopen(input,"r")) == NULL)
	{
		fprintf(stderr,"Error opening %s - does it exist?\n",input);
		fprintf(stderr,"Program exits\n",input);		
		exit(1);
	}

	if ((outp = fopen("webfile","w")) == NULL)
	{
		fprintf(stderr,"Error creating webfile\n",output);
		exit(1);
	}

	read1();

	while (feof(inp) == 0)
	{
		p1cnt=0;
		len=strcspn(p1,".");		
		strncpy(site,p1,len);
		site[len]='\0';
		len=strcspn(p1," ");		
		strncpy(ip,p1,len);
		ip[len]='\0';
		while(strncmp(p1,"[",1) !=0)
		{
			p1++;
		}
		p1++;
		len=strcspn(p1," ");		
		strncpy(fulldate,p1,len);
		fulldate[len]='\0';
		len=strcspn(p1,":");		
		strncpy(dateon,p1,len);
		dateon[len]='\0';
		p1+=len;
		p1++;
		len=strcspn(p1," ");		
		strncpy(timeon,p1,len);
		timeon[len]='\0';
		while(strncmp(p1,"\"",1) !=0)
		{
			p1++;
		}
		p1++;
		len=strcspn(p1," ");		
		strncpy(getpost,p1,len);
		getpost[len]='\0';
		p1+=len;
		p1++;
		len=strcspn(p1,"\"");		
		strncpy(command,p1,len);
		command[len]='\0';
		p1+=len;
		p1++;
		strcpy(unknown,p1);

		fprintf(outp,"%s,%s,%s,%s,%s,%s,%s,%s",site,ip,fulldate,dateon,timeon,getpost,command,unknown);
		cnt2++;
		read1();
	}

	fclose(inp);
	fclose(outp);
	fprintf(stdout,"Records Read From %s = %d\n",filename,cnt1);
	fprintf(stdout,"Records Written To webfile = %d\n",cnt2);
	return 0;

}

 /***************************   read1  ******************************/

read1()
{
	fgets(buf,500,inp);
	if (feof(inp) != 0)
	{
		fprintf(stderr,"End of file %s\n",input);
		return 0;
	}
	cnt1++;
	p1 = buf;
	return 0;
}
