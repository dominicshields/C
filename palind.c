/*-------------------------------------------------------------------------------
Program Name   : palind.c
Program Author : Dominic Shields
Date           : 21/10/1999  
Purpose        : Fourth C Programming exercise to determine whether an input
                 string is a palindrome
-------------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

main()
{
	int i=0;
	int j=0;
	char buf[80];
	char bufcaps[80];
	char reverse[11];
	char reversecaps[11];
	char c;
	
	printf("Please enter string>");
	while((c = getchar()) != '\n')
	{
		if(!isalpha(c))
		{
			printf("Invalid Input\n");
			exit(1);
		}
		buf[i] = c;
		bufcaps[i] = toupper(c);
		i++;
	}

	buf[i]='\0';
	bufcaps[i]='\0';

	if(i > 10)
	{
			printf("String too long\n");
			exit(1);
	}

	for(j=0,i--;j<strlen(buf);j++,i--)
	{
		reverse[j]=buf[i];
		reversecaps[j]=bufcaps[i];
	}
	reverse[j] = '\0';
	reversecaps[j] = '\0';

	if(strcmp(buf,reverse) == 0)
	{
		printf("String is a palindrome\n");
	}
	else
	{
			if(strcmp(bufcaps,reversecaps) == 0)
			{
				printf("String is a case-insensitive palindrome\n");
			}
			else
			{
				printf("String is not a palindrome\n");
			}
	}
	
	return(0);
}


