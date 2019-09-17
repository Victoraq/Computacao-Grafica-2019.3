#include "Bloco.h"
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

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

/// Desenha o bloco com base na sua cor, dimensão e origem
void Bloco::drawBloco() {
    if (show) {
        glPushMatrix();
            setColor(cor[0],cor[1],cor[2]);
            glTranslatef(origem.x, origem.y, origem.z);
            glScalef(dim,1.0, 1.0);
            glutSolidCube(SIZE);
        glPopMatrix();
    }
}

/// Calcula se o objeto passado colidiu com o bloco e muda a sua coordenada
void Bloco::colisao(vertice centro, float vetor_direcao[], float raio) {

    float dist_origem_y = SIZE/2;
    float dist_origem_x = (SIZE*dim)/2;



    // Parede superior e inferior
    // verifica se está entre os "x" do bloco
    if (centro.x + raio > origem.x - dist_origem_x && origem.x + dist_origem_x > centro.x - raio) {

        if (centro.y + raio > origem.y - dist_origem_y && centro.y - raio < origem.y + dist_origem_y ) {
            // Parede inferior
            if (centro.y+raio < origem.y)
                vetor_direcao[1] *= -1;
            // Parede superior
            if (centro.y-raio > origem.y)
                vetor_direcao[1] *= -1;
        }

    }

    // Paredes laterais

}


