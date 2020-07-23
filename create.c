/* Creation of mini system database */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define   HEADPHONE	1
#define   MICROPHONE	2
#define   LONGWAVE 	4
#define   MEDIUMWAVE	8
#define   FMWAVE       16
#define   FIVECD       32
#define   THREECD      64
#define   ONECD       128

void main()
{
void get_string(char *);
FILE *fptr;
short element;
short value;
char temp[10];
struct mini_system
{
char manufacturer[20];
char model[10];
short maximum_power_output;
float price;
char details;
} ;
struct mini_system mini_system_info;
struct mini_system *mini_system_ptr;
mini_system_ptr=&mini_system_info;
fptr=fopen("mini_systems","w");

printf("\nEnter manufacturer: ");
/* while (fgets(mini_system_info.manufacturer,20,stdin)!=NULL)
    { */
    if (mini_system_info.manufacturer[strlen(mini_system_info.manufacturer)-1]=='\n')
        mini_system_info.manufacturer[strlen(mini_system_info.manufacturer)-1]='\0'; 
    else
        while (getchar() != '\n') ;
    printf("Enter model: ");
    fgets(mini_system_info.model,10,stdin);
    if (mini_system_info.model[strlen(mini_system_info.model)-1]=='\n')
        mini_system_info.model[strlen(mini_system_info.model)-1]='\0';
    else
        while (getchar() != '\n') ;
    printf("Enter maximum power output: ");
    get_string(temp);
    mini_system_info.maximum_power_output=atoi(temp);
    printf("Enter price: ");
    get_string(temp);
    mini_system_info.price=atof(temp);
    mini_system_info.details=0;
    printf("Headphone? (y/n) ");
    get_string(temp);
    if (temp[0]=='y')
        mini_system_info.details|=HEADPHONE;
    printf("Microphone? (y/n) ");
    get_string(temp);
    if (temp[0]=='y')
        mini_system_info.details|=MICROPHONE;
    printf("Longwave? (y/n) ");
    get_string(temp);
    if (temp[0]=='y')
        mini_system_info.details|=LONGWAVE;

    printf("Mediumwave? (y/n) ");
    get_string(temp);
    if (temp[0]=='y')
        mini_system_info.details|=MEDIUMWAVE;
    printf("FMwave? (y/n) ");
    get_string(temp);
    if (temp[0]=='y')
        mini_system_info.details|=FMWAVE;
    printf("Five CD player? (y/n) ");
    get_string(temp);
    if (temp[0]=='y')
        mini_system_info.details|=FIVECD;
    printf("Three CD player? (y/n) ");
    get_string(temp);
    if (temp[0]=='y')
        mini_system_info.details|=THREECD;
    printf("One CD player? (y/n) ");
    get_string(temp);
    if (temp[0]=='y')
        mini_system_info.details|=ONECD;
    fwrite(mini_system_ptr,sizeof(struct mini_system),1,fptr);
    /*
    printf("\nEnter manufacturer: ");
    } */
fclose(fptr);
/* printf("\n"); */
}

void get_string(char temp[10])
{
    fgets(temp,10,stdin);
    if (temp[strlen(temp)-1]=='\n')
        temp[strlen(temp)-1]='\0';
    else
        while (getchar() != '\n') ;
}
