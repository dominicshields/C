/*-------------------------------------------------------------------------------
Program Name   : create_stock.c
Program Author : Dominic Shields
Date           : 07/12/1999  
Purpose        : Creates second (stock) database for assignment 4
-------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void main()
{
void get_string(char *);
FILE *fptr;
short element;
short value;
char temp[10];

struct stock
{
char name[20];
short quantity_in_stock;
float price;
} ;
struct stock stock_info;
struct stock *stock_ptr;
stock_ptr=&stock_info;

fptr=fopen("stock","w");
printf("\nEnter Stock Name: ");
while (fgets(stock_info.name,20,stdin)!=NULL)
{
    if (stock_info.name[strlen(stock_info.name)-1]=='\n')
    {
        stock_info.name[strlen(stock_info.name)-1]='\0'; 
    }
    else
    {
        while (getchar() != '\n') ;
    }      

    printf("Enter the quantity in stock: ");
    get_string(temp);
    stock_info.quantity_in_stock=atoi(temp);

    printf("Enter price of stock item: ");
    get_string(temp);
    stock_info.price=atof(temp);

    fwrite(stock_ptr,sizeof(struct stock),1,fptr);
    printf("\nEnter Stock Name: ");
    }
fclose(fptr);
printf("\n");
}


void get_string(char temp[10])
{
    fgets(temp,10,stdin);
    if (temp[strlen(temp)-1]=='\n')
        temp[strlen(temp)-1]='\0';
    else
        while (getchar() != '\n') ;
}
