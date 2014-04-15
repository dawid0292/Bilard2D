#ifndef POOLTABLE_H
#define POOLTABLE_H

#include <GL/glut.h>

class PoolTable{
private:
	double x, y, z;
public:
	void drawTable(int, GLfloat[][3], int[][3]);
};

#endif