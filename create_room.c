/*-------------------------------------------------------------------------------
Program Name   : create_room.c
Program Author : Dominic Shields
Date           : 30/11/1999  
Purpose        : Creates database for assignment 4
-------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define   OHP		1
#define   VIDEO		2
#define   SLIDE		4
#define   BLACKBOARD	8
#define   WHITEBOARD	16
#define   FLIPCHART	32
#define   DIMMER	64
#define   REFRESH       128

void main()
{
void get_string(char *);
FILE *fptr;
short element;
short value;
char temp[10];

struct room
{
char building[20];
char room_number[6];
short max_people;
float hire_charge;
char details;
} ;
struct room room_info;
struct room *room_ptr;
room_ptr=&room_info;

fptr=fopen("room.dat","w");
printf("\nEnter Room No: ");
while (fgets(room_info.room_number,6,stdin)!=NULL)
{
    if (room_info.room_number[strlen(room_info.room_number)-1]=='\n')
    {
        room_info.room_number[strlen(room_info.room_number)-1]='\0'; 
    }
    else
    {
        while (getchar() != '\n') ;
    }      

    printf("Enter building name: ");
    fgets(room_info.building,20,stdin);

    if (room_info.building[strlen(room_info.building)-1]=='\n')
    {
        room_info.building[strlen(room_info.building)-1]='\0'; 
    }
    else
    {
        while (getchar() != '\n') ;
    }      

    printf("Enter the maximum number of people the room holds: ");
    get_string(temp);
    room_info.max_people=atoi(temp);

    printf("Enter price of room hire_charge: ");
    get_string(temp);
    room_info.hire_charge=atof(temp);

    room_info.details=0;

    printf("OHP? (y/n) ");
    get_string(temp);
    if (temp[0]=='y')
        room_info.details|=OHP;

    printf("Video Projector? (y/n) ");
    get_string(temp);
    if (temp[0]=='y')
        room_info.details|=VIDEO;

    printf("Slide Projector? (y/n) ");
    get_string(temp);
    if (temp[0]=='y')
        room_info.details|=SLIDE;

    printf("Black Board? (y/n) ");
    get_string(temp);
    if (temp[0]=='y')
        room_info.details|=BLACKBOARD;

    printf("White Board? (y/n) ");
    get_string(temp);
    if (temp[0]=='y')
        room_info.details|=WHITEBOARD;

    printf("Flip Chart? (y/n) ");
    get_string(temp);
    if (temp[0]=='y')
        room_info.details|=FLIPCHART;

    printf("Light Dimmer Switch? (y/n) ");
    get_string(temp);
    if (temp[0]=='y')
        room_info.details|= DIMMER;

    printf("Refreshments? (y/n) ");
    get_string(temp);
    if (temp[0]=='y')
        room_info.details|=REFRESH;

    fwrite(room_ptr,sizeof(struct room),1,fptr);
    printf("\nEnter Room No: ");
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
