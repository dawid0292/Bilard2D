#include <stdio.h>
#include <strings.h>
#include <math.h>
#include <getopt.h>
#include "wave.h"

struct _params
	{
	float scaleX,scaleY;
	};

void add_maplib(wfObject *obj,char *libname);
void add_usemap(wfObject *obj,char *mapname);
void add_texverts_perpoly(wfObject *obj,struct _params *);
void add_texverts_plane(wfObject *obj,struct _params *,int uindex,int vindex);
void add_texverts_sphere(wfObject *obj,struct _params *,int up,int left,int front);
void add_texverts_cyl(wfObject *obj,struct _params *,int up,int left,int front);
void add_texverts_cube(wfObject *obj,struct _params *);
void compute_normal(wfFace *face,wfObject *obj,float *norm);

main(int argc,char **argv)
{
 wfObject *obj;
 FILE *outfp;
 float minX,maxX,minZ,maxZ;
 int opt,i;
 char *texname=NULL,*texlib=NULL;
 struct _params param;
 param.scaleX = 1;
 param.scaleY = 1;
 while ((opt = getopt(argc,argv,"t:l:s")) != EOF)
	{
	switch (opt)
		{
		case 't': texname = strdup(optarg); break;
		case 'l': texlib = strdup(optarg); break;
		case 's': param.scaleX = atof(argv[optind++]);
			  param.scaleY = atof(argv[optind++]);
			  break;
		}
	}
 if (optind > argc-3)
	{
	fprintf(stderr,"Usage: %s [options] infile.obj outfile.obj <method>\n",
		argv[0]);
	fprintf(stderr, "   [options] can be:\n"
			"	-t texture-name\n"
			"	-l texture-lib\n"
			"	-s scalex scaley\n");
	fprintf(stderr, "   <method> can be:\n"
			"	perpoly - map image onto each polygon\n"
			"	xy / yx - project along X/Y plane\n"
			"	xz / zx - project along X/Z plane\n"
			"	yz / zy - project along Y/Z plane\n"
			"	cube - project along faces of cube\n"
			"	sphereX - spherical projection (+X is up)\n"
			"	sphereY - spherical projection (+Y is up)\n"
			"	sphereZ - spherical projection (+Z is up)\n"
			"	cylX - cylindrical projection (+X is up)\n"
			"	cylY - cylindrical projection (+Y is up)\n"
			"	cylZ - cylindrical projection (+Z is up)\n");
	exit(1);
	}
 wfEnable(WF_INCLUDE_ALL_PARTS);
 if ((obj = wfReadObject(argv[optind])) == NULL)
	exit(1);
 if (obj->ntexcoords)
	fprintf(stderr,"Warning: existing texture coordinates will be removed\n");
 if (!strcasecmp(argv[optind+2],"perpoly"))
	add_texverts_perpoly(obj,&param);
 else if (!strcasecmp(argv[optind+2],"xy"))
	add_texverts_plane(obj,&param,0,1);
 else if (!strcasecmp(argv[optind+2],"yx"))
	add_texverts_plane(obj,&param,1,0);
 else if (!strcasecmp(argv[optind+2],"xz"))
	add_texverts_plane(obj,&param,0,2);
 else if (!strcasecmp(argv[optind+2],"zx"))
	add_texverts_plane(obj,&param,2,0);
 else if (!strcasecmp(argv[optind+2],"yz"))
	add_texverts_plane(obj,&param,1,2);
 else if (!strcasecmp(argv[optind+2],"zy"))
	add_texverts_plane(obj,&param,2,1);
 else if (!strcasecmp(argv[optind+2],"sphereX"))
	add_texverts_sphere(obj,&param,0,1,2);
 else if (!strcasecmp(argv[optind+2],"sphereY"))
	add_texverts_sphere(obj,&param,1,0,2);
 else if (!strcasecmp(argv[optind+2],"sphereZ"))
	add_texverts_sphere(obj,&param,2,0,1);
 else if (!strcasecmp(argv[optind+2],"cylX"))
	add_texverts_cyl(obj,&param,0,1,2);
 else if (!strcasecmp(argv[optind+2],"cylY"))
	add_texverts_cyl(obj,&param,1,0,2);
 else if (!strcasecmp(argv[optind+2],"cylZ"))
	add_texverts_cyl(obj,&param,2,0,1);
 else if (!strcasecmp(argv[optind+2],"cube"))
	add_texverts_cube(obj,&param);
 else
	{
	fprintf(stderr,"Unknown mapping method \"%s\"\n",argv[optind+2]);
	exit(1);
	}
 if ((param.scaleX != 1) || (param.scaleY != 1))
	for (i=0; i < obj->ntexcoords; i++)
		{
		obj->texc[i][0] *= param.scaleX;
		obj->texc[i][1] *= param.scaleY;
		}
 if (texlib)
	add_maplib(obj,texlib);
 if (texname)
	add_usemap(obj,texname);
 if (!strcmp(argv[optind+1],"-"))
	outfp = stdout;
 else if (!(outfp = fopen(argv[optind+1],"w")))
	{
	perror(argv[optind]);
	exit(1);
	}
 wfWriteObject(outfp,obj);
 fclose(outfp);
}

