#include "Physicist.h"
#include "Ball.h"
#include "PoolTable.h"
#include "Cue.h"
#include <math.h>
#include <iostream>
#include <GL\glut.h>
#include "constants.h"

void drawScene();

void Physicist::rollbackMoveBall(Ball * ball){
	double tmpZ = -sin(PI * (ball->getAngle() - 180.0) / 180.0) * ball->getSpeed() / (MOTIONDIVIDER * 2);
	double tmpX = cos(PI * (ball->getAngle() - 180.0) / 180.0) * ball->getSpeed() / (MOTIONDIVIDER * 2);
	double distance = sqrt(tmpZ * tmpZ + tmpX * tmpX);	
	ball->changePosition(ball->getX() + tmpX, ball->getRadius(), ball->getZ() + tmpZ);
	ball->setSpeed(ball->getSpeed() / 0.999);
	ball->setRotate(ball->getRotate() + distance * 360 / (2 * PI * ball->getRadius()));//trzeba zmienic obracanie bili
}

void Physicist::moveBall(Ball * ball){
	double tmpZ = -sin(PI * ball->getAngle() / 180.0) * ball->getSpeed() / MOTIONDIVIDER;
	double tmpX = cos(PI * ball->getAngle() / 180.0) * ball->getSpeed() / MOTIONDIVIDER;
	double distance = sqrt(tmpZ * tmpZ + tmpX * tmpX);
	//std::cout<<"tmp "<<tmpX<<" "<<tmpZ<<std::endl;
	//std::cout<<"bila numer "<<ball.getNumber()<<" x "<<ball.getX() + tmpX<<" z "<< ball.getZ() + tmpZ<<std::endl;
	ball->changePosition(ball->getX() + tmpX, ball->getRadius(), ball->getZ() + tmpZ);
	//std::cout<<"speed1 "<<ball.getSpeed()<<std::endl;
	ball->setSpeed(ball->getSpeed() * 0.999);
	//std::cout<<"speed2 "<<ball.getSpeed()<<std::endl;
	//std::cout<<"dystans "<<distance * 360 / (2 * PI * ball.getRadius())<<std::endl;
	ball->setRotate(ball->getRotate() + distance * 360 / (2 * PI * ball->getRadius()));//trzeba zmienic obracanie bili
	if(ball->getSpeed() < 0.01){
		ball->setSpeed(0.0);
		//ball.setRotate(0.0);
		//ball.setAngle(90.0);
	}
	//std::cout<<"x "<<ball.getX()<<" z "<<ball.getZ()<<" radius "<<ball.getRadius()<<std::endl;
	//rotateBall(ball);
	//_sleep(1000);
};

void Physicist::rotateBall(Ball * ball){
	double axisAngle = (ball->getAngle() + 90.0);//kat osi obrotu
	//double distance = sqrt(cos(3.14 * ball.getAngle() / 180)
	//glPushMatrix();
	//glTranslated(-ball.getX(), 0.0, -ball.getZ());
	//ball.setRotate(ball.getRotate() + 10.0);
	//std::cout<<"obrocilem "<<axisAngle<<" x "<<cos(PI * axisAngle / 180.0)<<" z "<<sin(PI * axisAngle / 180.0)<<std::endl;
	glRotated(ball->getRotate(), cos(PI * axisAngle / 180.0), 0.0, sin(PI * axisAngle / 180.0));
	//ball.setRotate(ball.getRotate() + 10.0);
	//glRotated(5, 1.0, 0.0, 0.0);
	//std::cout<<"obrocilem"<<std::endl;
	//glTranslated(ball.getX(), 0.0, ball.getZ());
	//glPopMatrix();
}

