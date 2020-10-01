#include "Arbol.h"
void Arbol::DibujarArbol(float x, float y, float z) {
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glutSolidCylinder(0.15, 0.8, 20, 20);
	glTranslatef(0.0, 0.0, -0.3);
	glutWireSphere(0.4, 20, 20);
	glTranslatef(0.15, 0.0, 0.1);
	glutWireSphere(0.4, 20, 20);
	glTranslatef(-0.2, 0.0, 0.1);
	glutWireSphere(0.4, 20, 20);
	glPopMatrix();
}

Arbol::Arbol(){}