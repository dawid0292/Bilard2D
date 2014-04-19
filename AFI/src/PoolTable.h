#ifndef POOLTABLE_H
#define POOLTABLE_H

#include <GL/glut.h>
#include <vector>
#include "Vertex.h"

class PoolTable{
private:
double x, y, z;
public:
void drawTable(int, std::vector< Vertex >, std::vector< Vertex>);
};

#endif