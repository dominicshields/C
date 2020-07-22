/*
  ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                                                         บ
  บ         DRAGON = Program to generate dragon curve       บ
  บ                                                         บ
  บ              By Roger T. Stevens  6-10-92               บ
  บ                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/
#include <dos.h>
#include <stdio.h>
#include <math.h>

int xres = 640, yres = 480;
int  color, row, col;
float Xmax=1.4, Xmin= -0.4, Ymax = 0.8, Ymin = -0.8;
float P=1.646, Q=0.967, deltaX, deltaY, sqdif, prod, x, y, y2, xsq, ysq;

void plot(int x,int y,int color);
void setmode(int mode);
void cls(int color);

main()
{
	setmode(18);
	cls(7);
	deltaX = (Xmax - Xmin)/xres;
	deltaY = (Ymax - Ymin)/yres;
	col = 0;
	cols:
	if (col<xres)
	{
		row = 0;
		rows:
		if (row<yres)
		{
			x = Xmin + col*deltaX;
			y = Ymax - row*deltaY;
			xsq = ysq = 0.0;
			color = 0;
			iterations:
			if ((color<256) && (xsq + ysq < 4))
			{
				xsq = x*x;
				ysq = y*y;
				sqdif = ysq - xsq;
				prod = 2*x*y;
				y2 = Q*(sqdif + x) - P*(prod - y);
				x = P*(sqdif + x) + Q*(prod - y);
				y = y2;
				color++;
				goto iterations;
			}
			color = (color >= 256) ? ((int) ((xsq + ysq) * 8.0)) %7
				+ 9 : 1;
			plot(col, row, color);
			row++;
			goto rows;
		}
	col++;
	goto cols;
	}
	getch();
}

/*
  ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                                                         บ
  บ               setmode() = Sets Video Mode               บ
  บ                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

void setmode(int mode)
{

	union REGS reg;

	reg.x.ax = mode;
	int86 (0x10,&reg,&reg);
}

/*
  ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                                                         บ
  บ                 cls() = Clears the Screen               บ
  บ                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

void cls(int color)
{
	 union REGS reg;

	 reg.x.ax = 0x0600;
	 reg.x.cx = 0;
	 reg.x.dx = 0x1E4F;
	 reg.h.bh = color;
	 int86(0x10,&reg,&reg);
}

/*
  ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                                                         บ
  บ  plot() = Plots a point on the screen at a designated   บ
  บ            position using a selected color for 16 color บ
  บ            modes.                                       บ
  บ                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

void plot(int x, int y, int color)
{
	#define graph_out(index,val)  {outp(0x3CE,index);\
					 outp(0x3CF,val);}

	int dummy,mask;
	char far * address;

	address = (char far *) 0xA0000000L + (long)y * xres/8L +
		((long)x / 8L);
	mask = 0x80 >> (x % 8);
	graph_out(8,mask);
	graph_out(5,2);
	dummy = *address;
	*address = color;
	graph_out(5,0);
	graph_out(8,0xFF);
}