void add_maplib(wfObject *obj,char *libname)
{
 wfPart *p,*prev;
 if (obj->parts)
	for (prev = obj->parts; prev->next; prev = prev->next)
		while ((prev->next) && (prev->next->parttype == WF_TEXTURELIB))
			prev->next = prev->next->next;
 if ((obj->parts) && (obj->parts->parttype == WF_TEXTURELIB))
	obj->parts = obj->parts->next;
 p = (wfPart *) malloc(sizeof(wfPart));
 p->parttype = WF_TEXTURELIB;
 p->part.textureLib = (wfTextureLib *) malloc(sizeof(wfTextureLib));
 strcpy(p->part.textureLib->name,libname);
 p->next = obj->parts;
 obj->parts = p;
}

void add_usemap(wfObject *obj,char *mapname)
{
 wfPart *p,*prev;
 if (obj->parts)
	for (prev = obj->parts; prev->next; prev = prev->next)
		while ((prev->next) && (prev->next->parttype == WF_TEXTURE))
			prev->next = prev->next->next;
 if ((obj->parts) && (obj->parts->parttype == WF_TEXTURE))
	obj->parts = obj->parts->next;
 p = (wfPart *) malloc(sizeof(wfPart));
 p->parttype = WF_TEXTURE;
 p->part.texture = (wfTexmap *) malloc(sizeof(wfTexmap));
 strcpy(p->part.texture->name,mapname);
 for (prev = obj->parts; prev; prev = prev->next)
	if (prev->parttype == WF_TEXTURELIB)
		break;
 if (prev)
	{
	p->next = prev->next;
	prev->next = p;
	}
 else
	{
	p->next = obj->parts;
	obj->parts = p;
	}
}

void add_texverts_perpoly(wfObject *obj,struct _params *param)
{
 wfPart *p;
 int warned=0;
 obj->ntexcoords = 4;
 obj->texc = (wfTexcoord *) malloc(4 * sizeof(wfTexcoord));
 obj->texc[0][0] = 0;		obj->texc[0][1] = 0;
 obj->texc[1][0] = 1;		obj->texc[1][1] = 0;
 obj->texc[2][0] = 1;		obj->texc[2][1] = 1;
 obj->texc[3][0] = 0;		obj->texc[3][1] = 1;
 for (p=obj->parts; p; p=p->next)
    if (p->parttype == WF_FACE)
	{
	wfFace *face;
	int i;
	face = &(p->part.face);
	if ((face->nverts != 4) && (!warned))
		{
		fprintf(stderr,"Warning: Applying 'perpoly' mapping to face(s)"
			" with other than 4 sides\n");
		warned = 1;
		}
	for (i=0; i<face->nverts; i++)
		face->tex[i] = i%4 + 1;
	}
}

