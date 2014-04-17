#include "PoolTable.h"
#include <vector>

void PoolTable::drawTable(int n, std::vector< Vertex > vertices)
{
	static GLfloat col[3] = {1.0f, 0.0f, 0.0f};
	glBegin(GL_TRIANGLES);
	for(int i = 0; i < n;){      // tworzymy n/3 œcian trójk¹tnych
		glColor3fv(col);             // kolor wg numeru œciany
		//for(int j = 0; j < 3; j++)
		glVertex3d(vertices[i].x, vertices[i].y, vertices[i].z);
		glVertex3d(vertices[i + 1].x, vertices[i + 1].y, vertices[i + 1].z);
		glVertex3d(vertices[i + 2].x, vertices[i + 2].y, vertices[i + 2].z);
		//printf("%f, %f, %f \n", vertices[i].x, vertices[i].y, vertices[i].z);
		//printf("%f, %f, %f \n", vertices[i+1].x, vertices[i+1].y, vertices[i+1].z);
		//printf("%f, %f, %f \n", vertices[i+2].x, vertices[i+2].y, vertices[i+2].z);
		i += 3;
	}
	glEnd();

}