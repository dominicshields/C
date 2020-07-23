/*--------------------------------------------------------------------
CMP 502 Coursework - Dominic Shields
The problem:
Write the following functions to implement a sequentially ordered list: 
List_new 
List_clear 
List_is_full 
List_is_empty 
List_size 
List_includes 
List_ordered_insert 
List_ordered_delete 
List_ascending_write 
List_descending_write 
--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

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
    int a; 
    l->num = 0;
    l->headp = 0;
    
    fprintf(stdout,"\nIn Function List_new\n");
    
    for (a=0;a<10;a++)
    {
    	l->listnodes[a] = ' '; 
    	l->listlinks[a] = 0;
    }
}

void List_clear(list *l)
{
    int a; 
    l->num = 0;
    l->headp = 0;
    
    fprintf(stdout,"\nIn Function List_clear\n");
    
    for (a=0;a<10;a++)
    {
    	l->listnodes[a] = ' '; 
    	l->listlinks[a] = 0;
    }
}

int List_is_full(list *l)
{
    fprintf(stdout,"\nIn Function List_is_full\n");
    return(l->num == LISTMAXSIZE);
}

int List_is_empty(list *l)
{
    fprintf(stdout,"\nIn Function List_is empty\n");
    return(l->num == 0);
}

int List_size(list *l)
{
    fprintf(stdout,"\nIn Function List_size\n");
    return(l->num);
}

int List_includes(itemtype x,list *l)
{
   int flag = 0;
   int a; 
   
   fprintf(stdout,"\nIn Function List_includes\n");
   
   for(a=0;a<l->num;a++)
   { 
	if(l->listnodes[a]==x) 
	{ 
	  flag = 1;
	} 
    } 
    
    return(flag);
}

void List_ordered_insert(itemtype x, list *l)
{
    int a;
    
     fprintf(stdout,"\nIn Function List_ordered_insert\n");
    
    if(l->num == 0)         /* IF LIST HAS NO ITEMS */
    { 
      l->listnodes[0] = x; 
      l->listlinks[0] = -1;
    }
    else
    {	
      a = l->num - 1;
      while ((a<l->listnodes[a])&&(a>=0))
      {
	  l->listnodes[a+1] = l->listnodes[a];
	  l->listlinks[a+1] = (l->listlinks[a])+1;
	  l->listlinks[l->num] = -1;
	  a--;
      }
      l->listnodes[a+1]=x;
      l->listlinks[a]=a+1;
      l->listlinks[a+1] = l->listlinks[a]+1; 
      l->listlinks[l->num] = -1;
    }
    l->num++;
}

void List_ordered_delete(itemtype x, list *l)
{
    int a;
    int z;
    
    fprintf(stdout,"\nIn Function List_ordered_delete\n");
     
    a=l->num-1;
    while ((x!=l->listnodes[a])&&(a>=0))
    a--;
    if((x==l->listnodes[a])&&(a>=0))
    {
      for (z=a;z<l->num;z++)
      {
	  l->listnodes[z] = l->listnodes [z+1];
	  l->listlinks[z] = (l->listlinks [z+1])-1;
      }
      l->num--;	
      l->listlinks[l->num-1] =-1;
    }
}

void List_ascending_write (list *l)
{
   int a;
   
   fprintf(stdout,"\nIn Function List_ascending_write\n");
    
   for (a=0;a<=l->num;a++)
   {
	fprintf(stdout,"List Contains\t\t\t%c\n", l->listnodes[a]);
        fprintf(stdout,"At Array position\t\t%d\n", l->listlinks[a]);
   }	
	

}

void List_descending_write(list *l)
{
   int a;
   
   fprintf(stdout,"\nIn Function List_descending_write\n");
      
   for (a=l->num;a>=0;a--)
   {
        fprintf(stdout,"List Contains\t\t\t%c\n",l->listnodes[a]);
        fprintf(stdout,"At array position\t\t%d\n",l->listlinks[a]);
   }
}
   

main()
{	
	list l;
	itemtype a;
	int retval;

	List_new(&l);
	
	a = 'A';
	List_ordered_insert(a,&l);
	a = 'B';
	List_ordered_insert(a,&l);
	a = 'C';
	List_ordered_insert(a,&l);
	a = 'D';
	List_ordered_insert(a,&l);
	a = 'E';
	List_ordered_insert(a,&l);
	a = 'F';
	List_ordered_insert(a,&l);
	List_ascending_write(&l);
	
	fprintf(stdout,"\n");
	
	fprintf(stdout,"List size\t\t\t%d\n\n",List_size(&l));
	
	a = 'B';
	List_ordered_delete(a,&l);
	List_ascending_write(&l);
	fprintf(stdout,"\n");
	
	List_descending_write(&l);
	fprintf(stdout,"\n");
	
	a = 'C';
	retval = List_includes(a,&l);
	if(retval == 1)
	{
		fprintf(stdout,"List includes\t\t\t%c\n",a);
	}
	else
	{
		fprintf(stdout,"List does not include %c\n",a);
	}
	fprintf(stdout,"\n");
	
	List_clear(&l);
	List_ascending_write(&l);
	fprintf(stdout,"\n");
	retval = List_is_empty(&l);
	if(retval == 1)
	{
		fprintf(stdout,"List is empty\n\n");
	}
	else
	{
		fprintf(stdout,"List is not empty\n\n");
	}
	
	retval = List_is_full(&l);
	if(retval == 1)
	{
		fprintf(stdout,"List is full\n\n");
	}
	else
	{
		fprintf(stdout,"List is not full\n\n");
	}
	

	return(0);

}       
