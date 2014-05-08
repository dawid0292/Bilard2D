#ifndef BALL_H
#define BALL_H

class Physicist;

class Ball{
public:
	Ball(int);
	Ball(int, double, double, double);
	void drawBall(Physicist, double, double, double);
	void changePosition(double, double, double);
	void setX(double x){this->x = x;};
	void setZ(double z){this->z = z;};
	void setSpeed(double speed){this->speed = speed;};
	void setAngle(double);
	double getX(){return x;};
	double getY(){return y;};
	double getZ(){return z;};
	double getAngle(){return angle;};
	double getSpeed(){return speed;};
	int getNumber(){return number;};
	double getRotate(){return rotate;};
	double getRadius(){return radius;};
	void setRotate(double rotate){this->rotate = rotate;};
private:
	double x, y, a, b, z, angle, speed, radius, rotate, previousX, previousZ;
	int number;
};

#endif