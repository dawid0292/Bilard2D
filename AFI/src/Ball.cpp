#include "Ball.h"
#include <GL\glut.h>

Ball::Ball(){
	this->a = 2.0;//na sztywno
	this->b = 0.0;//na sztywno
	x = 0;
	y = 0.0286;
	z = 0;
	speed = 1;
	angle = 30;
	radius = 0.0286;
}

void Ball::drawBall(double r, double g, double b){
	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3d(r ,g, b);
	//glScalef(0.05, 0.05, 0.05);
	glutSolidSphere(radius,20,10);
	glPopMatrix();
}
void Ball::changePosition(double x, double y, double z){
	this->x = x;
	this->y = y;
	this->z = z;
}
