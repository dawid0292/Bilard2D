#ifndef PHYSICIST_H
#define PHYSICIST_H

class Ball;
class PoolTable;
class Cue;

class Physicist{
public:
	void moveBall(Ball*);
	void rollbackMoveBall(Ball*);
	void rotateBall(Ball*);
	void checkBallsCollision(Ball*, Ball*);
	bool checkBallsCollisionNew(Ball*, Ball*);
	void checkCollisionBallWithBands(Ball*, PoolTable);
	void ballInPocket(Ball *, PoolTable);
	void setBallsDirectionAfterHitByCue(Ball*, Cue);
	double * quadraticEquation(double, double, double);
};

#endif