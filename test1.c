*****Listing 1*****

#include	<graphics.h>

extern	void (*fill_box_call);
extern	void (*outtextxy_call);
extern	void (*putpixel_call);
extern	void (*line_call);

extern	struct viewport v;
extern	int		output_location;
extern	int		text_width;
extern	int		text_height;

#define	SCREEN		0
#define	PORTRAIT	1
#define	LANDSCAPE	2

/************************************************************
*	setup_device() - set output parameters
*
*	Parameters:
*		device (in) - SCREEN, PORTRAIT (printer), or
*					  LANDSCAPE (printer)
*	Notes:
*		1.	The sample code was done using Turbo C version 2.0
*
*	History
*		Original code William Roetzheim, 1990
*************************************************************/

void setup_device(int device)
{
	void	fill_box();
	void	outtextxy();
	void	line();
	void	putpixel();
	void	lj_fill_box();
	void	lj_outtextxy();
	void	lj_line();
	void	lj_putpixel();
	struct	viewporttype	vp; 	/* used for screen only */

	switch (device)
	{
		case SCREEN:
			fill_box_call = fill_box;
			outtextxy_call = outtextxy;
			line_call = line;
			putpixel_call = putpixel;
			text_width = 8;
			text_height = 10;	/* allow for spaces between lines */
			getviewsettings(&vp);
			v.left = vp.left;
			v.top = vp.top;
			v.right = vp.right;
			v.bottom = vp.bottom;
			break;

		case PORTRAIT:
		case LANDSCAPE:
			fill_box_call = lj_fill_box;
			outtextxy_call = lj_outtextxy;
			putpixel_call = lj_putpixel;
			v.left = 0;
			v.top = 0;
			if (device == LANDSCAPE)
			{
				v.right = 3179;
				v.bottom = 2249;
			}
			else
			{
				v.right = 2249;
				v.bottom = 3179;
			}
			break;
	}
	output_location = device;
}

