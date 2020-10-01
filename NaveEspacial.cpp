#include "NaveEspacial.h"
void NaveEspacial::DibujarNave(float x, float y, float z) 
{
	glPushMatrix();
		glTranslatef(x, y, z);
		glutSolidCylinder(0.3, 0.8, 20, 20);
		glTranslatef(0.0, 0.0, 0.8);
		glutSolidCone(0.3, 0.5, 20, 20);
	glPopMatrix();
}

NaveEspacial::NaveEspacial() {

}