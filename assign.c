/* Implementation of sequential content ordered list */
/* uses a sentinal value */

#include <stdio.h>
#include <stdlib.h>

/* #include "seqtype.h"*/


#define LISTMAXSIZE 10
#define END   -1
#define EMPTY -2

typedef char itemtype; 

typedef struct {
    itemtype listnodes[LISTMAXSIZE]; 
    int      listlinks[LISTMAXSIZE]; 
    int      num;
    int      headp;
}list;
 

void List_new(list *l)
{
    l->num = 0;
}

void List_clear(list *l)
{
    l->num = 0;
}

int List_is_full(list *l)
{
    return(l->num == LISTMAXSIZE);
}

int List_is_empty(list *l)
{
    return(l->num == 0);
}

int List_size(list *l)
{
    return(l->num);
}

int List_includes(itemtype x,list *l)
{
    int scanp;

    l->listnodes[0] = x;  /* sentinal for backwards search */
    scanp = l->num;
    while (x<l->listnodes[scanp])
        scanp--;
    if ((x == l->listnodes[scanp]) && (scanp != 0))
        return(1);
    else
        return(0);
}

void List_ordered_insert(itemtype x, list *l)
{
    int scanp;

    l->listnodes[0] = x;  /* sentinal for backwards search */
    scanp = l->num;
    while (x < l->listnodes[scanp]) {
        l->listnodes[scanp + 1] = l->listnodes[scanp];
        scanp--;
    }
    l->listnodes[scanp + 1] = x;
    l->num++;
}

void List_ordered_delete(itemtype x, list *l)
{
    int scanp;
    int index;

    l->listnodes[0] = x;  /* sentinal for backwards search */
    scanp = l->num;
    while (x < l->listnodes[scanp])
        scanp--;
    if ((x == l->listnodes[scanp]) && (scanp != 0)) {
        for (index = scanp; index < l->num; index++)
            l->listnodes[index] = l->listnodes[index + 1];
        l->num--;
    }
}

void List_ascending_write (list *l)
{
    int scanp;

    for (scanp = 1; scanp <= l->num; scanp++)
        printf("%c\t",l->listnodes[scanp]);
    printf("\n");
}

void List_descending_write (list *l)
{
    int scanp;

    for (scanp = l->num; scanp >= 1; scanp--)
        printf("%c\t",l->listnodes[scanp]);
    printf("\n");
}
   

main()
{
	
	list l;

	List_new(&l);
	
	l.num = 5;
	
	printf("Num = %d\n",l.num);
	
	List_new(&l);
	
	printf("Num = %d\n",l.num);
	

	return 0;

}       
