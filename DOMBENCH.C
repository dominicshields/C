#include <stdio.h>
#include <time.h>

long i;
time_t first, second;

main()
{

	first = time(NULL);
	fprintf(stderr,"\nProgram Begins  %s\n",ctime(&first));

	while (i < 10000000)
	{
		i++;
	}

	second = time(NULL);
	fprintf(stderr,"\nProgram Ends  %s\n",ctime(&second));
	printf("The difference is: %f seconds\n",difftime(second,first));

	return 0;
}

