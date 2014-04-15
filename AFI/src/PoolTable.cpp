#include "PoolTable.h"

void PoolTable::drawTable(int n, GLfloat v[][3], int s[][3])
{
	static GLfloat col[3] = {1.0f, 0.0f, 0.0f};
	glBegin(GL_TRIANGLES);
	for(int i = 0; i < n; i ++){      // tworzymy n œcian trójk¹tnych
		glColor3fv(col);             // kolor wg numeru œciany
		for(int j = 0; j < 3; j++)
			glVertex3fv(v[ s[i][j] ]); 
	}
	glEnd();

}