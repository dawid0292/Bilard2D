#include "Physicist.h"
#include "Ball.h"
#include <math.h>
#include <iostream>
#include <GL\glut.h>

void Physicist::moveBall(Ball & ball){
	double tmpX = sin(3.14 * ball.getAngle() / 180.0) * ball.getSpeed() / 10.0;
	double tmpZ = cos(3.14 * ball.getAngle() / 180.0) * ball.getSpeed() / 10.0;
	//std::cout<<tmpX<<std::endl;
	ball.changePosition(ball.getX() + tmpX, 0.0, ball.getZ() + tmpZ);
	ball.setSpeed(ball.getSpeed() * 0.95);
	if(ball.getSpeed() < 0.1)
		ball.setSpeed(0.0);
	//std::cout<<ball.getSpeed()<<std::endl;
	//rotateBall(ball);
};

void Physicist::rotateBall(Ball & ball){
	double axisAngle = ball.getAngle() - 90;//kat osi obrotu
	//glPushMatrix();
	glTranslated(-ball.getX(), 0.0, -ball.getZ());
	//glRotated(5, 1.0, 0.0, sin(3.14 * axisAngle / 180));//tu zamiast tego 5 powinno bys jakies przeliczenie ile drogi pokona
	glRotated(5, 0.0, 1.0, 0.0);
	//std::cout<<"obrocilem"<<std::endl;
	glTranslated(ball.getX(), 0.0, ball.getZ());
	//glPopMatrix();
}
