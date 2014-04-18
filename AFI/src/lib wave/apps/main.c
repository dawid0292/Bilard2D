#include "wave.h"
#include <gl.h>

void define_lighting(),do_draw();

main(int argc,char **argv)
{
 wfObject *obj;
 Object gl_obj;
 int i;
 foreground();
 keepaspect(1,1);
 winopen(argv[0]);
 RGBmode();
 zbuffer(TRUE);
 doublebuffer();
 gconfig();
 dpInitTexmapping();
 concave(TRUE);
/*
 backface(TRUE);
*/
 mmode(MVIEWING);
 perspective(400,1.0,0.1,1000.0);
 czclear(0,getgdesc(GD_ZMAX));
 define_lighting();
 makeobj(gl_obj=genobj());
  for (i=1; i<argc; i++)
	{
	obj = wfReadObject(argv[i]);
	if (!obj) exit();
	printf("%d verts   %d norms\n",obj->nverts,obj->nnorms);
	cpack(0xff0000ff);
	linewidth(2);
	wfDrawObject(obj);
	}
 closeobj();
 do_draw(gl_obj);
 spin(do_draw,gl_obj);
}

static Matrix idmat = { 1,0,0,0,  0,1,0,0,  0,0,1,0,  0,0,0,1};

void do_draw(Object gl_obj)
{
 extern Matrix translate_mat,rotate_mat,scale_mat;
 extern float spin_tx,spin_ty,spin_tz,spin_rx,spin_ry,spin_rz,spin_sc;
 reshapeviewport();
 cpack(0x606060); clear(); zclear();
 pushmatrix();
 translate(0,0,-100.0);
/*
 multmatrix(translate_mat);
 multmatrix(rotate_mat);
 multmatrix(scale_mat);
*/
 translate(spin_tx,spin_ty,spin_tz);
 rot(spin_ry,'y');
 rot(spin_rx,'x');
 scale(spin_sc,spin_sc,spin_sc);
 blendfunction(BF_SA,BF_MSA); 
/* afunction(0,AF_NOTEQUAL); */
 callobj(gl_obj);
 popmatrix();
 swapbuffers();
}

void define_lighting()
{
 static float light_data[] = { LCOLOR, 1.0, 1.0, 1.0,
                               POSITION, 0.2, 0.2, 1.0, 0.0,
                               LMNULL };
 lmdef(DEFLMODEL,1,0,NULL);
 lmdef(DEFMATERIAL,1,0,NULL);
 lmdef(DEFLIGHT,1,10,light_data);
 lmbind(LMODEL,1);
 lmbind(LIGHT0,1);
}
