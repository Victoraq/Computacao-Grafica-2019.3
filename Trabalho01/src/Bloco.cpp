#include "Bloco.h"



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

void Bloco::setColor(float r, float g, float b)
{
    // Parametros comuns para os dois lados da superfície
    GLfloat objeto_especular[] = { 0.626, 0.626, 0.626, 1.0 };
    GLfloat objeto_brilho[]    = { 90.0f };
    GLfloat objeto_ambient[]   = { 0.1, 0.1, 0.1, 1.0 };

    // Material da faces 'frente' (amarelo)
    GLfloat objeto_difusa[]    = { r, g, b, 1.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, objeto_difusa);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, objeto_brilho);
}

void Bloco::drawBloco() {
    if (show) {
        glPushMatrix();
            setColor(cor[0],cor[1],cor[2]);
            glTranslatef(origem.x, origem.y, origem.z);
            glScalef(dim,1.0, 1.0);
            glutSolidCube(0.25);
        glPopMatrix();
    }
}