bool Physicist::checkBallsCollisionNew(Ball * firstBall, Ball * secondBall){
	double distance = sqrt((secondBall->getX() - firstBall->getX()) * (secondBall->getX() - firstBall->getX()) + ((-secondBall->getZ()) - (-firstBall->getZ())) * ((-secondBall->getZ()) - (-firstBall->getZ())));
	//std::cout<<" firstBall x "<<firstBall.getX()<<" z "<<firstBall.getZ()<<" secondBall x "<< secondBall.getX()<<" z "<<secondBall.getZ()<<std::endl;
	//std::cout<<"distance "<<distance<<" bila numer "<<firstBall.getNumber()<<" x "<<firstBall.getX()<<" z "<< firstBall.getZ()<<std::endl;
	//drawScene();	
	if(distance <= (firstBall->getRadius() * 2)){
		while(distance < firstBall->getRadius() * 2){
			rollbackMoveBall(firstBall);
			rollbackMoveBall(secondBall);
			distance = sqrt((secondBall->getX() - firstBall->getX()) * (secondBall->getX() - firstBall->getX()) + ((-secondBall->getZ()) - (-firstBall->getZ())) * ((-secondBall->getZ()) - (-firstBall->getZ())));
			//std::cout<<"rollbackMoveBall "<<std::endl;
			//_sleep(1000);
		}
		double firstX, secondX, firstZ, secondZ, a;
		firstX = firstBall->getX();
		firstZ = -firstBall->getZ();
		secondX = secondBall->getX();
		secondZ = -secondBall->getZ();
		a = (firstZ - secondZ) / (firstX - secondX);
		//std::cout<<" firstBall x "<<firstX<<" z "<<firstZ<<" secondBall x "<< secondX<<" z "<<secondZ<<std::endl;
		//std::cout<<"wspolczynnik nachylenia "<<a<<std::endl;
		double slopeAngle;
		if((secondX - firstX) > 0 && (secondZ - firstZ) > 0){
			slopeAngle = atan(a) * 180.0 / PI;
		}else if((secondX - firstX) < 0 && (secondZ - firstZ) < 0){
			slopeAngle = 180.0 + atan(a) * 180.0 / PI;
		}else if((secondX - firstX) < 0){
			slopeAngle = 180.0 + atan(a) * 180.0 / PI;
		}else if((secondZ - firstZ) < 0){
			slopeAngle = 360.0 + (atan(a) * 180.0 / PI);
		}
		//double slopeAngle = atan(a * 180.0 / PI);
		while(slopeAngle >= 360.0)
			slopeAngle -= 360.0;
		while(slopeAngle < 0.0)
			slopeAngle += 360.0;
		//std::cout<<"slopeAngle "<<slopeAngle<<std::endl;
		double xVectorFirstBall, zVectorFirstBall, xVectorSecondBall, zVectorSecondBall;
		zVectorFirstBall = sin(PI * (firstBall->getAngle() - slopeAngle) / 180.0) * firstBall->getSpeed() / MOTIONDIVIDER;
		zVectorSecondBall = sin(PI * (secondBall->getAngle() - slopeAngle) / 180.0) * secondBall->getSpeed() / MOTIONDIVIDER;
		xVectorFirstBall = cos(PI * (firstBall->getAngle() - slopeAngle) / 180.0) * firstBall->getSpeed() / MOTIONDIVIDER;
		xVectorSecondBall = cos(PI * (secondBall->getAngle() - slopeAngle) / 180.0) * secondBall->getSpeed() / MOTIONDIVIDER;

		//std::cout<<"zVectorFirstBall "<<zVectorFirstBall<< " zVectorSecondBall "<< zVectorSecondBall << " xVectorFirstBall " << xVectorFirstBall << " xVectorSecondBall " <<xVectorSecondBall<<std::endl;

		double newFirstBallAngle, newSecondBallAngle;

		if(xVectorSecondBall == 0.0 && zVectorFirstBall == 0.0)
			newFirstBallAngle = 0.0;
		else if(xVectorSecondBall == 0.0 && zVectorFirstBall > 0.0)
			newFirstBallAngle = 90.0;
		else if(xVectorSecondBall == 0.0 && zVectorFirstBall < 0.0)
			newFirstBallAngle = -90.0;
		else if(xVectorSecondBall > 0 && zVectorFirstBall > 0){
			newFirstBallAngle = atan(zVectorFirstBall / xVectorSecondBall) * 180.0 / PI;
		}else if(xVectorSecondBall < 0 && zVectorFirstBall < 0){
			newFirstBallAngle = 180.0 + atan(zVectorFirstBall / xVectorSecondBall) * 180.0 / PI;
		}else if(xVectorSecondBall < 0){
			newFirstBallAngle = 180.0 + atan(zVectorFirstBall / xVectorSecondBall) * 180.0 / PI;
		}else if(zVectorFirstBall < 0){
			newFirstBallAngle = 360.0 + atan(zVectorFirstBall / xVectorSecondBall) * 180.0 / PI;
		}
		//sqrt(xVectorFirstBall * xVectorFirstBall + zVectorSecondBall * zVectorSecondBall);
		//sqrt(xVectorSecondBall * xVectorSecondBall + zVectorFirstBall * zVectorFirstBall);

		//firstBall->setSpeed(MOTIONDIVIDER * sqrt(xVectorSecondBall * xVectorSecondBall + zVectorFirstBall * zVectorFirstBall) / cos(PI * (secondBall->getAngle() - slopeAngle) / 180.0));
		//secondBall->setSpeed(MOTIONDIVIDER * sqrt(xVectorFirstBall * xVectorFirstBall + zVectorSecondBall * zVectorSecondBall) / cos(PI * (firstBall->getAngle() - slopeAngle) / 180.0));

		//std::cout<<"before change firstBall->getSpeed() "<<firstBall->getSpeed()<<" secondBall->getSpeed() "<<secondBall->getSpeed()<<std::endl;

		firstBall->setSpeed(MOTIONDIVIDER * sqrt(xVectorSecondBall * xVectorSecondBall + zVectorFirstBall * zVectorFirstBall));
		secondBall->setSpeed(MOTIONDIVIDER * sqrt(xVectorFirstBall * xVectorFirstBall + zVectorSecondBall * zVectorSecondBall));

		//std::cout<<"after change firstBall->getSpeed() "<<firstBall->getSpeed()<<" secondBall->getSpeed() "<<secondBall->getSpeed()<<std::endl;
		
		if(xVectorFirstBall == 0.0 && zVectorSecondBall == 0.0)
			newFirstBallAngle = 0.0;
		else if(xVectorFirstBall == 0.0 && zVectorSecondBall > 0.0)
			newFirstBallAngle = 90.0;
		else if(xVectorFirstBall == 0.0 && zVectorSecondBall < 0.0)
			newFirstBallAngle = -90.0;
		else if(xVectorFirstBall > 0 && zVectorSecondBall > 0){
			newSecondBallAngle = atan(zVectorSecondBall / xVectorFirstBall) * 180.0 / PI;
		}else if(xVectorFirstBall < 0 && zVectorSecondBall < 0){
			newSecondBallAngle = 180.0 + atan(zVectorSecondBall / xVectorFirstBall) * 180.0 / PI;
		}else if(xVectorFirstBall < 0){
			newSecondBallAngle = 180.0 + atan(zVectorSecondBall / xVectorFirstBall) * 180.0 / PI;
		}else if(zVectorSecondBall < 0){
			newSecondBallAngle = 360.0 + atan(zVectorSecondBall / xVectorFirstBall) * 180.0 / PI;
		}

		firstBall->setAngle(newFirstBallAngle + slopeAngle);
		secondBall->setAngle(newSecondBallAngle + slopeAngle);


		//std::cout<<"newFirstBallAngle "<<newFirstBallAngle<< " newSecondBallAngle "<<newSecondBallAngle<<std::endl;
		//std::cout<<"newFirstBallAngle + slopeAngle "<<firstBall->getAngle()<< " newSecondBallAngle + slopeAngle "<<secondBall->getAngle()<<std::endl;

		//_sleep(30000);


		//Physicist physicist;
		//std::cout<<"rysuje"<<std::endl;
		//glTranslated(firstBall.getX(), firstBall.getRadius(), firstBall.getZ());
		//glutSolidSphere(firstBall.getRadius(), 10, 20);
		//firstBall.drawBall(physicist, 1.0, 1.0, 1.0);
		//std::cout<<"narysowalem"<<std::endl;
		//_sleep(500);
		//firstBall->setAngle(firstBall->getAngle() + 180.0);
		//secondBall->setAngle(firstBall->getAngle() + 180.0);
		//secondBall->setSpeed(firstBall->getSpeed() * 0.9);
		return true;
	}else{
		return false;
	}
	//std::cout<<"angle first ball"<<firstBall.getAngle()<<" angle second ball "<<secondBall.getAngle()<<std::endl;
}

