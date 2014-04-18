#include <stdio.h>
#include <math.h>
#include <string.h>
#include "wave.h"

char *wfVersionString="libwave - Wavefront/GL rendering library, Version 1.4.1";

static wfObject *init_structure(char *fname);
static int line_type(char *line);
static void process_line(char *line,wfObject *obj,int *curv,int *curn,
			int *curt);
static void get_vertex(wfObject *obj,int *curv);
static void get_normal(wfObject *obj,int *curn);
static void get_texcoord(wfObject *obj,int *curt);
static void get_face(char *line,wfObject *obj,int curv,int curn,int curt);
static void get_line(char *line,wfObject *obj);
static void get_mtl_lib(wfObject *obj);
static void get_mtl(wfObject *obj);
static void get_map_lib(wfObject *obj);
static void get_map(wfObject *obj);
static void get_unsupported(wfObject *obj,char *line);
static void append_part(wfPart *p,wfObject *o);

#define whitespace " \t"

static int current_line;
static int firstPass=1;

/* Types of lines that may be read from object file */
#define VERTEX 1
#define NORMAL 2
#define TEXTURE 3
#define FACE 4
#define MTLLIB 5
#define USEMTL 6
#define MAPLIB 7
#define USEMAP 8
#define LINE 9
#define COMMENT 99
#define UNSUPPORTED 999

/* wfReadLine - reads a line from fp, treating '\' at the end of a line
	as a continuation character */
char * wfReadLine(char *buf,int bufsize,FILE *fp,int *lineNum)
{
 int len;
 if (!fgets(buf,bufsize,fp)) return NULL;
 if (lineNum)
	(*lineNum)++;
 len = strlen(buf)-1;
 if (len <= 0)
	return buf;
 while ((len<bufsize) && (buf[len-1] == '\\'))
	{
	if (!fgets(buf+len-1,bufsize-len,fp)) break;
	if (lineNum)
		(*lineNum)++;
	len = strlen(buf)-1;
	}
 return buf;
}

/* wfReadObject - reads a Wavefront object file and returns a pointer to an
	wfObject structure containing the objects definition. Returns NULL
	if it cannot read the file. */
wfObject *wfReadObject(char *fname)
{
 wfObject *obj;
 FILE *fp;
 char line[1024];
 int curv,curn,curt;
/* Create the struct, and count vertices, tex vertices, & normals */
 obj = init_structure(fname);
 if (!obj) return(NULL);
 if ((fp = fopen(fname,"r")) == NULL)
	{
	perror(fname);
	return(NULL);
	}
 current_line = 0;
 firstPass = 0;
 curv = curn = curt = 0;   /* Next vertex, next normal, & next tex vertex */
/* Read each line & add the appropriate data to the object struct */
 while (wfReadLine(line,1024,fp,&current_line))
	{
	line[strlen(line)-1] = '\0'; /* get rid of newline */
	process_line(line,obj,&curv,&curn,&curt);
	}
 fclose(fp);
 return(obj);
}

/* process_line - Determines what 'command' a line contains and adds
	the info to the object */
static void process_line(char *line,wfObject *obj,int *curv,int *curn,
			int *curt)
{
 int t;
 char linecopy[1024];
/* Save a copy of the line for get_face et al, before line_type() strtok's it */
 strcpy(linecopy,line);
 t = line_type(line);
 switch (t) {
	case VERTEX: get_vertex(obj,curv); break;
	case NORMAL: get_normal(obj,curn); break;
	case TEXTURE: get_texcoord(obj,curt); break;
	case FACE: get_face(linecopy,obj,*curv,*curn,*curt); break;
	case LINE: get_line(linecopy,obj); break;
	case MTLLIB: get_mtl_lib(obj); break;
	case USEMTL: get_mtl(obj); break;
	case MAPLIB: get_map_lib(obj); break;
	case USEMAP: get_map(obj); break;
	case UNSUPPORTED: if (obj->allParts)
				get_unsupported(obj,linecopy);
			  break;
	}
}

/* get_vertex - processes a 'v' line; gets the coordinates of the next
	vertex */
