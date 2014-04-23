#include "Ball.h"
#include "Physicist.h"
#include <GL\glut.h>
#include <iostream>

Ball::Ball(int number){
	this->a = 2.0;//na sztywno
	this->b = 0.0;//na sztywno
	x = 0;
	y = 0.0286;
	z = 0;
	previousX = 0;
	previousZ = 0;
	speed = 1;
	angle = 180.0;
	radius = 0.0572;
	this->number = number;
	rotate = 0;
}

void Ball::drawBall(Physicist physicist, double r, double g, double b){
	//std::cout<<"pozycja bili"<<x<<" "<<z<<std::endl;
	glPushMatrix();
	//glLoadIdentity();
	//glTranslatef(x, y, z);
	glColor3d(r ,g, b);
	//glScalef(0.05, 0.05, 0.05);
	
	//glRotatef(45.0, 0.0, 1.0, 0.0);
	glTranslated(x,y + 0.028,z);
	//glRotated(90.0, 0.0, 1.0, 0.0);
	physicist.rotateBall(*this);
	//glRotatef(rotate += 10, angle - 90, 1.0, 0.0);//powinny byc specjalne wyliczenia tego rotate
	//std::cout<<"move ball"<<std::endl;
	glutWireTorus(0.01, radius,30, 30);
	//glutSolidSphere(radius, 10, 20);
	// Sleep(10000);
	//glutSolidSphere(radius,20,10);
	glPopMatrix();
	//glFlush();
}
void Ball::changePosition(double x, double y, double z){
	this->x = x;
	this->y = y;
	this->z = z;
}