void add_texverts_plane(wfObject *obj,struct _params *param,int uindex,int vindex)
{
 wfPart *p;
 wfVertex min,max;
 int i;
 float usize,vsize;
 wfGetBoundingBox(obj,min,max);
 usize = max[uindex] - min[uindex];
 if (usize < .000001) usize = 1;
 vsize = max[vindex] - min[vindex];
 if (vsize < .000001) vsize = 1;
 obj->ntexcoords = obj->nverts;
 obj->texc = (wfTexcoord *) malloc(obj->ntexcoords * sizeof(wfTexcoord));
 for (i=0; i < obj->ntexcoords; i++)
	{
	obj->texc[i][0] = (obj->vert[i][uindex] - min[uindex]) / usize;
	obj->texc[i][1] = (obj->vert[i][vindex] - min[vindex]) / vsize;
	}
 for (p=obj->parts; p; p=p->next)
    if (p->parttype == WF_FACE)
	{
	wfFace *face;
	int i;
	face = &(p->part.face);
	for (i=0; i<face->nverts; i++)
		face->tex[i] = face->vert[i];
	}
}

void add_texverts_sphere(wfObject *obj,struct _params *param,int up,int left,int front)
{
 wfPart *p;
 wfVertex center;
 int i;
 float radius,d[3],len;
 wfGetBoundingSphere(obj,center,&radius);
 obj->ntexcoords = obj->nverts;
 obj->texc = (wfTexcoord *) malloc(obj->ntexcoords * sizeof(wfTexcoord));
 for (i=0; i < obj->ntexcoords; i++)
	{
	d[0] = obj->vert[i][0] - center[0];
	d[1] = obj->vert[i][1] - center[1];
	d[2] = obj->vert[i][2] - center[2];
	len = fsqrt(d[0]*d[0] + d[1]*d[1] + d[2]*d[2]);
	if (len < .000001)
		{ d[0] = 0; d[1] = 1; d[2] = 0; }
	else
		{ d[0] /= len;  d[1] /= len;  d[2] /= len; }
	obj->texc[i][0] = (fatan2(d[front],d[left]) + M_PI) / (2.0f * M_PI);
	obj->texc[i][1] = (fasin(d[up]) + M_PI_2) / M_PI;
	}
 for (p=obj->parts; p; p=p->next)
    if (p->parttype == WF_FACE)
	{
	wfFace *face;
	int i;
	face = &(p->part.face);
	for (i=0; i<face->nverts; i++)
		face->tex[i] = face->vert[i];
	}
}

void add_texverts_cyl(wfObject *obj,struct _params *param,int up,int left,int front)
{
 wfPart *p;
 wfVertex center,min,max;
 int i;
 float radius,d[3],len,vsize;
 wfGetBoundingSphere(obj,center,&radius);
 wfGetBoundingBox(obj,min,max);
 vsize = max[up] - min[up];
 if (vsize < .000001) vsize = 1;
 obj->ntexcoords = obj->nverts;
 obj->texc = (wfTexcoord *) malloc(obj->ntexcoords * sizeof(wfTexcoord));
 for (i=0; i < obj->ntexcoords; i++)
	{
	d[0] = obj->vert[i][0] - center[0];
	d[1] = obj->vert[i][1] - center[1];
	d[2] = obj->vert[i][2] - center[2];
	len = fsqrt(d[0]*d[0] + d[1]*d[1] + d[2]*d[2]);
	if (len < .000001)
		{ d[0] = 0; d[1] = 1; d[2] = 0; }
	else
		{ d[0] /= len;  d[1] /= len;  d[2] /= len; }
	obj->texc[i][0] = (fatan2(d[front],d[left]) + M_PI) / (2.0f * M_PI);
	obj->texc[i][1] = (obj->vert[i][up] - min[up]) / vsize;
	}
 for (p=obj->parts; p; p=p->next)
    if (p->parttype == WF_FACE)
	{
	wfFace *face;
	int i;
	face = &(p->part.face);
	for (i=0; i<face->nverts; i++)
		face->tex[i] = face->vert[i];
	}
}

