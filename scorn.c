#include <stdio.h>
#define harsh_but_fair  TRUE

main(argc,argv)
int argc;
char *argv[];
{

	int ret;
	printf("What's the next number in the sequence . . . 2 4 6 8 10 12\n");

	scanf("%d", &ret);
	
	if(ret == 14)
	{
		printf("Congratulations, you are an IS diva, climb aboard\n");
	}
	else
	{
		printf("Hmm . . . Have you considered a job in management ?\n");
	}

}