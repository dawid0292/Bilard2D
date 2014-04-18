#include "wave.h"
#include <math.h>


static struct { int id,value; } wfFeature[] = 
	{
	 { WF_SHOW_DEFINED_TEXTURES, 0 },
	 { WF_USE_TEXTURES, 1 },
	 { WF_INCLUDE_ALL_PARTS, 0 },
	 { WF_DRAW_WIREFRAME, 0 },
	 { -1, 0 }
	};

void wfEnable(int id)
{
 int i;
 for (i=0; wfFeature[i].id > -1; i++)
	if (wfFeature[i].id == id)
		{
		wfFeature[i].value = 1;
		return;
		}
 fprintf(stderr,"wfEnable error: invalid id %d\n",id);
}

int wfIsEnabled(int id)
{
 int i;
 for (i=0; wfFeature[i].id > -1; i++)
	if (wfFeature[i].id == id)
		return wfFeature[i].value;
 fprintf(stderr,"wfIsEnabled error: invalid id %d\n",id);
 return 0;
}

void wfDisable(int id)
{
 int i;
 for (i=0; wfFeature[i].id > -1; i++)
	if (wfFeature[i].id == id)
		{
		wfFeature[i].value = 0;
		return;
		}
 fprintf(stderr,"wfDisable error: invalid id %d\n",id);
}

static int (*wfNewIDFunc)(void) = NULL;

void wfSetNewIDFunction(int (*idfn)(void))
{
 wfNewIDFunc = idfn;
}

/* wfNewID - returns a new ('unique') ID number. For materials, texmaps, etc. */
int wfNewID(void)
{
 static int next_number = 256;
 if (wfNewIDFunc) return (*wfNewIDFunc)();
 else
	{
#ifdef IRISGL
	next_number++;
	return next_number;
#else
	return glGenLists(1);
#endif
	}
}

/* wfGetBoundingSphere - computes the center and radius of a bounding
	sphere for an object. */
void wfGetBoundingSphere(wfObject *obj,wfVertex center,float *radius)
{
 float xsum,ysum,zsum,dx,dy,dz,d,maxd;
 int i;
 if ((!obj) || (!obj->nverts)) return;
 xsum = ysum = zsum = 0.0;
 for (i=0; i<obj->nverts; i++)
	{
	xsum += obj->vert[i][0];
	ysum += obj->vert[i][1];
	zsum += obj->vert[i][2];
	}
 center[0] = xsum/obj->nverts;
 center[1] = ysum/obj->nverts;
 center[2] = zsum/obj->nverts;
 maxd = 0;
 for (i=0; i<obj->nverts; i++)
	{
	dx = obj->vert[i][0] - center[0];
	dy = obj->vert[i][1] - center[1];
	dz = obj->vert[i][2] - center[2];
	d = dx*dx + dy*dy + dz*dz;
	if (d>maxd) maxd = d;
	}
 *radius = fsqrt(maxd);
}

/* wfGetBoundingBox - computes an axis-aligned bounding box for an object.
	It is returned as two corner vertices (minx,miny,minz) &
	(maxx,maxy,maxz). */
void wfGetBoundingBox(wfObject *obj,wfVertex corner0,
			wfVertex corner1)
{
 float minx,miny,minz,maxx,maxy,maxz;
 int i;
 if ((!obj) || (!obj->nverts)) return;
 minx = maxx = obj->vert[0][0];
 miny = maxy = obj->vert[0][1];
 minz = maxz = obj->vert[0][2];
 for (i=1; i<obj->nverts; i++)
	{
	if (obj->vert[i][0] < minx) minx = obj->vert[i][0];
	else if (obj->vert[i][0] > maxx) maxx = obj->vert[i][0];
	if (obj->vert[i][1] < miny) miny = obj->vert[i][1];
	else if (obj->vert[i][1] > maxy) maxy = obj->vert[i][1];
	if (obj->vert[i][2] < minz) minz = obj->vert[i][2];
	else if (obj->vert[i][2] > maxz) maxz = obj->vert[i][2];
	}
 corner0[0] = minx;
 corner0[1] = miny;
 corner0[2] = minz;
 corner1[0] = maxx;
 corner1[1] = maxy;
 corner1[2] = maxz;
}

wfObject * wfCopyObjectGeometry(wfObject *obj)
{
 wfObject *newobj = (wfObject *) wfAlloc(sizeof(wfObject));
 newobj->nverts = obj->nverts;
 newobj->vert = (wfVertex *) wfAlloc(obj->nverts * sizeof(wfVertex));
 memcpy(newobj->vert,obj->vert,obj->nverts * sizeof(wfVertex));
 newobj->nnorms = obj->nnorms;
 newobj->norm = (wfNormal *) wfAlloc(obj->nnorms * sizeof(wfNormal));
 memcpy(newobj->norm,obj->norm,obj->nnorms * sizeof(wfNormal));
 newobj->ntexcoords = obj->ntexcoords;
 newobj->texc = obj->texc;
 newobj->parts = obj->parts;
 newobj->materials = obj->materials;
 newobj->texmaps = obj->texmaps;
 newobj->gl_initted = obj->gl_initted;
 newobj->app_data = obj->app_data;
 return newobj;
}
