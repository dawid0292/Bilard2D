#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GL/gl.h>
#include <gl/image.h>
#include "wave.h"

static void define_materials(wfObject *obj);
static void create_material_illum0(wfMaterial *m);
static void create_material_illum1(wfMaterial *m);
static void create_material_illum2(wfMaterial *m);
static void create_material_illum3(wfMaterial *m);
static void define_texmaps(wfObject *obj);
static int get_teximage(wfTexmap *m,unsigned long **image,int *xdim,
			int *ydim);
static int get_rgba_teximage(wfTexmap *m,unsigned long **image,int *xdim,
				int *ydim);
static int get_alpha_teximage(wfTexmap *m,unsigned long **image,int *xdim,
				int *ydim);
static int read_sgi_image_long(char *fname,unsigned long **data,int *xdim,
                                int *ydim);
static int read_sgi_image_byte(char *fname,unsigned long **data,int *xdim,
                                int *ydim);

/* wfInitObject - initializes GL stuff for an object (material defs, etc.) */
void wfInitObject(wfObject *obj)
{
 define_materials(obj);
 if (wfIsEnabled(WF_USE_TEXTURES))
	define_texmaps(obj);
 obj->gl_initted = 1;
}

/* define_materials - creates GL definitions for materials in obj's
	material list */
static void define_materials(wfObject *obj)
{
 wfMaterial *m;
 m = obj->materials;
 while (m)
     {
     if (m->used)
	switch (m->illum)
	   {
	   case 0: create_material_illum0(m); break;
	   case 1: create_material_illum1(m); break;
	   case 2: create_material_illum2(m); break;
	   case 4: case 5: case 6: case 7:
	   case 3: create_material_illum3(m); break;
	   default: fprintf(stderr,
			"Warning: unsupported illumination model (%d)\n",
			m->illum);
		    break;
	   }
     m = m->next;
     }
}

/* create_material_illum0 - lmdef's an illumination model 0 material
	(flat color; uses EMISSION w/ 0.0 AMBIENT & DIFFUSE to get the
	 right result) */
static void create_material_illum0(wfMaterial *m)
{
 float props[4];
 glNewList(m->material_number,GL_COMPILE);
 props[0] = m->diffuse[0];
 props[1] = m->diffuse[1];
 props[2] = m->diffuse[2];
 props[3] = m->dissolve;
 glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,props);
 props[0] = 0.0;
 props[1] = 0.0;
 props[2] = 0.0;
 props[3] = m->dissolve;
 glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,props);
 glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,props);
 glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,0.0);
 glEndList();
}

/* create_material_illum1 - lmdef's an illumination model 1 material
	(ambient & diffuse) */
static void create_material_illum1(wfMaterial *m)
{
 float props[4];
 glNewList(m->material_number,GL_COMPILE);
 props[0] = m->ambient[0];
 props[1] = m->ambient[1];
 props[2] = m->ambient[2];
 props[3] = m->dissolve;
 glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,props);
 props[0] = m->diffuse[0];
 props[1] = m->diffuse[1];
 props[2] = m->diffuse[2];
 props[3] = m->dissolve;
 glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,props);
 props[0] = props[1] = props[2] = props[3] = 0.0;
 glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,props);
 glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,0.0);
 glEndList();
}

/* create_material_illum2 - lmdef's an illumination model 2 material
	(ambient, diffuse, & specular) */
static void create_material_illum2(wfMaterial *m)
{
 float props[4],ns;
 glNewList(m->material_number,GL_COMPILE);
 props[0] = m->ambient[0];
 props[1] = m->ambient[1];
 props[2] = m->ambient[2];
 props[3] = m->dissolve;
 glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,props);
 props[0] = m->diffuse[0];
 props[1] = m->diffuse[1];
 props[2] = m->diffuse[2];
 props[3] = m->dissolve;
 glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,props);
 props[0] = m->specular[0];
 props[1] = m->specular[1];
 props[2] = m->specular[2];
 props[3] = m->dissolve;
 glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,props);
 ns = m->specindex;
 if (ns>128.0) ns = 128.0;
 glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,ns);
 glEndList();
}

/* create_material_illum3 - lmdef's an illumination model 3 material
	(ambient, diffuse, & specular, with reflection (not supported,
	 of course)) */
static void create_material_illum3(wfMaterial *m)
{
 fprintf(stderr,"Warning: reflection is not supported - using illum 2 in place"
	" of illum 3+\n");
 create_material_illum2(m);
}

struct _texrec { char *Kd_file,*d_file; int texid; };

/* define_texmaps - creates GL definitions for textures in obj's texmap list */
static void define_texmaps(wfObject *obj)
{
 static struct _texrec tex_lib[WF_MAX_TEXTURES];
 static int num_tex=0;
 wfTexmap *m;
 unsigned long *image;
 int xdim,ydim,i;
 for (m=obj->texmaps; m; m=m->next)
    if (m->used)
	{
	for (i=0; i < num_tex; i++)
		if (!strcmp(m->Kd_file,tex_lib[i].Kd_file) &&
		    !((m->d_file[0]) && strcmp(m->d_file,tex_lib[i].d_file)))
			{
			m->texmap_number = tex_lib[i].texid;
			break;
			}
	if ((i == num_tex) && (get_teximage(m,&image,&xdim,&ydim)))
		{
#if 0
		if (wfIsEnabled(WF_SHOW_DEFINED_TEXTURES))
			lrectwrite(0,0,xdim-1,ydim-1,image);
#endif
		glNewList(m->texmap_number,GL_COMPILE);
		glTexImage2D(GL_TEXTURE_2D,0,4,xdim,ydim,0,GL_ABGR_EXT,
				GL_UNSIGNED_BYTE,image);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glEndList();
		wfFree(image);
		if (num_tex < WF_MAX_TEXTURES)
			{
			tex_lib[num_tex].Kd_file = strdup(m->Kd_file);
			tex_lib[num_tex].d_file = strdup(m->d_file);
			tex_lib[num_tex].texid = m->texmap_number;
			num_tex++;
			}
		}
	}
}