void Physicist::checkBallsCollision(Ball * firstBall, Ball * secondBall){
	double distance = sqrt((secondBall->getX() - firstBall->getX()) * (secondBall->getX() - firstBall->getX()) + (secondBall->getZ() - firstBall->getZ()) * (secondBall->getZ() - firstBall->getZ()));
	//std::cout<<"distance "<<distance<<std::endl;
	double a, b, OY, c, x, y, tangesAngle, zCenter, xCenter, radius;
	double * squareRoots;
	double newAngle;
	//std::cout<<distance<<std::endl;
	//std::cout<<firstBall.getRadius() * 2<<std::endl;
	if(distance <= (firstBall->getRadius() * 2)){
		tangesAngle = tan(PI * firstBall->getAngle() / 180.0);
		OY = -firstBall->getZ() - tangesAngle * firstBall->getX();
		std::cout<<"angle "<<firstBall->getAngle()<<" tangesAngle "<<tangesAngle<<" OY "<<OY<<std::endl;
		std::cout<<"firstBall z "<<firstBall->getZ()<<" firstBall x " <<firstBall->getX()<<std::endl;
		zCenter = -secondBall->getZ();
		xCenter = secondBall->getX();
		radius = secondBall->getRadius();
		a = tangesAngle * tangesAngle + 1;
		b = 2 * tangesAngle * OY - 2 * zCenter * tangesAngle - 2 * xCenter;
		c = -2 * OY * zCenter + zCenter * zCenter + OY * OY + xCenter * xCenter - 4 * radius * radius;
		squareRoots = quadraticEquation(a, b, c);
		if(squareRoots[0] == 1){
			x = squareRoots[1];
		}else{
			std::cout<<"ile jest pierwiastkow "<<squareRoots[0]<< " x1 "<<squareRoots[1]<< " x2 "<<squareRoots[2]<<std::endl;
			std::cout<<"x "<<firstBall->getX()<<" xCenter "<<xCenter<<std::endl;
			if(firstBall->getAngle() > 90 && firstBall->getAngle() < 270)
				x = squareRoots[1] > squareRoots[2]? squareRoots[1]:squareRoots[2];//wchodzi tutaj gdy 
			else 
				x = squareRoots[1] < squareRoots[2]? squareRoots[1]:squareRoots[2];
		}
		y = (tan(PI * firstBall->getAngle() / 180.0) * x + OY);
		//std::cout<<"przed zmiana firstBall z "<<firstBall.getZ()<<" firstBall x " <<firstBall.getX()<<std::endl;
		firstBall->changePosition(x, firstBall->getRadius(), -y);
		//std::cout<<"po zmianie firstBall z "<<firstBall.getZ()<<" firstBall x " <<firstBall.getX()<<std::endl;
		std::cout<<"rozwiazanie x "<<x<<" y "<<y<<std::endl;
		//_sleep(5000);
		std::cout<<"zCenter "<<zCenter<<" y "<<y<<" xCenter "<<xCenter<<" x"<<x<<std::endl;
		std::cout<<"zCenter - y "<<(zCenter - y)<<" xCenter - x "<<(xCenter - x)<<std::endl;
		std::cout<<"(zCenter - y) / (xCenter - x) "<<(zCenter - y) / (xCenter - x)<<std::endl; 
		//newAngle = (atan((zCenter - y) / (xCenter - x)) * 180.0 / PI);
		if((xCenter - x) > 0 && (zCenter - y) > 0){
			newAngle = (atan((zCenter - y) / (xCenter - x)) * 180.0 / PI);
		}else if((xCenter - x) < 0 && (zCenter - y) < 0){
			newAngle = 180 + (atan((zCenter - y) / (xCenter - x)) * 180.0 / PI);
		}else if((xCenter - x) < 0){
			newAngle = 180 + (atan((zCenter - y) / (xCenter - x)) * 180.0 / PI);
		}else if((zCenter - y) < 0){
			newAngle = 360 + (atan((zCenter - y) / (xCenter - x)) * 180.0 / PI);
		}
		std::cout<<"newAngle "<<newAngle<<std::endl;
		secondBall->setAngle(newAngle);
		secondBall->setSpeed(1.0);
		if(newAngle > firstBall->getAngle())
			firstBall->setAngle(newAngle - 90);//tu powinno byc -90 lub 90 zaleznie czy z gory uderza czy z dolu
		else
			firstBall->setAngle(newAngle + 90);
		//std::cout<<"is collision"<<std::endl;
		_sleep(5000);
		//firstBall.setAngle(210);
	}
	//_sleep(200);
}

