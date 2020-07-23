/*-------------------------------------------------------------------------------
Program Name   : summary.c
Program Author : Dominic Shields
Date           : 07/12/1999  
Purpose        : Eighth C Programming exercise to display details from a DB
-------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void main(argc,argv)
int argc;
char *argv[];
{
FILE *fptr;
int testpat;

struct stock_item
{
char name[20];
short quantity_in_stock;
float price;
} ;
struct stock_item stock_info;
struct stock_item *stock_ptr;
stock_ptr=&stock_info;

if(argc != 2)
{
	printf("Error, usage is %s <stock database>\n",argv[0]);
	exit(1);
}

if((fptr=fopen(argv[1],"r")) == NULL)
{
	printf("Error opening file %s, exiting program\n", argv[1]);
	exit(1);
}



printf("************************\n");
printf("Items out of stock\n");
printf("************************\n\n");

fread(stock_ptr, sizeof(struct stock_item),1, fptr);

while (feof(fptr)== 0)
{
    if (stock_info.quantity_in_stock == 0)
    {
	printf("%s\n",stock_info.name);
    }

    fread(stock_ptr, sizeof(struct stock_item),1, fptr);
}

fclose(fptr);


}


