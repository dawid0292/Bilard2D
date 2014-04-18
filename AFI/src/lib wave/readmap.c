#include <stdio.h>
#include <math.h>
#include <string.h>
#include "wave.h"

static int line_type(char *line);
static int define_map(wfTexmap **list,FILE *fp);
static wfTexmap *create_map_struct();
static void get_ka(wfTexmap *m);
static void get_kd(wfTexmap *m);
static void get_ks(wfTexmap *m);
static void get_bump(wfTexmap *m);
static void get_dissolve(wfTexmap *m);

static char *whitespace = " \t";

/* Types of lines that may be read from object file */
#define NEWMAP 1
#define KA 2
#define KD 3
#define KS 4
#define BUMP 5
#define DISSOLVE 6
#define COMMENT 99
#define UNKNOWN 999

struct _token { char *str; int id; };
#define NUMTOKENS 7
static struct _token token[NUMTOKENS] = {
			{ "newmap", NEWMAP },
			{ "Ka", KA },
			{ "Kd", KD },
			{ "Ks", KS },
			{ "bump", BUMP },
			{ "d", DISSOLVE },
			{ "#", COMMENT}
			};

/* wfReadMaplib - reads a Wavefront map file and returns a pointer to a
	list of the texmaps defined. Returns NULL if it cannot read the
	file. */
wfTexmap *wfReadMaplib(char *fname)
{
 wfTexmap *list;
 FILE *fp;
 char line[1024],fname2[256];
 int t;
 if ((fp = fopen(fname,"r")) == NULL) {
	sprintf(fname2,"%s.map",fname);
	if ((fp = fopen(fname2,"r")) == NULL) {
		perror(fname);
		return(NULL);
		}
	}
/* Read lines until first "newmap" found */
 list = NULL;
 while (wfReadLine(line,1024,fp,NULL)) {
	line[strlen(line)-1] = '\0'; /* get rid of newline */
	t = line_type(line);
	if (t==NEWMAP) break;
	}
/* Add texmap definitions until EOF reached */
 if (t==NEWMAP)
	while (define_map(&list,fp)) ;
 fclose(fp);
 return(list);
}

/* create_map_struct - creates a wfTexmap struct & fills it with default
	values. */
static wfTexmap *create_map_struct()
{
 wfTexmap *m;
 m = wfAlloc(sizeof(wfTexmap));
 if (!m) return NULL;
 bzero(m->name,sizeof(m->name));
 bzero(m->Ka_file,sizeof(m->Ka_file));
 bzero(m->Kd_file,sizeof(m->Kd_file));
 bzero(m->Ks_file,sizeof(m->Ks_file));
 bzero(m->bump_file,sizeof(m->bump_file));
 bzero(m->d_file,sizeof(m->d_file));
 m->Ka_s[0] = m->Ka_s[1] = m->Ka_s[2] = 1.0;
 m->Kd_s[0] = m->Kd_s[1] = m->Kd_s[2] = 1.0;
 m->Ks_s[0] = m->Ks_s[1] = m->Ks_s[2] = 1.0;
 m->bump_s[0] = m->bump_s[1] = m->bump_s[2] = 1.0;
 m->Ka_o[0] = m->Ka_o[1] = m->Ka_o[2] = 0.0;
 m->Kd_o[0] = m->Kd_o[1] = m->Kd_o[2] = 0.0;
 m->Ks_o[0] = m->Ks_o[1] = m->Ks_o[2] = 0.0;
 m->bump_o[0] = m->bump_o[1] = m->bump_o[2] = 0.0;
 m->bump_bm = 1.0;
 m->d_s[0] = m->d_s[1] = m->d_s[2] = 1.0;
 m->d_o[0] = m->d_o[1] = m->d_o[2] = 0.0;
 m->texmap_number = wfNewID();
 m->used = 0;
 m->next = NULL;
 return(m);
}

/* define_map - creates a new texmap struct & fills it in. Returns
	1 when the next "newmap" is reached, or 0 if EOF is reached. */
static int define_map(wfTexmap **list,FILE *fp)
{
 wfTexmap *map;
 char *name,line[1024];
 int t;
 map = create_map_struct();
 if (!map) return 0;
 map->next = *list;
 *list = map;
 name = strtok(NULL,whitespace);
 if (name)
	strncpy(map->name,name,sizeof(map->name)-1);
 else
	map->name[0] = '\0';
 while (wfReadLine(line,1024,fp,NULL)) {
	line[strlen(line)-1] = '\0'; /* get rid of newline */
	t = line_type(line);
	switch (t) {
	   case NEWMAP: return(1);
	   case KA: get_ka(map); break;
	   case KD: get_kd(map); break;
	   case KS: get_ks(map); break;
	   case BUMP: get_bump(map); break;
	   case DISSOLVE: get_dissolve(map); break;
	   }
	}
 return(0);
}

