#include <stdio.h>

void main()
{

	int marks[80];
	int count;
	float mean=0.0;
	int pass=0;
	marks[0] = 10;
	marks[1] = 20;
	marks[2] = 30;
	marks[3] = 40;
	marks[4] = 50;
	marks[5] = 60;
	marks[6] = 70;
	
	count = 7;
	stats(marks,&count,&mean,&pass);
	printf("mean = %f, number passed = %d\n",mean,pass);

return(0);
}

void stats(int marks[80],int count,float mean,int pass)
{
	int i;
	int tot = 0;

	for(i=0;i<count;i++)
	{
		tot += marks[i];
		if(marks[i] > 39)
		{
			pass++;
		}
	}

	mean = tot/count;
}