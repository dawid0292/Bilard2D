#include <stdio.h>
#include <math.h>
#include <GL/gl.h>
#include "wave.h"

static void draw_face(wfPart *p,wfObject *obj);
static void draw_line(wfPart *p,wfObject *obj);

/* wfDrawObject - draws a Wavefront object defined by the wfObject struct. */
void wfDrawObject(wfObject *obj)
{
 wfPart *p;
 wfMaterial *m;
 int mtlbound=0,texbound=0,texscaled=0,
	curmtl=-1,curmap=-1;
 if (!obj) return;
 if (!obj->gl_initted) wfInitObject(obj);
 for (p=obj->parts; p; p=p->next)
	{
	if (p->parttype == WF_FACE) draw_face(p,obj);
	else if (p->parttype == WF_LINE) draw_line(p,obj);
	else if (p->parttype == WF_MATERIAL)
		{
		m = p->part.material;
		if (m)
			{
			if (m->material_number != curmtl)
				{
				if (!mtlbound)
					glEnable(GL_LIGHTING);
				if (m->illum == 0)
					glColor3fv(m->diffuse);
				glCallList(m->material_number);
				curmtl = m->material_number;
				mtlbound=1;
				}
			}
		else
			{
			glDisable(GL_LIGHTING);
			mtlbound = 0;
			}
		}
	else if ((p->parttype == WF_TEXTURE) &&
		 (p->part.texture->texmap_number != curmap) &&
		 (wfIsEnabled(WF_USE_TEXTURES)))
	    {
	    if (p->part.texture->texmap_number == 0)
		{
		glDisable(GL_TEXTURE_2D);
		texbound = 0;
		}
	    else
		{
		if (!texbound)
			glEnable(GL_TEXTURE_2D);
		glCallList(p->part.texture->texmap_number);
		if ((p->part.texture->Kd_s[0] != 1) ||
		    (p->part.texture->Kd_s[1] != 1) ||
		    (p->part.texture->Kd_o[0] != 0) ||
		    (p->part.texture->Kd_o[1] != 0))
			{
			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
			glScalef(p->part.texture->Kd_s[0],
				p->part.texture->Kd_s[1], 1.0);
			glTranslatef(p->part.texture->Kd_o[0],
				p->part.texture->Kd_o[1], 0.0);
			glMatrixMode(GL_MODELVIEW);
			texscaled = 1;
			}
		else if (texscaled)
			{
			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
			texscaled = 0;
			}
		curmap = p->part.texture->texmap_number;
		texbound=1;
		}
	    }
	}
 if (mtlbound)
	glDisable(GL_LIGHTING);
 if (texbound)
	{
	glDisable(GL_TEXTURE_2D);
	if (texscaled)
		{
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		}
	}
}

/* draw_face - draws a single face as a polygon. Uses normals if they
	were included. */
static void draw_face(wfPart *p,wfObject *obj)
{
 int v,curnorm=-1,curtexv=-1;
 wfFace *f;
 f = &(p->part.face);
 glBegin(GL_POLYGON);
  for (v=0; v<f->nverts; v++) {
	if ((f->norm[v]) && (f->norm[v] != curnorm))
		{
		glNormal3fv(obj->norm[f->norm[v]-1]);
		curnorm = f->norm[v];
		}
	if ((f->tex[v]) && (f->tex[v] != curtexv))
		{
		glTexCoord2fv(obj->texc[f->tex[v]-1]);
		curtexv = f->tex[v];
		}
	glVertex3fv(obj->vert[f->vert[v]-1]);
	}
 glEnd();
}

/* draw_line - draws a single line element. */
static void draw_line(wfPart *p,wfObject *obj)
{
 int v;
 wfLine *l;
 l = &(p->part.line);
 glBegin(GL_LINE_STRIP);
  for (v=0; v<l->nverts; v++) {
	glVertex3fv(obj->vert[l->vert[v]-1]);
	}
 glEnd();
}
