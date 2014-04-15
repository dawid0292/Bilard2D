#ifndef BALL_H
#define BALL_H

class Ball{
public:
	Ball();
	void drawBall(double, double, double);
	void changePosition(double, double, double);
	void setX(double x){this->x = x;};
	void setZ(double z){this->z = z;};
	void setSpeed(double speed){this->speed = speed;};
	double getX(){return x;};
	double getZ(){return z;};
	double getAngle(){return angle;};
	double getSpeed(){return speed;};
private:
	double x, y, a, b, z, angle, speed, radius;
};

#endif