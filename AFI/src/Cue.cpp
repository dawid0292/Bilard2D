#include "Cue.h"
#include "Ball.h"
#include <GL\glut.h>
#include <math.h>
#include <iostream>

Cue::Cue(double x, double y, double z, double angle, double smallerRadius, double biggerRadius){
	this->x = x;
	this->y = y;
	this->z = z;
	this->angle = angle;
	this->smallerRadius = smallerRadius;
	this->biggerRadius = biggerRadius;
}

void Cue::drawCue(Ball whiteBall, double r, double g, double b){
	GLUquadricObj *obj = gluNewQuadric();
	glPushMatrix();	
	glColor3d(r ,g, b);
	//glScalef(0.05, 0.05, 0.05);
	//std::cout<<"pozycja bialej w kiju "<<whiteBall.getX()<<" "<<whiteBall.getZ()<<std::endl;
	
	glTranslated(whiteBall.getX(), 0.0, whiteBall.getZ());
	//glTranslated(x, y, z);
	glRotated(angle, 0.0, 1.0, 0.0);
	gluCylinder(obj, smallerRadius, biggerRadius, 1.31, 30, 30);
	glPopMatrix();
}

void Cue::setDirection(int x){
	angle = (double)x * (double)(360.0/750.0) -10.0;
	//std::cout<<"***"<<x<<"****";
	this->x = sin(3.1415926535 * angle / 180.0);
	this->y = cos(3.1415926535 * angle / 180.0);
}