/* get_teximage - reads in a texture image. The Kd texture is used for
	R, G, & B; the dissolve texture is used for Alpha. If either
	texture is not specified, a default 255 value is used. The images
	should be in SGI format; the extension '.sgi' is tried in place
	of any '.tex' extension.
	The function returns 0 if an error occurred, 1 for success. */
static int get_teximage(wfTexmap *m,unsigned long **image,int *xdim,
			int *ydim)
{
 if (m->Kd_file[0]) 		/* Kd texture specified */
	return(get_rgba_teximage(m,image,xdim,ydim));
 else if (m->d_file[0]) 	/* Dissolve w/o color - use white for color */
	return(get_alpha_teximage(m,image,xdim,ydim));
 else return(0);		/* Probably a bump map */
}

static int get_rgba_teximage(wfTexmap *m,unsigned long **image,int *xdim,
				int *ydim)
{
 if (!read_sgi_image_long(m->Kd_file,image,xdim,ydim))
	{
	char name[256];			/* Try adding .sgi extension */
	strcpy(name,m->Kd_file);
	if (!strcmp(&name[strlen(name)-4],".tex"))
		strcpy(&name[strlen(name)-4],".sgi");
	else strcat(name,".sgi");
	if (!read_sgi_image_long(name,image,xdim,ydim))
		{
		fprintf(stderr,"WF Error: couldn't read texture \"%s\"\n",
			m->Kd_file);
		return 0;
		}
	}
 if (m->d_file[0])	/* Dissolve specified also */
	{
	unsigned long *alpha,*ip,*ap;
	int ax,ay,i;
	if (!get_alpha_teximage(m,&alpha,&ax,&ay)) return 0;
	if ((ax != *xdim) || (ay != *ydim))
		{
		fprintf(stderr,"WF Error: \"%s\" & \"%s\" are different"
			" sizes\n",m->Kd_file,m->d_file);
		wfFree(alpha);
		return 0;
		}
	for (ip= *image, ap=alpha, i=ax*ay; i; i--, ip++, ap++)
		*ip = (*ip & 0x00ffffff) | (*ap & 0xff000000);
	wfFree(alpha);
	}
 return 1;
}

static int get_alpha_teximage(wfTexmap *m,unsigned long **image,int *xdim,
				int *ydim)
{
 if (!read_sgi_image_byte(m->d_file,image,xdim,ydim))
	{
	char name[256];		/* Try adding .sgi extension */
	strcpy(name,m->Kd_file);
	if (!strcmp(&name[strlen(name)-4],".tex"))
		strcpy(&name[strlen(name)-4],".sgi");
	else strcat(name,".sgi");
	if (!read_sgi_image_long(name,image,xdim,ydim))
		{
		fprintf(stderr,"WF Error: couldn't read texture \"%s\"\n",
			m->d_file);
		return 0;
		}
	}
 return 1;
}

static int read_sgi_image_long(char *fname,unsigned long **data,int *xdim,
				int *ydim)
{
 IMAGE *image;
 int x,y;
 short *rbuf,*gbuf,*bbuf;
 unsigned long *p;
 int zdim;
 if ((image=iopen(fname,"r")) == NULL ) return(0);
 *xdim = image->xsize;
 *ydim = image->ysize;
 zdim = image->zsize;
 rbuf = (short *) wfAlloc(image->xsize*sizeof(short));
 if (!rbuf) return 0;
 if (zdim>1)
	{
	gbuf = (short *) wfAlloc(image->xsize*sizeof(short));
	if (!gbuf) return 0;
	}
 else gbuf = rbuf;
 if (zdim>2)
	{
	bbuf = (short *) wfAlloc(image->xsize*sizeof(short));
	if (!bbuf) return 0;
	}
 else bbuf = gbuf;
 p = *data = wfAlloc(image->xsize*image->ysize*sizeof(unsigned long));
 if (!p) return 0;
 for (y=0; y<image->ysize; y++)
	{
        getrow(image,rbuf,y,0);
        if (zdim > 1) getrow(image,gbuf,y,1);
        if (zdim > 2) getrow(image,bbuf,y,2);
        for (x=0; x<image->xsize; x++)
                *p++ = rbuf[x] | (gbuf[x]<<8) | (bbuf[x]<<16) | 0xff000000;
        }
 wfFree(rbuf);
 if (zdim>1) wfFree(gbuf);
 if (zdim>2) wfFree(bbuf);
 iclose(image);
 return(1);
}

static int read_sgi_image_byte(char *fname,unsigned long **data,int *xdim,
                                int *ydim)
{
 IMAGE *image;
 int x,y;
 unsigned long *r;
 short *buf;
 if ((image=iopen(fname,"r")) == NULL ) return(0);
 *xdim = image->xsize;
 *ydim = image->ysize;
 buf = (short *) wfAlloc(image->xsize*sizeof(short));
 if (!buf) return 0;
 r = *data = wfAlloc(image->xsize*image->ysize*sizeof(unsigned long));
 if (!r) return 0;
 for (y=0; y<image->ysize; y++)
	{
        getrow(image,buf,y,0);
        for (x=0; x<image->xsize; x++) *r++ = (buf[x] << 24) & 0xff000000;
        }
 wfFree(buf);
 iclose(image);
 return(1);
}
