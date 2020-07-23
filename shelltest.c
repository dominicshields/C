#include <stdio.h>

int main(void)
{
	int array[5];		// An array of integers.
	int length = 5;		// Length of the array.
	int i, d, x;
	int tmp;

	//Some input
	for (i = 0; i < length; i++)
	{
		printf ("Enter a number between 1 and 10000: ");
  	scanf ("%d",&array[i]);
	}

	//Algorithm
	d = length;
	printf("Initial d = %d\n",d);
	do {
		d = (d + 1)/2;
		printf("d = %d\n",d);
		for (i =0; i < (length - d); i++)
		{
/*			if (array[i + d] > array[i])*/
			if (array[i + d] < array[i])
			{
				tmp = array[i+d];
				array[i + d] = array[i];
				array[i] = tmp;
			}
		}
	} while(d > 1);
	
	//Some output
	for (i = 0; i < 5; i++)
	{
		printf("%d\n",array[i]);
	}
	
	return 0;
}