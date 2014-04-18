#include "PoolTable.h"
#include <vector>


void PoolTable::drawTable(int n, std::vector< Vertex > vertices, std::vector< Vertex > normals)
{
	static GLfloat col[3] = {1.0f, 0.0f, 0.0f};
	glBegin(GL_TRIANGLES);
	for(int i = 0; i < n;){      // tworzymy n/3 œcian trójk¹tnych
		//glColor3fv(col);             // kolor wg numeru œciany
		//for(int j = 0; j < 3; j++)
		glNormal3f(normals[i].x, normals[i].y, normals[i].z);
		glVertex3d(vertices[i].x, vertices[i].y, vertices[i].z);
		glNormal3f(normals[i+1].x, normals[i+1].y, normals[i+1].z);
		glVertex3d(vertices[i + 1].x, vertices[i + 1].y, vertices[i + 1].z);
		glNormal3f(normals[i+2].x, normals[i+2].y, normals[i+2].z);
		glVertex3d(vertices[i + 2].x, vertices[i + 2].y, vertices[i + 2].z);
		//printf("%f, %f, %f \n", vertices[i].x, vertices[i].y, vertices[i].z);
		//printf("%f, %f, %f \n", vertices[i+1].x, vertices[i+1].y, vertices[i+1].z);
		//printf("%f, %f, %f \n", vertices[i+2].x, vertices[i+2].y, vertices[i+2].z);
		i += 3;
		
	}
	glEnd();

}/*
glNormal3f(vn[(f[i].vn1-1)].x, vn[(f[i].vn1-1)].y, vn[(f[i].vn1-1)].z);
glVertex3f(v[(f[i].v1-1)].x, v[(f[i].v1-1)].y, v[(f[i].v1-1)].z);

glNormal3f(vn[(f[i].vn2-1)].x, vn[(f[i].vn2-1)].y, vn[(f[i].vn2-1)].z);
glVertex3f(v[(f[i].v2-1)].x, v[(f[i].v2-1)].y, v[(f[i].v2-1)].z);

glNormal3f(vn[(f[i].vn3-1)].x, vn[(f[i].vn3-1)].y, vn[(f[i].vn3-1)].z);
glVertex3f(v[(f[i].v3-1)].x, v[(f[i].v3-1)].y, v[(f[i].v3-1)].z

		   glBegin(GL_TRIANGLES);
for(int i = 0; i < f_count; i++){
    if(f[i].type == 'c'){
        GLfloat * color = new GLfloat[4];
        color[0] = f[i].x;
        color[1] = f[i].y;
        color[2] = f[i].z;
        color[3] = 1;

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
    }
    else{
        glVertex3f(vn[(f[i].vn1-1)].x, vn[(f[i].vn1-1)].y, vn[(f[i].vn1-1)].z); // normals
        glVertex3f(vn[(f[i].vn2-1)].x, vn[(f[i].vn2-1)].y, vn[(f[i].vn2-1)].z); // normals
        glVertex3f(vn[(f[i].vn3-1)].x, vn[(f[i].vn3-1)].y, vn[(f[i].vn3-1)].z); // normals

        glVertex3f(v[(f[i].v1-1)].x, v[(f[i].v1-1)].y, v[(f[i].v1-1)].z);
        glVertex3f(v[(f[i].v2-1)].x, v[(f[i].v2-1)].y, v[(f[i].v2-1)].z);
        glVertex3f(v[(f[i].v3-1)].x, v[(f[i].v3-1)].y, v[(f[i].v3-1)].z);   
    }
}
glEnd();*/