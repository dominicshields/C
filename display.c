/*-------------------------------------------------------------------------------
Program Name   : display.c
Program Author : Dominic Shields
Date           : 07/12/1999  
Purpose        : Seventh C Programming exercise to display details from a DB
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

void main(argc,argv)
int argc;
char *argv[];
{
FILE *fptr;
int testpat;

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

if(argc != 2)
{
	printf("Error, usage is display <room database>\n");
	exit(1);
}

if((fptr=fopen(argv[1],"r")) == NULL)
{
	printf("Error opening file %s, exiting program\n", argv[1]);
	exit(1);
}

testpat = OHP + REFRESH;


printf("***************************************************************************************************************************\n");
printf("Details of rooms containing an overhead projector and refreshment facilities seating 25 or more and costs less than 50 Quid\n");
printf("***************************************************************************************************************************\n\n");
printf("Building	Room Number	Max People	Charge\n\n");

fread(room_ptr, sizeof(struct room),1, fptr);

while (feof(fptr)== 0)
{
    if (((room_info.details & testpat) == testpat) & room_info.max_people > 24 & room_info.hire_charge < 50.00)
    {
	printf("%s\t\t%s\t%5d\t\t%5.2f\n",room_info.building,room_info.room_number,room_info.max_people,room_info.hire_charge);
    }

    fread(room_ptr, sizeof(struct room),1, fptr);
}

fclose(fptr);


}