static void get_vertex(wfObject *obj,int *curv)
{
 char *v;
 if (*curv >= obj->nverts) {
	fprintf(stderr,"ERROR: read_object: bad vertex count at line %d"
		" - initial count was %d\n",current_line,obj->nverts);
	exit(1);
	}
 v = strtok(NULL,whitespace);
 if (v) obj->vert[*curv][0] = atof(v);
 else obj->vert[*curv][0] = 0;
 v = strtok(NULL,whitespace);
 if (v) obj->vert[*curv][1] = atof(v);
 else obj->vert[*curv][1] = 0;
 v = strtok(NULL,whitespace);
 if (v) obj->vert[*curv][2] = atof(v);
 else obj->vert[*curv][2] = 0;
/*
 if (obj->allParts)
	{
	wfPart *p = wfAlloc(sizeof(wfPart));
	if (p)
		{
		p->parttype = WF_VERTEX;
		p->part.vertex = &obj->vert[*curv];
		append_part(p,obj);
		}
	}
*/
 (*curv)++;
}

/* get_normal - processes a 'vn' line; gets the next vertex normal */
static void get_normal(wfObject *obj,int *curn)
{
 char *v;
 if (*curn > obj->nnorms) {
	fprintf(stderr,"ERROR: read_object: bad normal count at line %d"
		" - initial count was %d\n",current_line,obj->nnorms);
	exit();
	}
 v = strtok(NULL,whitespace);
 if (v) obj->norm[*curn][0] = atof(v);
 else obj->norm[*curn][0] = 0;
 v = strtok(NULL,whitespace);
 if (v) obj->norm[*curn][1] = atof(v);
 else obj->norm[*curn][1] = 0;
 v = strtok(NULL,whitespace);
 if (v) obj->norm[*curn][2] = atof(v);
 else obj->norm[*curn][2] = 0;
/*
 if (obj->allParts)
	{
	wfPart *p = wfAlloc(sizeof(wfPart));
	if (p)
		{
		p->parttype = WF_NORMAL;
		p->part.normal = &obj->norm[*curn];
		append_part(p,obj);
		}
	}
*/
 (*curn)++;
}

/* get_texcoord - processes a 'vt' line; gets the next texture vertex */
static void get_texcoord(wfObject *obj,int *curt)
{
 char *v;
 if (*curt > obj->ntexcoords) {
	fprintf(stderr,"ERROR: read_object: bad tex vertex count at line %d"
		" - initial count was %d\n",current_line,obj->ntexcoords);
	exit();
	}
 v = strtok(NULL,whitespace);
 if (v) obj->texc[*curt][0] = atof(v);
 else obj->texc[*curt][0] = 0;
 v = strtok(NULL,whitespace);
 if (v) obj->texc[*curt][1] = atof(v);
 else obj->texc[*curt][1] = 0;
 v = strtok(NULL,whitespace);
 if (v) obj->texc[*curt][2] = atof(v);
 else obj->texc[*curt][2] = 0;
/*
 if (obj->allParts)
	{
	wfPart *p = wfAlloc(sizeof(wfPart));
	if (p)
		{
		p->parttype = WF_TEXCOORD;
		p->part.texcoord = &obj->texc[*curt];
		append_part(p,obj);
		}
	}
*/
 (*curt)++;
}

/* get_face - processes a 'f' or 'fo' line. Creates a new face struct &
	adds the indices of the vertices, texture vertices, and vertex
	normals. */
