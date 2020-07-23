#include <stdio.h>

int main() 
{
	int i;
  const char *names[] = { "John", "Mona", "Lisa", "Frank", NULL };

    for (i=0; names[i]; ++i) {
        const char *ch = names[i]; 
        while(*ch) {
            putchar(*ch++);
            putchar('\n');
        }
        putchar('\n');
    }
    
    return 0;
} 