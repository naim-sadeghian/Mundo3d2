#include "Objeto3D.h"

void Objeto3D::DibujarMalla(float _x, float _y, float _z, bool texturizado) {
    x = _x;
    y = _y;
    z = _z;

    glTranslatef(x, y, z);
    if (texturizado) {
        glmDraw(objmodel_ptr, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
    }
    else {
        glmDraw(objmodel_ptr, GLM_SMOOTH | GLM_MATERIAL);
    }
    
}

void Objeto3D::AbrirMalla(char* objeto) {
    objmodel_ptr = NULL;
    if (!objmodel_ptr)
    {
        objmodel_ptr = glmReadOBJ(objeto);
        if (!objmodel_ptr)
            exit(0);

        glmUnitize(objmodel_ptr);
        glmFacetNormals(objmodel_ptr);
        glmVertexNormals(objmodel_ptr, 90.0);
    }
}