static void get_face(char *line,wfObject *obj,int curv,int curn,int curt)
{
 wfPart *p;
 int nverts=0,i=0,v,vt,vn;
 char *s,*r;
/* Count the number of vertices on the line */
 while (strtok(NULL,whitespace)) nverts++;
/* Create the struct */
 p = wfAlloc(sizeof(wfPart));
 if (!p) return;
 p->parttype = WF_FACE;
 p->part.face.nverts = nverts;
 p->part.face.vert = wfAlloc(nverts * sizeof(int));
 if (!p->part.face.vert) return;
 p->part.face.tex = wfAlloc(nverts * sizeof(int));
 if (!p->part.face.tex) return;
 p->part.face.norm = wfAlloc(nverts * sizeof(int));
 if (!p->part.face.norm) return;
 s = strtok(line,whitespace);
/* Add each vertex */
 while (s = strtok(NULL,whitespace))
	{
	v = atoi(s);
	if (v>=0) p->part.face.vert[i] = v;
	else p->part.face.vert[i] = curv+1 + v;
  /* Find the vertex texture after the first '/' */
	r = s;
	while ((*r) && (*r != '/')) r++;
	if (*r)
		{
		r++;
		vt = atoi(r);
		if (vt >= 0) p->part.face.tex[i] = vt;
		else p->part.face.tex[i] = curt+1 + vt;
		}
	else p->part.face.tex[i] = 0;
  /* Find the vertex normal after the second '/' */
	while ((*r) && (*r != '/')) r++;
	if (*r)
		{
		r++;
		vn = atoi(r);
		if (vn >= 0) p->part.face.norm[i] = vn;
		else p->part.face.norm[i] = curn+1 + vn;
		}
	else p->part.face.norm[i] = 0;
	i++;
	}
 append_part(p,obj);
}

/* get_line - processes a 'l' line. Creates a new line struct &
	adds the indices of the vertices. */
static void get_line(char *line,wfObject *obj)
{
 wfPart *p;
 int nverts=0,i=0,v;
 char *s,*r;
/* Count the number of vertices on the line */
 while (strtok(NULL,whitespace)) nverts++;
/* Create the struct */
 p = wfAlloc(sizeof(wfPart));
 if (!p) return;
 p->parttype = WF_LINE;
 p->part.line.nverts = nverts;
 p->part.line.vert = wfAlloc(nverts * sizeof(int));
 if (!p->part.line.vert) return;
 s = strtok(line,whitespace);
/* Add each vertex */
 while (s = strtok(NULL,whitespace))
	{
	p->part.line.vert[i] = atoi(s);
	i++;
	}
 append_part(p,obj);
}

/* get_mtl_lib - processes a 'mtllib' line; reads the material library */
static void get_mtl_lib(wfObject *obj)
{
 char *fname;
 wfPart *p;
 fname = strtok(NULL,whitespace);
 if (!fname) return;
 p = wfAlloc(sizeof(wfPart));
 if (!p) return;
 p->parttype = WF_MATERIALLIB;
 p->part.materialLib = wfAlloc(sizeof(wfMaterialLib));
 if (!p->part.materialLib) return;
 strcpy(p->part.materialLib->name,fname);
 append_part(p,obj);
/* if (obj->materials) wfFreeMaterialList(obj->materials); */
 obj->materials = wfReadMtllib(fname);
}

/* get_mtl - processes a 'usemtl' line */
static void get_mtl(wfObject *obj)
{
 char *name;
 wfMaterial *m;
 wfPart *p;
 p = wfAlloc(sizeof(wfPart));
 if (!p) return;
 p->parttype = WF_MATERIAL;
 p->part.material = NULL;
 name = strtok(NULL,whitespace);
 if (name)
    {
    for (m=obj->materials; m; m=m->next)
	if (!strcmp(name,m->name))
		{
		p->part.material = m;
		m->used = 1;
		append_part(p,obj);
		return;
		}
    fprintf(stderr,"Error: unknown material \"%s\" at line %d\n",name,
		current_line);
    }
}

/* get_map_lib - processes a 'maplib' line; reads the texmap library */
static void get_map_lib(wfObject *obj)
{
 char *fname;
 wfPart *p;
 fname = strtok(NULL,whitespace);
 if (!fname) return;
 p = wfAlloc(sizeof(wfPart));
 if (!p) return;
 p->parttype = WF_TEXTURELIB;
 p->part.textureLib = wfAlloc(sizeof(wfTextureLib));
 if (!p->part.textureLib) return;
 strcpy(p->part.textureLib->name,fname);
 append_part(p,obj);
/* if (obj->texmaps) wfFreeTexmapList(obj->texmaps); */
 obj->texmaps = wfReadMaplib(fname);
}

