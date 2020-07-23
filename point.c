#include<stdio.h>

main()
{
void get_2_int();
int int1,int2;
get_2_int(&int1,&int2);
printf("\n The first integer is %d",int1);
printf("\n Whose address is     %x",&int1);
printf("\n The second integer is %d",int2);
printf("\n Whose address is      %x",&int2);
return 0;
}

void get_2_int(no1,no2)
int *no1,*no2;
{
*no1=3;
printf("\n First number is %d",*no1);
printf("\n Whose address is %x",no1);
*no2=5;
printf("\n Second number is %d",*no2);
printf("\n Whose address is %x",no2);
}
