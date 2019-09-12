#include "Bloco.h"

//#include "../extras.h"

#include <GL/glut.h>
#include <stdlib.h>

Bloco::Bloco(vertice origem, float dim, float cor[])
{
    this->origem = origem;
    this->dim = dim;
    this->cor[0] = cor[0];
    this->cor[1] = cor[1];
    this->cor[2] = cor[2];
    this->show = true;
}

void Bloco::drawBloco() {
    if (show) {
        glPushMatrix();
            glColor3f(cor[0],cor[1],cor[2]);
            glTranslatef(origem.x, origem.y, origem.z);
            glScalef(dim,1.0, 1.0);
            glutSolidCube(0.25);
        glPopMatrix();
    }
}
