#include "Physicist.h"
#include "Ball.h"
#include <math.h>
#include <iostream>
#include <GL\glut.h>

void Physicist::moveBall(Ball & ball){
	double tmpZ = -sin(3.1415926535 * ball.getAngle() / 180.0) * ball.getSpeed() / 10.0;
	double tmpX = cos(3.1415926535 * ball.getAngle() / 180.0) * ball.getSpeed() / 10.0;
	double distance = sqrt(tmpZ * tmpZ + tmpX * tmpX);
	//std::cout<<"tmp "<<tmpX<<" "<<tmpZ<<std::endl;
	ball.changePosition(ball.getX() + tmpX, 0.0, ball.getZ() + tmpZ);
	ball.setSpeed(ball.getSpeed() * 0.95);
	std::cout<<"dystans "<<distance * 360 / (2 * 3.1415926535 * ball.getRadius())<<std::endl;
	ball.setRotate(ball.getRotate() + distance * 360 / (2 * 3.1415926535 * ball.getRadius()));
	if(ball.getSpeed() < 0.1){
		ball.setSpeed(1.0);
		ball.setRotate(0.0);
		ball.setAngle(90.0);
	}
	//std::cout<<ball.getSpeed()<<std::endl;
	//rotateBall(ball);
};

void Physicist::rotateBall(Ball & ball){
	double axisAngle = (ball.getAngle() + 90.0);//kat osi obrotu
	//double distance = sqrt(cos(3.14 * ball.getAngle() / 180)
	//glPushMatrix();
	//glTranslated(-ball.getX(), 0.0, -ball.getZ());
	//ball.setRotate(ball.getRotate() + 10.0);
	std::cout<<"obrocilem "<<axisAngle<<" x "<<cos(3.1415926535 * axisAngle / 180.0)<<" z "<<sin(3.1415926535 * axisAngle / 180.0)<<std::endl;
	glRotated(ball.getRotate(), cos(3.1415926535 * axisAngle / 180.0), 0.0, sin(3.1415926535 * axisAngle / 180.0));
	//ball.setRotate(ball.getRotate() + 10.0);
	//glRotated(5, 1.0, 0.0, 0.0);
	//std::cout<<"obrocilem"<<std::endl;
	//glTranslated(ball.getX(), 0.0, ball.getZ());
	//glPopMatrix();
}
