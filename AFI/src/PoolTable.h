#ifndef POOLTABLE_H
#define POOLTABLE_H

#include <GL/glut.h>
#include <vector>
#include "Vertex.h"


class PoolTable{
private:
	double minX, maxX, minZ, maxZ;
	double xPocket[6];
	double zPocket[6];
	double xLeftTopPocket, zLeftTopPocket;
	double xMiddleTopPocket, zMiddleTopPocket;
	double xRightTopPocket, zRightTopPocket;
	double xLeftBottomPocket, zLeftBottomPocket;
	double xMiddleBottomPocket, zMiddleBottomPocket;
	double xRightBottomPocket, zRightBottomPocket;
	double pocketRadius;
public:
	double getMinX(){return minX;};
	double getMaxX(){return maxX;};
	double getMinZ(){return minZ;};
	double getMaxZ(){return maxZ;};
	double getPocketRadius(){return pocketRadius;};
	double * getXPocket(){return xPocket;};
	double * getZPocket(){return zPocket;};
	PoolTable(double, double, double, double);
	void drawTable(int, std::vector< Vertex >, std::vector< Vertex>, std::vector<Vertex> uv);
};

#endif