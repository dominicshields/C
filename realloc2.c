#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **myArray;
int arrayIndexs;

main(int argc, char *argv[])
{
    int i;
    myArray = malloc(2 * sizeof(char*));
    arrayIndexs = sizeof(*myArray) / sizeof(char*);

    //Allocate memory for each [x]
    for (i = 0; i <= arrayIndexs; i++)
        myArray[i] = malloc(254 * sizeof(char*));
    //Populate initial values
    if(myArray != NULL)
    {
        strcpy(myArray[0], "string 1");
        strcpy(myArray[1], "string 2");
    }
    //Print out array values
    printf("Before: \n");
    for (i = 0; i <= arrayIndexs; i++)
        printf("Array[%d]: %s\n",i, myArray[i]);

    /*arrayIndexs++;*/
    
    //Expand array to allow one additional item in the array
    myArray = (char **)realloc(myArray,++arrayIndexs * sizeof(char*));

    //Allocate memory for the new string item in the array
    myArray[arrayIndexs] = malloc(254 * sizeof(char*));

    //Populate a new value in the array
    strcpy(myArray[arrayIndexs], "string 3"); //

    //Print out array values
    printf("After: \n");
    for (i = 0; i <= arrayIndexs; i++)
        printf("Array[%d]: %s\n",i, myArray[i]);
        
    return 0;
}