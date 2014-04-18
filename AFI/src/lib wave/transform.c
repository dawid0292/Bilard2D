#include <stdio.h>
#include <math.h>
#include "wave.h"

static void rotPoint(float *p,float mat[3][3]);

void wfTranslateObject(wfObject *obj,float tx,float ty,float tz)
{
 int i;
 for (i=0; i<obj->nverts; i++)
	{
	obj->vert[i][0] += tx;
	obj->vert[i][1] += ty;
	obj->vert[i][2] += tz;
	}
}

void wfScaleObject(wfObject *obj,float sx,float sy,float sz)
{
 int i;
 for (i=0; i<obj->nverts; i++)
	{
	obj->vert[i][0] *= sx;
	obj->vert[i][1] *= sy;
	obj->vert[i][2] *= sz;
	}
}

void wfRotateObject(wfObject *obj,float angle,float x,float y,float z)
{
 int i;
 float rotmat[3][3],len,sinth,costh;
 len = fsqrt(x*x + y*y + z*z);
 x /= len;
 y /= len;
 z /= len;
 angle *= 0.017453293f;
 sinth = fsin(angle);
 costh = fcos(angle);
 rotmat[0][0] = (1-costh)*x*x + costh;
 rotmat[0][1] = (1-costh)*x*y + sinth*z;
 rotmat[0][2] = (1-costh)*x*z - sinth*y;
 rotmat[1][0] = (1-costh)*x*y - sinth*z;
 rotmat[1][1] = (1-costh)*y*y + costh;
 rotmat[1][2] = (1-costh)*y*z + sinth*x;
 rotmat[2][0] = (1-costh)*x*z + sinth*y;
 rotmat[2][1] = (1-costh)*y*z - sinth*x;
 rotmat[2][2] = (1-costh)*z*z + costh;
 for (i=0; i<obj->nverts; i++)
	rotPoint(obj->vert[i],rotmat);
 for (i=0; i<obj->nnorms; i++)
	rotPoint(obj->norm[i],rotmat);
}

static void rotPoint(float *p,float mat[3][3])
{
 float x,y,z;
 x = p[0]*mat[0][0] + p[1]*mat[1][0] + p[2]*mat[2][0];
 y = p[0]*mat[0][1] + p[1]*mat[1][1] + p[2]*mat[2][1];
 z = p[0]*mat[0][2] + p[1]*mat[1][2] + p[2]*mat[2][2];
 p[0] = x;
 p[1] = y;
 p[2] = z;
}

void wfDeformObject(wfObject *obj,void (*vertFunc)(float *),void (*normFunc)(float *))
{
 int i;
 if (vertFunc)
	for (i=0; i<obj->nverts; i++)
		vertFunc(obj->vert[i]);
 if (normFunc)
	for (i=0; i<obj->nnorms; i++)
		normFunc(obj->norm[i]);
}
