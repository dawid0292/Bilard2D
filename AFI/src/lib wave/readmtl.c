#include <stdio.h>
#include <math.h>
#include <string.h>
#include "wave.h"

static int line_type(char *line);
static int define_material(wfMaterial **list,FILE *fp);
static wfMaterial *create_mtl_struct(void);
static void get_illum(wfMaterial *m);
static void get_ka(wfMaterial *m);
static void get_kd(wfMaterial *m);
static void get_ks(wfMaterial *m);
static void get_ns(wfMaterial *m);
static void get_dissolve(wfMaterial *m);

static char *whitespace = " \t";

/* Types of lines that may be read from object file */
#define NEWMTL 1
#define KA 2
#define KD 3
#define KS 4
#define NS 5
#define ILLUM 6
#define DISSOLVE 7
#define COMMENT 99
#define UNKNOWN 999

struct _token { char *str; int id; };
#define NUMTOKENS 8
static struct _token token[NUMTOKENS] = {
			{ "newmtl", NEWMTL },
			{ "Ka", KA },
			{ "Kd", KD },
			{ "Ks", KS },
			{ "Ns", NS },
			{ "illum", ILLUM},
			{ "d", DISSOLVE },
			{ "#", COMMENT}
			};

/* wfReadMtllib - reads a Wavefront material file and returns a pointer to a
	list of the materials defined. Returns NULL if it cannot read the
	file. */
wfMaterial *wfReadMtllib(char *fname)
{
 wfMaterial *list;
 FILE *fp;
 char line[1024],fname2[256];
 int t;
 if ((fp = fopen(fname,"r")) == NULL) {
	sprintf(fname2,"%s.mtl",fname);
	if ((fp = fopen(fname2,"r")) == NULL) {
		perror(fname);
		return(NULL);
		}
	}
/* Read lines until first "newmtl" found */
 list = NULL;
 while (wfReadLine(line,1024,fp,NULL)) {
	line[strlen(line)-1] = '\0'; /* get rid of newline */
	t = line_type(line);
	if (t==NEWMTL) break;
	}
/* Add material definitions until EOF reached */
 while (define_material(&list,fp)) ;
 fclose(fp);
 return(list);
}

/* create_mtl_struct - creates a wfMaterial struct & fills it with default
	values. */
static wfMaterial *create_mtl_struct(void)
{
 wfMaterial *m;
 m = wfAlloc(sizeof(wfMaterial));
 if (!m) return NULL;
 bzero(m->name,sizeof(m->name));
 m->illum = 0;
 m->ambient[0] = m->ambient[1] = m->ambient[2] = 0.0;
 m->diffuse[0] = m->diffuse[1] = m->diffuse[2] = 0.0;
 m->specular[0] = m->specular[1] = m->specular[2] = 0.0;
 m->specindex = 0.0;
 m->dissolve = 1.0;
 m->material_number = wfNewID();
 m->used = 0;
 m->next = NULL;
 return(m);
}

/* define_material - creates a new material struct & fills it in. Returns
	1 when the next "newmtl" is reached, or 0 if EOF is reached. */
static int define_material(wfMaterial **list,FILE *fp)
{
 wfMaterial *mtl;
 char *name,line[1024];
 int t;
 mtl = create_mtl_struct();
 if (!mtl) return 0;
 mtl->next = *list;
 *list = mtl;
 name = strtok(NULL,whitespace);
 strncpy(mtl->name,name,sizeof(mtl->name)-1);
 while (wfReadLine(line,1024,fp,NULL)) {
	line[strlen(line)-1] = '\0'; /* get rid of newline */
	t = line_type(line);
	switch (t) {
	   case NEWMTL: return(1);
	   case ILLUM: get_illum(mtl); break;
	   case KA: get_ka(mtl); break;
	   case KD: get_kd(mtl); break;
	   case KS: get_ks(mtl); break;
	   case NS: get_ns(mtl); break;
	   case DISSOLVE: get_dissolve(mtl); break;
	   }
	}
 return(0);
}

/* get_illum - processes a "illum" line */
static void get_illum(wfMaterial *m)
{
 char *s;
 s = strtok(NULL,whitespace);
 m->illum = atoi(s);
}

/* get_ka - processes a "Ka" line */
static void get_ka(wfMaterial *m)
{
 char *s;
 s = strtok(NULL,whitespace);
 m->ambient[0] = atof(s);
 s = strtok(NULL,whitespace);
 m->ambient[1] = atof(s);
 s = strtok(NULL,whitespace);
 m->ambient[2] = atof(s);
}

/* get_kd - processes a "Kd" line */
static void get_kd(wfMaterial *m)
{
 char *s;
 s = strtok(NULL,whitespace);
 m->diffuse[0] = atof(s);
 s = strtok(NULL,whitespace);
 m->diffuse[1] = atof(s);
 s = strtok(NULL,whitespace);
 m->diffuse[2] = atof(s);
}

/* get_ks - processes a "Ks" line */
static void get_ks(wfMaterial *m)
{
 char *s;
 s = strtok(NULL,whitespace);
 m->specular[0] = atof(s);
 s = strtok(NULL,whitespace);
 m->specular[1] = atof(s);
 s = strtok(NULL,whitespace);
 m->specular[2] = atof(s);
}

/* get_ns - processes a "Ns" line */
static void get_ns(wfMaterial *m)
{
 char *s;
 s = strtok(NULL,whitespace);
 m->specindex = atof(s);
}

/* get_dissolve - processes a "d" line */
static void get_dissolve(wfMaterial *m)
{
 char *s;
 s = strtok(NULL,whitespace);
 m->dissolve = atof(s);
}

/* line_type - determines the type of 'command' a line contains. Calls
	strtok() to get the first string on the line, and to prepare it
	for parsing by other routines. */
static int line_type(char *line)
{
 char *cmd;
 int i;
 cmd = strtok(line,whitespace);
 if (!cmd) return(COMMENT);
 for (i=0; i<NUMTOKENS; i++)
	if (!strcmp(cmd,token[i].str)) return(token[i].id);
 return(UNKNOWN);
}

/* wfFreeMaterialList - frees all the structs in a material list */
void wfFreeMaterialList(wfMaterial *m)
{
 wfMaterial *next;
 while (m) {
	next = m->next;
	wfFree(m);
	m = next;
	}
}
