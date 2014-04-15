#ifndef CUE_H
#define CUE_H

#include "Ball.h"

class Cue{
private:
	double x, y, z;
public:
	void drawCue(Ball, double, double, double);
};

#endif