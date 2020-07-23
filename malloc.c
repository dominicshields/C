#include "stdio.h"
#include "stdlib.h"

int waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
    return 0;
}

main(void)
{
char *p;
long l;

l = 0;

do
{
	p = malloc(1000);
	if(p) l += 1000;
}
while(p);

printf("Approx %ld bytes of free memory\n",l);

waitFor(10);
return 0;
}