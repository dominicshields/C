/* keyboard.c */
#include <stdio.h>
#include <dos.h>

union scan
{
int c;
char ch[2];
} sc;

main()
{
	do
	{
		sc.c = get_key();
		if(sc.ch[0] == 0)
		{
			fprintf(stdout,"Special key no = %d\n",sc.ch[1]);
		}
		else
		{
			fprintf(stdout,"key = %c\n",sc.ch[0]);
			fprintf(stdout,"hex = %x\n",sc.ch[0]);
		}
	} while(sc.ch[0]!='q');

	return(0);
}

get_key()
{
union REGS r;
	r.h.ah = 0;
	return int86(0x16,&r,&r);
}
