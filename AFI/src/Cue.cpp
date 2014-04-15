#include "Cue.h"
#include "Ball.h"
#include <GL\glut.h>

void Cue::drawCue(Ball whiteBall, double r, double g, double b){
	GLUquadricObj *obj = gluNewQuadric();
	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3d(r ,g, b);
	glScalef(0.05, 0.05, 0.05);
	gluCylinder(obj, 0.44, 0.48, 26.2, 30, 30);
	glPopMatrix();
}