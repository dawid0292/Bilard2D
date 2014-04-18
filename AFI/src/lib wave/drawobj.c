#include <stdio.h>
#include <math.h>
#include <gl.h>
#include "wave.h"

static void draw_face(wfPart *p,wfObject *obj,int *,int *);
static void draw_line(wfPart *p,wfObject *obj);
static void draw_closedline(wfPart *p,wfObject *obj,int *curnorm,int *curtexv);

/* wfDrawObject - draws a Wavefront object defined by the wfObject struct. */
void wfDrawObject(wfObject *obj)
{
 wfPart *p;
 wfMaterial *m;
 int mtlbound=0,texbound=0,texscaled=0,
	curnorm=-1,curtexv=-1,curmtl=-1,curmap=-1,
	wireframe=wfIsEnabled(WF_DRAW_WIREFRAME);
 if (!obj) return;
 if (!obj->gl_initted) wfInitObject(obj);
 for (p=obj->parts; p; p=p->next)
	{
	if (p->parttype == WF_FACE)
		{
		if (wireframe)
			draw_closedline(p,obj,&curnorm,&curtexv);
		else
			draw_face(p,obj,&curnorm,&curtexv);
		}
	else if (p->parttype == WF_LINE) draw_line(p,obj);
	else if (p->parttype == WF_MATERIAL)
		{
		m = p->part.material;
		if ((m) && (m->material_number != curmtl))
			{
			if (m->illum == 0)
				RGBcolor((short)(m->diffuse[0]*255),
					 (short)(m->diffuse[1]*255),
					 (short)(m->diffuse[2]*255));
			lmbind(MATERIAL,m->material_number);
			curmtl = m->material_number;
			mtlbound=1;
			}
		else
			{
			lmbind(MATERIAL,0);
			mtlbound = 0;
			}
		}
	else if ((p->parttype == WF_TEXTURE) &&
		 (p->part.texture->texmap_number != curmap) &&
		 (wfIsEnabled(WF_USE_TEXTURES)))
		{
		texbind(TX_TEXTURE_0,p->part.texture->texmap_number);
		if ((p->part.texture->Kd_s[0] != 1) ||
		    (p->part.texture->Kd_s[1] != 1) ||
		    (p->part.texture->Kd_o[0] != 0) ||
		    (p->part.texture->Kd_o[1] != 0))
			{
			Matrix idmat = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
			mmode(MTEXTURE);
			loadmatrix(idmat);
			translate(p->part.texture->Kd_o[0],
				p->part.texture->Kd_o[1], 0.0);
			scale(p->part.texture->Kd_s[0],
				p->part.texture->Kd_s[1], 1.0);
			mmode(MVIEWING);
			texscaled = 1;
			}
		else if (texscaled)
			{
			Matrix idmat = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
			mmode(MTEXTURE);
			loadmatrix(idmat);
			mmode(MVIEWING);
			texscaled = 0;
			}
		curmap = p->part.texture->texmap_number;
		texbound=1;
		}
	}
 if (mtlbound) lmbind(MATERIAL,0);
 if (texbound)
	{
	texbind(TX_TEXTURE_0,0);
	if (texscaled)
		{
		Matrix idmat = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
		mmode(MTEXTURE);
		loadmatrix(idmat);
		mmode(MVIEWING);
		}
	}
}

/* draw_face - draws a single face as a polygon. Uses normals if they
	were included. */
static void draw_face(wfPart *p,wfObject *obj,int *curnorm,int *curtexv)
{
 int v;
 wfFace *f;
 f = &(p->part.face);
 bgnpolygon();
  for (v=0; v<f->nverts; v++) {
	if ((f->norm[v]) && (f->norm[v] != *curnorm))
		{
		n3f(obj->norm[f->norm[v]-1]);
		*curnorm = f->norm[v];
		}
	if ((f->tex[v]) && (f->tex[v] != *curtexv))
		{
		t2f(obj->texc[f->tex[v]-1]);
		*curtexv = f->tex[v];
		}
	v3f(obj->vert[f->vert[v]-1]);
	}
 endpolygon();
}

/* draw_closedline - draws a closed line for a wireframe polygon. */
static void draw_closedline(wfPart *p,wfObject *obj,int *curnorm,int *curtexv)
{
 int v;
 wfFace *f;
 f = &(p->part.face);
 bgnline();
  for (v=0; v<f->nverts; v++) {
	if ((f->norm[v]) && (f->norm[v] != *curnorm))
		{
		n3f(obj->norm[f->norm[v]-1]);
		*curnorm = f->norm[v];
		}
	if ((f->tex[v]) && (f->tex[v] != *curtexv))
		{
		t2f(obj->texc[f->tex[v]-1]);
		*curtexv = f->tex[v];
		}
	v3f(obj->vert[f->vert[v]-1]);
	}
 if ((f->norm[0]) && (f->norm[0] != *curnorm))
	{
	n3f(obj->norm[f->norm[0]-1]);
	*curnorm = f->norm[0];
	}
 if ((f->tex[0]) && (f->tex[0] != *curtexv))
	{
	t2f(obj->texc[f->tex[0]-1]);
	*curtexv = f->tex[0];
	}
 v3f(obj->vert[f->vert[0]-1]);
 endline();
}

/* draw_line - draws a single line element. */
static void draw_line(wfPart *p,wfObject *obj)
{
 int v;
 wfLine *l;
 l = &(p->part.line);
 bgnline();
  for (v=0; v<l->nverts; v++) {
	v3f(obj->vert[l->vert[v]-1]);
	}
 endline();
}