void add_texverts_cube(wfObject *obj,struct _params *param)
{
 wfPart *p;
 wfVertex min,max;
 int i,ntexc;
 float xsize,ysize,zsize,size,norm[3];
 wfGetBoundingBox(obj,min,max);
 xsize = max[0] - min[0];
 if (xsize < .000001) xsize = 1;
 size = xsize;
 ysize = max[1] - min[1];
 if (ysize < .000001) ysize = 1;
 if (ysize > size)
	size = ysize;
 zsize = max[2] - min[2];
 if (zsize < .000001) zsize = 1;
 if (zsize > size)
	size = zsize;
 for (p=obj->parts, ntexc=0; p; p=p->next)
    if (p->parttype == WF_FACE)
	ntexc += p->part.face.nverts;
 obj->ntexcoords = ntexc;
 obj->texc = (wfTexcoord *) malloc(obj->ntexcoords * sizeof(wfTexcoord));
 for (p=obj->parts, ntexc=0; p; p=p->next)
    if (p->parttype == WF_FACE)
	{
	wfFace *face;
	int i;
	face = &(p->part.face);
	compute_normal(face,obj,norm);
	norm[0] = fabs(norm[0]);
	norm[1] = fabs(norm[1]);
	norm[2] = fabs(norm[2]);
	if ((norm[0] >= norm[1]) && (norm[0] >= norm[2]))
		{
		for (i=0; i<face->nverts; i++, ntexc++)
			{
			face->tex[i] = ntexc+1;
			obj->texc[ntexc][0] = (obj->vert[face->vert[i]-1][2] -
						min[2]) / size;
			obj->texc[ntexc][1] = (obj->vert[face->vert[i]-1][1] -
						min[1]) / size;
			}
		}
	else if ((norm[1] >= norm[0]) && (norm[1] >= norm[2]))
		{
		for (i=0; i<face->nverts; i++, ntexc++)
			{
			face->tex[i] = ntexc+1;
			obj->texc[ntexc][0] = (obj->vert[face->vert[i]-1][0] -
						min[0]) / size;
			obj->texc[ntexc][1] = (obj->vert[face->vert[i]-1][2] -
						min[2]) / size;
			}
		}
	else
		{
		for (i=0; i<face->nverts; i++, ntexc++)
			{
			face->tex[i] = ntexc+1;
			obj->texc[ntexc][0] = (obj->vert[face->vert[i]-1][0] -
						min[0]) / size;
			obj->texc[ntexc][1] = (obj->vert[face->vert[i]-1][1] -
						min[1]) / size;
			}
		}
	}
}

void compute_normal(wfFace *face,wfObject *obj,float *norm)
{
 float dx2,dy2,dz2,dx3,dy3,dz3;
 float x1,y1,z1,x2,y2,z2,x3,y3,z3;
 x1 = obj->vert[face->vert[1]-1][0];
 y1 = obj->vert[face->vert[1]-1][1];
 z1 = obj->vert[face->vert[1]-1][2];
 x2 = obj->vert[face->vert[0]-1][0];
 y2 = obj->vert[face->vert[0]-1][1];
 z2 = obj->vert[face->vert[0]-1][2];
 x3 = obj->vert[face->vert[2]-1][0];
 y3 = obj->vert[face->vert[2]-1][1];
 z3 = obj->vert[face->vert[2]-1][2];
 dx2 = x2 - x1; dy2 = y2 - y1; dz2 = z2 - z1;
 dx3 = x3 - x1; dy3 = y3 - y1; dz3 = z3 - z1;
 norm[0] = dy2*dz3 - dy3*dz2;
 norm[1] = dz2*dx3 - dz3*dx2;
 norm[2] = dx2*dy3 - dx3*dy2;
}
