#include <stdio.h>
#include <math.h>
#include "wave.h"

static int intersects(wfFace *face,wfObject *obj,float *p0,float *delta,float *t);
static void computePlane(wfFace *face,wfObject *obj,float plane[4]);
static int pointInPoly(float pt[3],wfFace *face,wfObject *obj,float plane[4]);


wfFace *wfRayIntersection(wfObject *obj,float *ray0,float *raydelta,float *ret_t)
{
 float t,min_t;
 wfFace *hit=NULL;
 wfPart *part;
 for (part = obj->parts; part; part = part->next)
	if (part->parttype == WF_FACE)
		if (intersects(&part->part.face,obj,ray0,raydelta,&t))
			if ((!hit) || (t < min_t))
				{
				hit = &part->part.face;
				min_t = t;
				}
 if ((hit) && (ret_t))
	*ret_t = min_t;
 return hit;
}

int wfRayHits(wfObject *obj,float *ray0,float *raydelta)
{
 float t;
 wfPart *part;
 for (part = obj->parts; part; part = part->next)
	if (part->parttype == WF_FACE)
		if (intersects(&part->part.face,obj,ray0,raydelta,&t))
			return 1;
 return 0;
}

static int intersects(wfFace *face,wfObject *obj,float *p0,float *delta,float *t)
{
 float plane[4],denom,hitpt[3];
 computePlane(face,obj,plane);
 denom = plane[0] * delta[0] + plane[1] * delta[1] + plane[2] * delta[2];
 if (fabs(denom) > 1e-20)
	{
	*t = - (plane[0]*p0[0] + plane[1]*p0[1] + plane[2]*p0[2] + plane[3]) /
		denom;
	if (*t < 0)
		return 0;
	hitpt[0] = p0[0] + delta[0] * (*t);		
	hitpt[1] = p0[1] + delta[1] * (*t);		
	hitpt[2] = p0[2] + delta[2] * (*t);
	if (pointInPoly(hitpt,face,obj,plane))
		return 1;
	else
		return 0;		
	}
 return 0;
}

static void computePlane(wfFace *face,wfObject *obj,float plane[4])
{
 float *v0,*v1,*v2;
 float dx1,dy1,dz1,dx2,dy2,dz2;
 plane[0] = plane[1] = plane[3] = 0; plane[2] = 1;	/* Default value in case of error */
 if (face->nverts < 3)
	return;
 v0 = obj->vert[face->vert[0] - 1];
 v1 = obj->vert[face->vert[1] - 1];
 v2 = obj->vert[face->vert[2] - 1];
 dx1 = v0[0] - v1[0];
 dy1 = v0[1] - v1[1];
 dz1 = v0[2] - v1[2];
 dx2 = v2[0] - v1[0];
 dy2 = v2[1] - v1[1];
 dz2 = v2[2] - v1[2];
 plane[0] = dy1*dz2 - dy2*dz1;
 plane[1] = dz1*dx2 - dz2*dx1;
 plane[2] = dx1*dy2 - dx2*dy1;
 plane[3] = - (plane[0]*v0[0] + plane[1]*v0[1] + plane[2]*v0[2]);
}

static int pointInPoly(float pt[3],wfFace *face,wfObject *obj,float plane[4])
{
 int index0,index1,i,hits=0;
 float delta,t,*v0,*v1,coord1;
 if ((fabs(plane[0]) > fabs(plane[1])) && (fabs(plane[0]) > fabs(plane[2])))
	{ index0 = 1; index1 = 2; }
 else if (fabs(plane[1]) > fabs(plane[2]))
	{ index0 = 0; index1 = 2; }
 else
	{ index0 = 0; index1 = 1; }
 for (i=0; i<face->nverts; i++)
	{
	v0 = obj->vert[face->vert[i] - 1];
	v1 = obj->vert[face->vert[(i+1)%face->nverts] - 1];
	delta = v1[index0] - v0[index0];
	if (fabs(delta) < 1e-20)
		continue;
 	t = (pt[index0] - v0[index0]) / delta;
	if ((t<0) || (t>=1))
		continue;
	coord1 = v0[index1] + t * (v1[index1] - v0[index1]);
	if (coord1 >= pt[index1])
		hits++;
	}
 return (hits&1);
}
