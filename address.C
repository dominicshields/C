/**********************************************
 address.c					
 knocks up pseudo-random address		
 Dominic June 2001                             
***********************************************/
#include <stdio.h>  
#include <stdlib.h>
#include <time.h>

FILE *out; 

long reqd, cnt;
int ran[5];
int i;

char line1[10][20] = {"Mr A Jones","Mrs B Smith","Mr A Johnson","Mrs G Evans","Mr A Edwards","Miss S Williams","Mr H Thomas","Miss M Bloggs","Mrs J Davies","Mr H Jardon"};
char line2[10][30] = {"1 High St","23 Acacia Avenue","31 Queen St","85 Happy Eaters","Dunprogramming","The Hollies C/O PO Box 31","5670 Main Road","6 Sleepy Villas","851 Newport Rd","23 Park Gardens"};
char line3[10][30] = {"Adamsdown","Penylan","Splott","Roath","Cathays","Cyncoed","Tremorfa","Llanrumbo","St Mellons","Darkest Ely"};
char line4[10][30] = {"Cardiff","Pontysheepy","Cwmconcrete","Newpoor","Aberflyarff","Llareggub","Picturesque Barry","Aberbanana","Abersignon","Merthyr Tydfil"};
char line5[10][10] = {"CF1 1AA","CF4 5FD","NP44 1ED","NP87 2WS","SA21 4RF","CF32 4NJ","NP10 8XG","CF5 8FX","CF7 9HB","CF63 5FA"};

main(argc,argv)
int argc;
char *argv[];
{

if(argc < 2)
{
	fprintf(stdout,"usage is address <no required>\n");
	exit(1);
}

if ((out = fopen("address.txt","w")) == NULL)
{
	fprintf(stderr,"Smoke me a kipper, I'll be back for breakfast - cannot create output file for some reason\n");
	exit(1);
}

reqd=0;
cnt=0;
reqd = atol(argv[1]);

while(cnt < reqd)
{

for(i=0;i<5;i++)
{
  ran[i]= ir_random(9);
}

fprintf(out,"%s,%s,%s,%s,%s\n",line1[ran[0]],line2[ran[1]],line3[ran[2]],line4[ran[3]],line5[ran[4]]);
 
cnt++;

}

printf("%ld address records written to address.txt, cushtie\n",cnt);
fclose(out);
return(0);

}

ir_random(num)
int num;
{
long random, ltime;
static long sub = 0;
static int flag = 0;
static long numstore[10000];

if(flag == 0)       
{
 flag++;
 time(&ltime);                  
 srand(ltime);                
 for(sub=0;sub<10000;sub++)
 {
  numstore[sub] = (rand());  
 }
 sub=0;
}

if(sub > 9998)
{
 flag=0;           
}

if (num < 1)                  
{
 random = (numstore[sub]);
}
else
{
 random = (((numstore[sub]) % num) + 1);
}
sub++;
return(random);

}

