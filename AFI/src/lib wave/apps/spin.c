#include <gl/gl.h>
#include <gl/device.h>

#define IDENTITY  { { 1.0, 0.0, 0.0, 0.0, }, \
		    { 0.0, 1.0, 0.0, 0.0, }, \
		    { 0.0, 0.0, 1.0, 0.0, }, \
		    { 0.0, 0.0, 0.0, 1.0 } }

/* translate_mat should probably be changed by the calling program
   to move the object back a bit */
Matrix translate_mat = IDENTITY;
Matrix rotate_mat = IDENTITY;
Matrix scale_mat = IDENTITY;

/* Drawing routine should perform :
	loadmatrix(translate_mat);
	multmatrix(rotate_mat);
	multmatrix(scale_mat);
*/

static void scale_world(int dx,int dy);
static void trans_world(int dx,int dy);
static void rotate_world(int dx,int dy);
static void ztrans_world(int dx,int dy);

spin(void (*drawfn)(),void *data)
{
 short val,dev;
 int prevx,prevy,dx,dy,curx,cury;
 qdevice(MOUSEX);
 qdevice(MOUSEY);
 qdevice(LEFTMOUSE);
 qdevice(MIDDLEMOUSE);
 prevx = getvaluator(MOUSEX);
 prevy = getvaluator(MOUSEY);
 while (1) {
	dev=qread(&val);
	if ((dev == MOUSEX) || (dev == MOUSEY)) {
		curx = getvaluator(MOUSEX);
		dx = curx - prevx;
		if ((dx>5) || (dx<-5)) prevx = curx;
		else dx=0;
		cury = getvaluator(MOUSEY);
		dy = cury - prevy;
		if ((dy>5) || (dy<-5)) prevy = cury;
		else dy=0;
		if ((!dx) && (!dy)) continue;
		if (getbutton(LEFTMOUSE)) {
			if (getbutton(MIDDLEMOUSE)) ztrans_world(dx,dy);
			else if (getbutton(RIGHTMOUSE)) scale_world(dx,dy);
			else trans_world(dx,dy);
			(*drawfn)(data);
			}
		else if (getbutton(MIDDLEMOUSE)) {
			rotate_world(dx,dy);
			(*drawfn)(data);
			}
		}
	else if (dev==REDRAW) (*drawfn)(data);
	}
 unqdevice(MOUSEX);
 unqdevice(MOUSEY);
 qreset();
 qenter(RIGHTMOUSE,1);
}

static Matrix idmat = IDENTITY;

/*
static void scale_world(int dx,int dy)
{
 float sc;
 sc = 1.0 + dy/40.0;
 if (sc<=0.0) return;
 loadmatrix(idmat);
 scale(sc,sc,sc);
 multmatrix(scale_mat);
 getmatrix(scale_mat);
}

static void trans_world(int dx,int dy)
{
 loadmatrix(idmat);
 translate((Coord)dx/5.0,(Coord)dy/5.0,0.0);
 multmatrix(translate_mat);
 getmatrix(translate_mat);
}

static void rotate_world(int dx,int dy)
{
 loadmatrix(idmat);
 rotate(-dy*5,'x');
 rotate(dx*5,'y');
 multmatrix(rotate_mat);
 getmatrix(rotate_mat);
}
*/

float spin_tx=0,spin_ty=0,spin_tz=0,spin_rx=0,spin_ry=0,spin_rz=0,spin_sc=1;

static void ztrans_world(int dx,int dy)
{
 spin_tz += dy/3.0;
}

static void scale_world(int dx,int dy)
{
 float sc;
 sc = 1.0 + dy/40.0;
 if (sc<=0.0) return;
 spin_sc *= sc;
}

static void trans_world(int dx,int dy)
{
 spin_tx += dx/5.0;
 spin_ty += dy/5.0;
}

static void rotate_world(int dx,int dy)
{
 spin_rx -= dy;
 spin_ry += dx;
/*
 loadmatrix(idmat);
 rotate(-dy*5,'x');
 rotate(dx*5,'y');
 multmatrix(rotate_mat);
 getmatrix(rotate_mat);
*/
}
