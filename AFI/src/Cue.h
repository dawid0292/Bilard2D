#ifndef CUE_H
#define CUE_H

#include "Ball.h"

class Cue{
private:
	double x, y, z, angle, smallerRadius, biggerRadius;
public:
	Cue(double, double, double, double, double, double);
	void drawCue(Ball, double, double, double);
	void setDirection(int);
};

#endif