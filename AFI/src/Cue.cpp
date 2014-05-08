#include "Cue.h"
#include "Ball.h"
#include <GL\glut.h>
#include <math.h>
#include <iostream>
#include "constants.h"

Cue::Cue(double angle, double smallerRadius, double biggerRadius){
	this->x = 0.0;
	this->y = 0.0572;
	this->z = 0.0;
	this->angle = angle;
	this->smallerRadius = smallerRadius;
	this->biggerRadius = biggerRadius;
	this->length = 2.64;
	this->power = 0.13;
	this->increasePower = false;
	this->showCue = true;
	this->hitBall = true ;
}

void Cue::drawCue(Ball whiteBall, double r, double g, double b){
	if(showCue){
		GLUquadricObj *obj = gluNewQuadric();
		glPushMatrix();	
		glColor3d(r ,g, b);
		//glScalef(0.05, 0.05, 0.05);
		//std::cout<<"pozycja bialej w kiju "<<whiteBall.getX()<<" "<<whiteBall.getZ()<<std::endl;

		glTranslated(whiteBall.getX(), whiteBall.getY(), whiteBall.getZ());
		glTranslated(x * power, 0.0, z * power);
		glRotated(angle, 0.0, 1.0, 0.0);
		glRotated(7.0, -1.0, 0.0, 0.0);//nie wiem dlaczego tak ale dziala
		//std::cout<<"angle "<<angle<<std::endl;
		//glRotatef(45.0, cos(PI * angle / 180.0), 0.0, sin(PI * angle/ 180.0));
		//glRotated(45.0, cos(PI * (angle + 270) / 180.0), 0.0, sin(PI * (angle + 270)/ 180.0) );
		//std::cout<<"x "<<cos(PI * (angle + 270) / 180.0)<<" z "<<sin(PI * (angle + 270)/ 180.0)<<std::endl;
		//glTranslated(x, y, z);
		gluCylinder(obj, smallerRadius, biggerRadius, length, 30, 30);
		glPopMatrix();
	}
} 

void Cue::setDirection(int x){
	angle = (double)x * (double)(360.0/750.0) - 10.0;
	//std::cout<<"***"<<x<<"****";
	this->x = cos(PI * (angle - 90) / 180.0);
	this->z = -sin(PI * (angle  - 90)/ 180.0);
}