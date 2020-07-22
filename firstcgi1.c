/*-------------------------------------------------------------------------------
Program Name   : firstcgi.c
Program Author : Dominic Shields
Date           : 31/12/1999  
Purpose        : CGI program for internet assignment 2 to take name and age from
                 a page and display it back.
-------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

main()
{
	char name[80];
	int age;

	fscanf(stdin,"%s%d",name,&age);

	printf("%s\n\n","Content-Type: text/html");
	printf("<html>\n");
    	printf("<head><title>Output from CGI Script firstcgi</title></head>\n");
	printf("<body>\n");
	printf("<B> Your name is %s and you are %d years old.</B>\n",name,age);
	printf("</body>\n");
    	printf("</html>\n");
	return 0;
}
