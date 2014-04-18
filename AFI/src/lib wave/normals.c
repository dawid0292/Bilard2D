#include <stdio.h>
#include <math.h>
#include "wave.h"

static int countFaces(wfObject *obj);
static void computeFaceNormal(wfFace *face,wfObject *obj,wfNormal norm,int clockwise);
static void assignNormalIndices(wfFace *face,int faceNum,int smooth);
static void addNormalToVertNorms(wfFace *face,wfObject *obj,wfNormal norm);
static void normalizeNormal(wfNormal norm);


void wfComputeNormals(wfObject *obj,int smooth,int clockwise)
{
 int numFaces = countFaces(obj),i;
 wfNormal *faceNorm;
 wfPart *part;
 if (obj->norm)
	wfFree(obj->norm);
 if (smooth)
	obj->nnorms = obj->nverts;
 else
	obj->nnorms = numFaces;
 obj->norm = (wfNormal *) wfAlloc(obj->nnorms * sizeof(wfNormal));
 faceNorm = (wfNormal *) wfAlloc(numFaces * sizeof(wfNormal));
 if (smooth)
	bzero(obj->norm,obj->nnorms * sizeof(wfNormal));
 for (part = obj->parts, i=0; part; part = part->next)
	if (part->parttype == WF_FACE)
		{
		computeFaceNormal(&part->part.face,obj,faceNorm[i],clockwise);
		assignNormalIndices(&part->part.face,i,smooth);
		if (smooth)
			addNormalToVertNorms(&part->part.face,obj,faceNorm[i]);
		i++;
		}
 if (smooth)
	for (i=0; i<obj->nnorms; i++)
		normalizeNormal(obj->norm[i]);
 else
	memcpy(obj->norm,faceNorm,numFaces * sizeof(wfNormal));
 free(faceNorm);
}

static int countFaces(wfObject *obj)
{
 wfPart *part;
 int num=0;
 for (part = obj->parts; part; part = part->next)
	if (part->parttype == WF_FACE)
		num++;
 return num;
}

static void computeFaceNormal(wfFace *face,wfObject *obj,wfNormal norm,int clockwise)
{
 float *v0,*v1,*v2;
 float dx1,dy1,dz1,dx2,dy2,dz2,nx,ny,nz,len;
 norm[0] = norm[1] = 0; norm[2] = 1;	/* Default value in case of error */
 if (face->nverts < 3)
	return;
 v0 = obj->vert[face->vert[0] - 1];
 v1 = obj->vert[face->vert[1] - 1];
 v2 = obj->vert[face->vert[2] - 1];
 if (clockwise)
	{
	dx1 = v0[0] - v1[0];
	dy1 = v0[1] - v1[1];
	dz1 = v0[2] - v1[2];
	dx2 = v2[0] - v1[0];
	dy2 = v2[1] - v1[1];
	dz2 = v2[2] - v1[2];
	}
 else
	{
	dx2 = v0[0] - v1[0];
	dy2 = v0[1] - v1[1];
	dz2 = v0[2] - v1[2];
	dx1 = v2[0] - v1[0];
	dy1 = v2[1] - v1[1];
	dz1 = v2[2] - v1[2];
	}
 nx = dy1*dz2 - dy2*dz1;
 ny = dz1*dx2 - dz2*dx1;
 nz = dx1*dy2 - dx2*dy1;
 len = fsqrt(nx*nx + ny*ny + nz*nz);
 if (len < 1e-20)
	return;
 norm[0] = nx / len;
 norm[1] = ny / len;
 norm[2] = nz / len;
}

static void assignNormalIndices(wfFace *face,int faceNum,int smooth)
{
 int i;
 if (!face->norm)
	face->norm = (int *) wfAlloc(face->nverts * sizeof(int));
 if (smooth)
	{
	for (i=0; i<face->nverts; i++)
		face->norm[i] = face->vert[i];
	}
 else
	{
	for (i=0; i<face->nverts; i++)
		face->norm[i] = faceNum + 1;
	}
}

static void addNormalToVertNorms(wfFace *face,wfObject *obj,wfNormal norm)
{
 int i,v;
 for (i=0; i<face->nverts; i++)
	{
	v = face->vert[i] - 1;
	obj->norm[v][0] += norm[0];
	obj->norm[v][1] += norm[1];
	obj->norm[v][2] += norm[2];
	}
}

static void normalizeNormal(wfNormal norm)
{
 float len;
 len = fsqrt(norm[0]*norm[0] + norm[1]*norm[1] + norm[2]*norm[2]);
 if (len < 1e-20)
	{
	0[norm] = 1[norm] = 0;
	2[norm] = 1;
	}
 else
	{
	norm[0] /= len;
	norm[1] /= len;
	norm[2] /= len;
	}
}
