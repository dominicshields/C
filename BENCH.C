#include <stdio.h>
#include <time.h>
#include <dos.h>

#define REPS 100000000
int mins, secs, hmins, hsecs; 
long i, x, domint, domfloat;
float fsecs, fhsecs, milli, hmilli; 
double d;
struct time t;

main()
{
gettime(&t);
hmins = t.ti_min;
hsecs = t.ti_sec;
hmilli = t.ti_hund;
fhsecs = hsecs + (hmilli/100);
printf("Integer addition test begins:\t\t%2d:%02d:%02d.%02d\n",t.ti_hour, t.ti_min, t.ti_sec, t.ti_hund);
while (i < REPS)
{
	i++;
	x = 57757747 + 577773453;
}
gettime(&t);
printf("100 Million integer iterations ends:\t%2d:%02d:%02d.%02d\n",t.ti_hour, t.ti_min, t.ti_sec, t.ti_hund);
mins = t.ti_min;
secs = t.ti_sec;
milli = t.ti_hund;
if(mins > hmins)
{
	secs += (60* (mins-hmins));
}
fsecs = secs + (milli/100);
printf("Time elapsed = \t%f secs \n\n",fsecs-fhsecs);
domint = REPS/(fsecs-fhsecs);

/******************************** INTEGER TEST ENDS **************************/

gettime(&t);
hmins = t.ti_min;
hsecs = t.ti_sec;
hmilli = t.ti_hund;
fhsecs = hsecs + (hmilli/100);
printf("Floating Point test begins:\t\t%2d:%02d:%02d.%02d\n",t.ti_hour, t.ti_min, t.ti_sec, t.ti_hund);
i=0;
while (i < REPS)
{
	i++;
	d = 555653544.2456/3254554.564564;
}
gettime(&t);
mins = t.ti_min;
secs = t.ti_sec;
milli = t.ti_hund;
if(mins > hmins)
{
	secs += (60* (mins-hmins));
}
fsecs = secs + (milli/100);
printf("100 Million float operations ends:\t%2d:%02d:%02d.%02d\n",t.ti_hour, t.ti_min, t.ti_sec, t.ti_hund);
printf("Time elapsed = \t%f secs \n\n",fsecs-fhsecs);
domfloat = REPS/(fsecs-fhsecs);

printf("\n************************************************************************\n");
printf("\nYour Domintmark is \t %ld Integer Operations Per Second\n",domint);
printf("\nYour Domfloatmark is \t %ld Floating Point Operations Per Second\n",domfloat);
printf("\n************************************************************************\n");
printf("\nFor comparison an AMD K6-200 gives values of\n");
printf("Domintmark\t24.94 Million\n");
printf("Domfloatmark\t8.35 Million\n");
return 0;
}