#ifndef CUE_H
#define CUE_H

#include "Ball.h"

class Cue{
private:
	double x, y, z, angle, smallerRadius, biggerRadius, length, power;
	bool increasePower, showCue, hitBall;
public:
	Cue(double, double, double);
	void drawCue(Ball, double, double, double);
	void setDirection(int);
	void setPower(float power){this->power = power;};
	void setIncreasePower(bool boolean){this->increasePower = boolean;};
	void setShowCue(bool boolean){this->showCue = boolean;};
	void setHitBall(bool boolean){this->hitBall= boolean;};
	void setX(float x){this->x = x;};
	void setZ(float z){this->z = z;};
	float getPower(){return power;};
	bool getIncreasePower(){return increasePower;};
	bool getShowCue(){return showCue;};
	bool getHitBall(){return hitBall;};
	double getZ(){return z;};
	double getX(){return x;};
	double getAngle(){return angle;};
};

#endif