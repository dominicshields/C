#include <stdio.h>

void main()
{
	char a;
	char b;
	char c;
	int control=0;
	int z=0;

	a= 85;

	if(a & 128)
	{
		printf("Bit 7 set\n");
	}

	if(a & 64)
	{
		printf("Bit 6 set\n");
	}

	if(a & 32)
	{
		printf("Bit 5 set\n");
	}

	b = a & 63;

	printf("b = %d\n",b);
	
	c = a | 10;

	printf("c = %d\n",c);

	control = z | 17;
	printf("control = %d\n",control);
	
	control = z & 17;

	control = z | 1088;
	printf("control = %d\n",control);

	control = control & 1088;
	if (control & 1024)
	{
		printf("Bit 10 set\n");
	}

	if (control & 64)
	{
		printf("Bit 6 set\n");
	}

	printf("control = %d\n",control);

}