/* get_ka - processes a "Ka" line */
static void get_ka(wfTexmap *m)
{
 char *s;
 while (s = strtok(NULL,whitespace))
	{
	if (s[0] != '-')
		{
		strncpy(m->Ka_file,s,sizeof(m->Ka_file)-1);
		return;
		}
	else if (!strcmp(s,"-s"))
		{
		s = strtok(NULL,whitespace);
		if (s) m->Ka_s[0] = atof(s);
		s = strtok(NULL,whitespace);
		if (s) m->Ka_s[1] = atof(s);
		s = strtok(NULL,whitespace);
		if (s) m->Ka_s[2] = atof(s);
		}
	else if (!strcmp(s,"-o"))
		{
		s = strtok(NULL,whitespace);
		if (s) m->Ka_o[0] = atof(s);
		s = strtok(NULL,whitespace);
		if (s) m->Ka_o[1] = atof(s);
		s = strtok(NULL,whitespace);
		if (s) m->Ka_o[2] = atof(s);
		}
	}
}

/* get_kd - processes a "Kd" line */
static void get_kd(wfTexmap *m)
{
 char *s;
 while (s = strtok(NULL,whitespace))
	{
	if (s[0] != '-')
		{
		strncpy(m->Kd_file,s,sizeof(m->Kd_file)-1);
		return;
		}
	else if (!strcmp(s,"-s"))
		{
		s = strtok(NULL,whitespace);
		if (s) m->Kd_s[0] = atof(s);
		s = strtok(NULL,whitespace);
		if (s) m->Kd_s[1] = atof(s);
		s = strtok(NULL,whitespace);
		if (s) m->Kd_s[2] = atof(s);
		}
	else if (!strcmp(s,"-o"))
		{
		s = strtok(NULL,whitespace);
		if (s) m->Kd_o[0] = atof(s);
		s = strtok(NULL,whitespace);
		if (s) m->Kd_o[1] = atof(s);
		s = strtok(NULL,whitespace);
		if (s) m->Kd_o[2] = atof(s);
		}
	}
}

/* get_ks - processes a "Ks" line */
static void get_ks(wfTexmap *m)
{
 char *s;
 while (s = strtok(NULL,whitespace))
	{
	if (s[0] != '-')
		{
		strncpy(m->Ks_file,s,sizeof(m->Ks_file)-1);
		return;
		}
	else if (!strcmp(s,"-s"))
		{
		s = strtok(NULL,whitespace);
		if (s) m->Ks_s[0] = atof(s);
		s = strtok(NULL,whitespace);
		if (s) m->Ks_s[1] = atof(s);
		s = strtok(NULL,whitespace);
		if (s) m->Ks_s[2] = atof(s);
		}
	else if (!strcmp(s,"-o"))
		{
		s = strtok(NULL,whitespace);
		if (s) m->Ks_o[0] = atof(s);
		s = strtok(NULL,whitespace);
		if (s) m->Ks_o[1] = atof(s);
		s = strtok(NULL,whitespace);
		if (s) m->Ks_o[2] = atof(s);
		}
	}
}

/* get_bump - processes a "bump" line */
static void get_bump(wfTexmap *m)
{
 char *s;
 while (s = strtok(NULL,whitespace))
	{
	if (s[0] != '-')
		{
		strncpy(m->bump_file,s,sizeof(m->bump_file)-1);
		return;
		}
	else if (!strcmp(s,"-s"))
		{
		s = strtok(NULL,whitespace);
		if (s) m->bump_s[0] = atof(s);
		s = strtok(NULL,whitespace);
		if (s) m->bump_s[1] = atof(s);
		s = strtok(NULL,whitespace);
		if (s) m->bump_s[2] = atof(s);
		}
	else if (!strcmp(s,"-o"))
		{
		s = strtok(NULL,whitespace);
		if (s) m->bump_o[0] = atof(s);
		s = strtok(NULL,whitespace);
		if (s) m->bump_o[1] = atof(s);
		s = strtok(NULL,whitespace);
		if (s) m->bump_o[2] = atof(s);
		}
	else if (!strcmp(s,"-bm"))
		{
		s = strtok(NULL,whitespace);
		if (s) m->bump_bm = atof(s);
		}
	}
}

/* get_dissolve - processes a "d" line */
static void get_dissolve(wfTexmap *m)
{
 char *s;
 while (s = strtok(NULL,whitespace))
	{
	if (s[0] != '-')
		{
		strncpy(m->d_file,s,sizeof(m->d_file)-1);
		return;
		}
	else if (!strcmp(s,"-s"))
		{
		s = strtok(NULL,whitespace);
		if (s) m->d_s[0] = atof(s);
		s = strtok(NULL,whitespace);
		if (s) m->d_s[1] = atof(s);
		s = strtok(NULL,whitespace);
		if (s) m->d_s[2] = atof(s);
		}
	else if (!strcmp(s,"-o"))
		{
		s = strtok(NULL,whitespace);
		if (s) m->d_o[0] = atof(s);
		s = strtok(NULL,whitespace);
		if (s) m->d_o[1] = atof(s);
		s = strtok(NULL,whitespace);
		if (s) m->d_o[2] = atof(s);
		}
	}
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

/* wfFreeTexmapList - frees all the structs in a texmap list */
void wfFreeTexmapList(wfTexmap *m)
{
 wfTexmap *next;
 while (m) {
	next = m->next;
	wfFree(m);
	m = next;
	}
}

wfTexmap *wfOffTexmap()
{
 static wfTexmap *offtex=NULL;
 if (!offtex)
	{
	offtex = create_map_struct();
	if (!offtex) return NULL;
	offtex->texmap_number = 0;
	offtex->used = 1;
	strcpy(offtex->name,"off");
	}
 return offtex;
}
