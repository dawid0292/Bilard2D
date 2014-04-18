#ifndef _wave_h_
#define _wave_h_

/*
   libwave - Wavefront/GL rendering library
   Version 1.4.1
   12 January 1996
   Dave Pape (pape@evl.eecs.uic.edu)

 Changes:
	1.4.1:	Corrected scaling option for .map files - uses -s, not -mm
		Added -o (offset) support for .map files
	1.4:	Added WF_DRAW_WIREFRAME option.
		Added wfCopyObjectGeometry(), wfTranslateObject(), wfRotateObject(),
		  wfScaleObject(), wfDeformObject(), wfComputeNormals(),
		  wfRayIntersection(), wfRayHits().
	1.3.3:  Fixed really subtle bug that only showed up with certain big files
	1.3.2:	Added wfEnable() & wfDisable(), with options WF_SHOW_DEFINED_TEXTURES,
			WF_USE_TEXTURES, WF_INCLUDE_ALL_PARTS
	1.3.1:	Added app_data element to wfObject
		Corrected maplib & usemap output of wfWriteObject()
		Optimized texture definition (a given image will only
			be texdef'ed once)
	1.3:	Added wfMaterialLib & wfTextureLib 'parts'
		Added wfWriteObject()
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <malloc.h>

#define WF_MAX_TEXTURES 1024

/* Values for wfPart.parttype */
#define WF_FACE		1
#define WF_MATERIAL	2
#define WF_TEXTURE	3
#define WF_LINE		4
#define WF_MATERIALLIB	5
#define WF_TEXTURELIB	6
#define WF_VERTEX	7
#define WF_NORMAL	8
#define WF_TEXCOORD	9
#define WF_UNSUPPORTED	10

/* Arguments for wfEnable()/wfDisable() */
#define WF_SHOW_DEFINED_TEXTURES	0x10001
#define WF_USE_TEXTURES			0x10002
#define WF_INCLUDE_ALL_PARTS		0x10003
#define WF_DRAW_WIREFRAME		0x10004

typedef float wfVertex[3];
typedef float wfNormal[3];
typedef float wfTexcoord[3];
typedef struct _wf_face wfFace;
typedef struct _wf_line wfLine;
typedef struct _wf_material wfMaterial;
typedef struct _wf_texmap wfTexmap;
typedef struct _wf_mtllib wfMaterialLib;
typedef struct _wf_texlib wfTextureLib;
typedef struct _wf_part wfPart;
typedef struct _wf_object wfObject;

struct _wf_face {
		int nverts;
		int *vert;
		int *tex;
		int *norm;
		};

struct _wf_line {
		int nverts;
		int *vert;
		};

struct _wf_material {
		char name[256];
		int illum;
		float ambient[3];
		float diffuse[3];
		float specular[3];
		float specindex;
		float dissolve;
		int material_number;
		int used;
		wfMaterial *next;
		};

struct _wf_texmap {
		char name[256];
		char Ka_file[256];
		float Ka_s[3];
		float Ka_o[3];
		char Kd_file[256];
		float Kd_s[3];
		float Kd_o[3];
		char Ks_file[256];
		float Ks_s[3];
		float Ks_o[3];
		char bump_file[256];
		float bump_s[3];
		float bump_o[3];
		float bump_bm;
		char d_file[256];
		float d_s[3];
		float d_o[3];
		int used;
		int texmap_number;
		wfTexmap *next;
		};

struct _wf_mtllib {
		char name[256];
		};

struct _wf_texlib {
		char name[256];
		};


struct _wf_part {
		int parttype;
		union {
			wfFace face;
			wfMaterial *material;
			wfTexmap *texture;
			wfLine line;
			wfMaterialLib *materialLib;
			wfTextureLib *textureLib;
			char *unsupported;
			} part;
		wfPart *next;
		};

struct _wf_object {
		int nverts;
		wfVertex *vert;
		int nnorms;
		wfNormal *norm;
		int ntexcoords;
		wfTexcoord *texc;
		wfPart *parts;
		wfMaterial *materials;
		wfTexmap *texmaps;
		int gl_initted;
		void *app_data;
		int allParts;	/* are verts etc. include in 'parts' */
		};


extern wfObject *wfReadObject(char *fname);
extern void wfDrawObject(wfObject *obj);
extern void wfInitObject(wfObject *obj);

extern void wfWriteObject(FILE *fp,wfObject *obj);
extern void wfGetBoundingSphere(wfObject *obj,wfVertex center,float *radius);
extern void wfGetBoundingBox(wfObject *obj,wfVertex corner0,wfVertex corner1);
extern void wfSetMemoryAllocator(void * (*allocfn)(size_t),void (*freefn)(void *));
extern void wfSetNewIDFunction(int (*idfn)(void));
extern int wfNewID(void);
extern void wfEnable(int);
extern void wfDisable(int);
extern int wfIsEnabled(int);
extern void wfTranslateObject(wfObject *obj,float tx,float ty,float tz);
extern void wfScaleObject(wfObject *obj,float sx,float sy,float sz);
extern void wfRotateObject(wfObject *obj,float angle,float x,float y,float z);
extern void wfDeformObject(wfObject *obj,void (*vertFunc)(float *),void (*normFunc)(float *));
extern wfObject * wfCopyObjectGeometry(wfObject *obj);
extern void wfComputeNormals(wfObject *obj,int smooth,int clockwise);
extern wfFace *wfRayIntersection(wfObject *obj,float *ray0,float *raydelta,float *ret_t);
extern int wfRayHits(wfObject *obj,float *ray0,float *raydelta);


/* Internally used functions */
extern wfMaterial *wfReadMtllib(char *fname);
extern void wfFreeMaterialList(wfMaterial *m);
extern wfTexmap *wfReadMaplib(char *fname);
extern void wfFreeTexmapList(wfTexmap *m);
extern wfTexmap *wfOffTexmap(void);
extern char *wfReadLine(char *buf,int bufsize,FILE *fp,int *lineNum);
extern void *wfAlloc(size_t n);
extern void wfFree(void *p);

#ifdef __cplusplus
}
#endif

#endif
