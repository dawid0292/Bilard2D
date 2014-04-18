#include "wave.h"

static void write_face(FILE *outfp,wfPart *p,wfObject *obj);
static void write_line(FILE *outfp,wfPart *p,wfObject *obj);

void wfWriteObject(FILE *outfp,wfObject *obj)
{
 int i;
 wfPart *p;
/* if (!obj->allParts) */
	{
	for (i=0; i<obj->nverts; i++)
		fprintf(outfp,"v %f %f %f\n",obj->vert[i][0],obj->vert[i][1],
			obj->vert[i][2]);
	for (i=0; i<obj->nnorms; i++)
		fprintf(outfp,"vn %f %f %f\n",obj->norm[i][0],obj->norm[i][1],
			obj->norm[i][2]);
	for (i=0; i<obj->ntexcoords; i++)
		fprintf(outfp,"vt %f %f\n",obj->texc[i][0],obj->texc[i][1]);
	}
 for (p=obj->parts; p; p=p->next)
    switch (p->parttype)
	{
	case WF_MATERIALLIB: fprintf(outfp,"mtllib %s\n",p->part.materialLib->name);
			     break;
	case WF_TEXTURELIB: fprintf(outfp,"maplib %s\n",p->part.textureLib->name);
			    break;
	case WF_MATERIAL: if (p->part.material)
				fprintf(outfp,"usemtl %s\n",p->part.material->name);
			  else
				fprintf(outfp,"usemtl\n");
			  break;
	case WF_TEXTURE: fprintf(outfp,"usemap %s\n",p->part.texture->name);
			 break;
	case WF_FACE: write_face(outfp,p,obj);
		      break;
	case WF_LINE: write_line(outfp,p,obj);
		      break;
/*
	case WF_VERTEX: fprintf(outfp,"v %f %f %f\n",(*p->part.vertex)[0],
				(*p->part.vertex)[1],(*p->part.vertex)[2]);
		      break;
	case WF_NORMAL: fprintf(outfp,"vn %f %f %f\n",(*p->part.normal)[0],
				(*p->part.normal)[1],(*p->part.normal)[2]);
		      break;
	case WF_TEXCOORD: fprintf(outfp,"vt %f %f\n",(*p->part.texcoord)[0],
				(*p->part.texcoord)[1]);
		      break;
*/
	case WF_UNSUPPORTED: fprintf(outfp,"%s\n",p->part.unsupported);
			   break;
	}
}

static void write_face(FILE *outfp,wfPart *p,wfObject *obj)
{
 int v;
 wfFace *f;
 f = &(p->part.face);
 fprintf(outfp,"f");
 for (v=0; v<f->nverts; v++)
	{
	fprintf(outfp," %d",f->vert[v]);
	if (f->tex[v])
		fprintf(outfp,"/%d",f->tex[v]);
	else if (f->norm[v])
		fprintf(outfp,"/");
	if (f->norm[v])
		fprintf(outfp,"/%d",f->norm[v]);
	}
 fprintf(outfp,"\n");
}

static void write_line(FILE *outfp,wfPart *p,wfObject *obj)
{
 int v;
 wfLine *l;
 l = &(p->part.line);
 fprintf(outfp,"l");
 for (v=0; v<l->nverts; v++)
	{
	fprintf(outfp," %d",l->vert[v]);
	}
 fprintf(outfp,"\n");
}