double * Physicist::quadraticEquation(double a, double b, double c){
	double d = b*b-4*a*c;
	double x1 = 0, x2 = 0;
	double * result = new double[3];
	//std::cout<<"a "<<a<<" b "<<b<<" c "<<c<<" d "<<d<<std::endl;
	//_sleep(5000);
	if (d==0)
	{
		x1 = -b/(2*a);
		result[0] = 1;
		result[1] = x1;
	} 
	else 
	{ 
		result[0] = 2;
		x1=(-b-sqrt(d))/(2*a);
		x2=(-b+sqrt(d))/(2*a);
		result[1] = x1;
		result[2] = x2;
	}
	std::cout<<"x1 "<<x1<<" x2 "<<x2<<std::endl;
	//_sleep(5000);
	return result;
}

void Physicist::checkCollisionBallWithBands(Ball * ball, PoolTable poolTable){
	double radius = ball->getRadius();
	double x = ball->getX();
	double z = ball->getZ();
	double minX = poolTable.getMinX();
	double maxX = poolTable.getMaxX();
	double minZ = poolTable.getMinZ();
	double maxZ = poolTable.getMaxZ();
	double angle = ball->getAngle();
	if(x - radius <= minX){
		//std::cout<<"lewa"<<std::endl;
		if(angle < 180.0 && angle >= 90.0){
			//std::cout<<"angle "<<angle<<std::endl;
			ball->setAngle(180.0 - angle);			
			//std::cout<<"old speed "<<ball->getSpeed()<<" ";
			ball->setSpeed(ball->getSpeed() * ((180.0 - angle) * (0.2/90.0) + 0.8));
			//std::cout<<"new speed "<<ball->getSpeed() * ((180.0 - angle) * (0.2/90.0) + 0.8)<<std::endl;
		}else if (angle >= 180.0 && angle <= 270.0){
			//std::cout<<"angle "<<angle<<std::endl;
			ball->setAngle(360.0 - angle + 180.0);
			//std::cout<<"old speed "<<ball->getSpeed()<<" ";
			ball->setSpeed(ball->getSpeed() * ((angle - 180.0) * (0.2/90.0) + 0.8));
			//std::cout<<"new speed "<<ball->getSpeed() * ((angle - 180.0) * (0.2/90.0) + 0.8)<<std::endl;
		}
	}else if(x + radius >= maxX){
		//std::cout<<"prawa"<<std::endl;
		if(angle >= 0.0 && angle <= 90.0){
			//std::cout<<"angle "<<angle<<std::endl;
			ball->setAngle(180.0 - angle);
			//std::cout<<"old speed "<<ball->getSpeed()<<" ";
			ball->setSpeed(ball->getSpeed() * ((angle) * (0.2/90.0) + 0.8));
			//std::cout<<"new speed "<<ball->getSpeed() * ((angle) * (0.2/90.0) + 0.8)<<std::endl;
		}else if(angle >= 270.0 && angle <= 360.0){
			//std::cout<<"angle "<<angle<<std::endl;
			ball->setAngle(360.0 - angle + 180.0);
			//std::cout<<"old speed "<<ball->getSpeed()<<" ";
			ball->setSpeed(ball->getSpeed() * ((360.0 - angle) * (0.2/90.0) + 0.8));
			//std::cout<<"new speed "<<ball->getSpeed() * ((360.0 - angle) * (0.2/90.0) + 0.8)<<std::endl;
		}
	}else if(z - radius <= minZ){
		//std::cout<<"gora?"<<std::endl;
		if(angle >= 90.0 && angle <= 180.0){
			//std::cout<<"angle "<<angle<<std::endl;
			ball->setAngle(360.0 - angle);
			//std::cout<<"old speed "<<ball->getSpeed()<<" ";
			ball->setSpeed(ball->getSpeed() * ((angle - 90.0) * (0.2/90.0) + 0.8));
			//std::cout<<"new speed "<<ball->getSpeed() * ((angle - 90.0) * (0.2/90.0) + 0.8)<<std::endl;
		}
		else if (angle < 90.0 && angle > 0.0){
			//std::cout<<"angle "<<angle<<std::endl;
			ball->setAngle(360.0 - angle);
			//std::cout<<"old speed "<<ball->getSpeed()<<" ";
			ball->setSpeed(ball->getSpeed() * ((90.0 - angle) * (0.2/90.0) + 0.8));
			//std::cout<<"new speed "<<ball->getSpeed() * ((90.0 - angle) * (0.2/90.0) + 0.8)<<std::endl;
		}
	}else if(z + radius >= maxZ){
		//std::cout<<"dol?"<<std::endl;
		if(angle <= 270.0 && angle >= 180.0){
			//std::cout<<"angle "<<angle<<std::endl;
			ball->setAngle(360.0 - angle);
			//std::cout<<"old speed "<<ball->getSpeed()<<" ";
			ball->setSpeed(ball->getSpeed() * ((270.0 - angle) * (0.2/90.0) + 0.8));
			//std::cout<<"new speed "<<ball->getSpeed() * ((270.0 - angle) * (0.2/90.0) + 0.8)<<std::endl;
		}else if(angle > 270.0 && angle <= 360.0){
			//std::cout<<"angle "<<angle<<std::endl;
			ball->setAngle(360.0 - angle);
			//std::cout<<"old speed "<<ball->getSpeed()<<" ";
			ball->setSpeed(ball->getSpeed() * ((angle - 270.0) * (0.2/90.0) + 0.8));
			//std::cout<<"new speed "<<ball->getSpeed() * ((angle - 270.0) * (0.2/90.0) + 0.8)<<std::endl;
		}
	}
	//std::cout<<"angle "<<ball.getAngle()<<std::endl;
	//_sleep(1000);
}

void Physicist::ballInPocket(Ball * ball, PoolTable poolTable){
	double xBall = ball->getX();
	double zBall = ball->getY();
	double * xPocket, * zPocket;
	xPocket = poolTable.getXPocket();
	zPocket = poolTable.getZPocket();
	for(int i = 0; i < 6; i++){
		if( sqrt((xBall - xPocket[i]) * (xBall - xPocket[i]) + (zBall - zPocket[i]) * (zBall - zPocket[i])) <= ball->getRadius() + poolTable.getPocketRadius()) 
			std::cout<<"w luzie"<<std::endl;
	}
}

void Physicist::setBallsDirectionAfterHitByCue(Ball * whiteBall, Cue cue){
	double xBall, zBall, xCue, zCue, newAngle;
	xBall = whiteBall->getX();
	zBall = -whiteBall->getZ();
	xCue = cue.getX();
	zCue = -cue.getZ();

	//printf("ball %f %f, cue %f %f\n",xBall, zBall, xCue, zCue);
	//std::cout<<"angle "<<cue.getAngle()<<std::endl;
	//std::cout<<"newAngle "<<newAngle<<std::endl;
	whiteBall->setAngle(cue.getAngle() + 90.0);
}