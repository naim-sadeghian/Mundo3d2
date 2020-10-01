#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "glsl.h"
#include <time.h>
#include "glm/glm.h"
class Objeto3D
{
	GLMmodel* objmodel_ptr;
	float x, y, z;
public:
	void AbrirMalla(char* objeto);
	void DibujarMalla(float _x, float _y, float _z);
};
