#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *outfil_ptr;
long rows;
long tot;

main(argc,argv)
int argc;
char *argv[];
{
	tot = atol(argv[1]);
	fprintf(stdout,"Creating %ld rows\n",tot);
	rows=1;
	outfil_ptr = fopen("rubbish.txt","w");
	while(rows < tot) 
	{
		fprintf(outfil_ptr,"Random data Random data Random data Random data Random data!\n");
		rows++;
	}
	fprintf(stdout,"Finished on row = %ld\n",rows);
	fclose(outfil_ptr);
	return(0);
}