/* get_map - processes a 'usemap' line */
static void get_map(wfObject *obj)
{
 char *name;
 wfTexmap *m=NULL;
 wfPart *p;
 name = strtok(NULL,whitespace);
 if ((!name) || (!strcmp(name,"off"))) m = wfOffTexmap();
 else
	{
	for (m=obj->texmaps; m; m=m->next)
		if (!strcmp(name,m->name)) break;
	}
 if (!m)
	{
	fprintf(stderr,"Error: unknown texmap \"%s\" at line %d\n",name,
		current_line);
	return;
	}
 p = wfAlloc(sizeof(wfPart));
 if (!p) return;
 p->parttype = WF_TEXTURE;
 p->part.texture = m;
 m->used = 1;
 append_part(p,obj);
}

/* get_unsupported - saves a line for an unsupported feature */
static void get_unsupported(wfObject *obj,char *line)
{
 wfPart *p;
 p = wfAlloc(sizeof(wfPart));
 if (!p) return;
 p->parttype = WF_UNSUPPORTED;
 p->part.unsupported = strdup(line);
 append_part(p,obj);
}

/* append_part - adds a new part to the end of the object */
static void append_part(wfPart *p,wfObject *o)
{
 wfPart *prev;
 p->next = NULL;
 if (o->parts == NULL) o->parts = p;
 else {
	prev = o->parts;
	while (prev->next) prev = prev->next;
	prev->next = p;
	}
}

/* init_structure - initializes the object struct. Counts the number
	of vertices, normals, & texture vertices in the file, and allocates
	the necessary space. */
static wfObject *init_structure(char *fname)
{
 FILE *fp;
 wfObject *obj;
 char line[1024];
/* Count vertices, normals, etc. */
 if ((fp = fopen(fname,"r")) == NULL)
	{
	perror(fname);
	return(NULL);
	}
 obj = wfAlloc(sizeof(wfObject));
 if (!obj) return NULL;
 bzero(obj,sizeof(wfObject));
 obj->allParts = wfIsEnabled(WF_INCLUDE_ALL_PARTS);
 obj->nverts = obj->nnorms = obj->ntexcoords = 0;
 while (wfReadLine(line,1024,fp,NULL))
	{
	int t;
	line[strlen(line)-1] = '\0'; /* get rid of newline */
	t = line_type(line);
	if (t==VERTEX) obj->nverts++;
	else if (t==NORMAL) obj->nnorms++;
	else if (t==TEXTURE) obj->ntexcoords++;
	}
 fclose(fp);
/* Allocate the vertex & normal arrays */
 if (obj->nverts)
	{
	obj->vert = wfAlloc(obj->nverts * sizeof(wfVertex));
	if (!obj->vert) return NULL;
	}
 if (obj->nnorms)
	{
	obj->norm = wfAlloc(obj->nnorms * sizeof(wfNormal));
	if (!obj->norm) return NULL;
	}
 if (obj->ntexcoords)
	{
	obj->texc = wfAlloc(obj->ntexcoords * sizeof(wfTexcoord));
	if (!obj->texc) return NULL;
	}
 obj->parts = NULL;
 obj->materials = NULL;
 obj->texmaps = NULL;
 obj->gl_initted = 0;
 return(obj);
}

/* line_type - determines the type of 'command' a line contains. Calls
	strtok() to get the first string on the line, and to prepare it
	for parsing by other routines. */
static int line_type(char *line)
{
 char *cmd;
 cmd = strtok(line,whitespace);
 if (!cmd) return(COMMENT);
 if (!strcmp(cmd,"v")) return(VERTEX);
 else if (!strcmp(cmd,"vn")) return(NORMAL);
 else if (!strcmp(cmd,"vt")) return(TEXTURE);
 else if (!strcmp(cmd,"l")) return(LINE);
 else if (!strcmp(cmd,"f")) return(FACE);
 else if (!strcmp(cmd,"fo")) return(FACE);
 else if (!strcmp(cmd,"mtllib")) return(MTLLIB);
 else if (!strcmp(cmd,"usemtl")) return(USEMTL);
 else if (!strcmp(cmd,"maplib")) return(MAPLIB);
 else if (!strcmp(cmd,"usemap")) return(USEMAP);
 else if (!strcmp(cmd,"#")) return(COMMENT);
 return(UNSUPPORTED);
}
