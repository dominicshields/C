#include <stdio.h>
#include <string.h>
#include <sys\timeb.h> 
#include <time.h>

static char* get_timestamp(char* buf) {
    struct timeb start;
    char append[100];
    if (buf) {
        ftime(&start);
        strftime(buf, 100, "%H:%M:%S", localtime(&start.time));

        /* append milliseconds */
        sprintf(append, ":%03u", start.millitm);
        strcat(buf, append);
    }
    return buf;
}

int main()
{
    char tmp[100];
    int i;
    int countdown = 0;
    for (i = 0; i < 10; i++, countdown = 0) {
        printf("%s\n", get_timestamp(tmp));
        while (countdown++ < 1000000)
            ;
    }
    return (1);
}