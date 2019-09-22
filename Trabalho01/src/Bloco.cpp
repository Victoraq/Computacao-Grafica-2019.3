#include "Bloco.h"
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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
bool Bloco::colisao(vertice centro, float vetor_direcao[], float raio) {

    float dist_origem_y = SIZE/2;
    float dist_origem_x = (SIZE*dim)/2;
    bool colidiu_inf = false; // verifica em que parede colidiu
    bool colidiu_sup = false;

    // Variaveis para calculo de distancia entre paredes
    float deltaX, deltaY, dist_inf1, dist_inf2;

    // Parede superior e inferior
    // verifica se está entre os "x" do bloco
    if (centro.x + raio > origem.x - dist_origem_x && origem.x + dist_origem_x > centro.x - raio) {

        if (centro.y + raio > origem.y - dist_origem_y && centro.y - raio < origem.y + dist_origem_y ) {
            // Parede inferior
            if (centro.y+raio < origem.y) {
                vetor_direcao[1] *= -1;
                colidiu_inf = true;
            }
            // Parede superior
            if (centro.y-raio > origem.y) {
                vetor_direcao[1] *= -1;
                colidiu_sup = true;
            }
        }

    }

    // Paredes laterais
    if (centro.y + raio > origem.y - dist_origem_y && centro.y - raio < origem.y + dist_origem_y ) {
        if (centro.x + raio > origem.x - dist_origem_x && origem.x + dist_origem_x > centro.x - raio) {

            // Parede lateral direita
            if (centro.x+raio < origem.x) {
                vetor_direcao[0] *= -1;

                if (colidiu_inf) {
                    // distancia da parede inferior
                    deltaX = origem.x - centro.x;
                    deltaY = origem.y - dist_origem_y - centro.y;
                    dist_inf1 = sqrt(pow(deltaX,2) + pow(deltaY,2));

                    // distancia da parede lateral direita
                    deltaX = origem.x + dist_origem_x - centro.x;
                    deltaY = origem.y - centro.y;
                    dist_inf2 = sqrt(pow(deltaX,2) + pow(deltaY,2));

                    if (dist_inf1 > dist_inf2) {
                        vetor_direcao[1] *= -1; // reverte mudança anterior
                    } else {
                        vetor_direcao[0] *= -1;
                    }

                } else if (colidiu_sup) {
                    // distancia da parede superior
                    deltaX = origem.x - centro.x;
                    deltaY = origem.y + dist_origem_y - centro.y;
                    dist_inf1 = sqrt(pow(deltaX,2) + pow(deltaY,2));

                    // distancia da parede lateral direita
                    deltaX = origem.x + dist_origem_x - centro.x;
                    deltaY = origem.y - centro.y;
                    dist_inf2 = sqrt(pow(deltaX,2) + pow(deltaY,2));

                    if (dist_inf1 > dist_inf2) {
                        vetor_direcao[1] *= -1; // reverte mudança anterior
                    } else {
                        vetor_direcao[0] *= -1;
                    }
                }

                return true;

            }
            // Parede lateral esquerda
            if (centro.x-raio > origem.x) {
                vetor_direcao[0] *= -1;

                if (colidiu_inf) {
                    // distancia da parede inferior
                    deltaX = origem.x - centro.x;
                    deltaY = origem.y - dist_origem_y - centro.y;
                    dist_inf1 = sqrt(pow(deltaX,2) + pow(deltaY,2));

                    // distancia da parede lateral esquerda
                    deltaX = origem.x - dist_origem_x - centro.x;
                    deltaY = origem.y - centro.y;
                    dist_inf2 = sqrt(pow(deltaX,2) + pow(deltaY,2));

                    if (dist_inf1 > dist_inf2) {
                        vetor_direcao[1] *= -1; // reverte mudança anterior
                    } else {
                        vetor_direcao[0] *= -1;
                    }

                } else if (colidiu_sup) {
                    // distancia da parede superior
                    deltaX = origem.x - centro.x;
                    deltaY = origem.y + dist_origem_y - centro.y;
                    dist_inf1 = sqrt(pow(deltaX,2) + pow(deltaY,2));

                    // distancia da parede lateral esquerda
                    deltaX = origem.x - dist_origem_x - centro.x;
                    deltaY = origem.y - centro.y;
                    dist_inf2 = sqrt(pow(deltaX,2) + pow(deltaY,2));

                    if (dist_inf1 > dist_inf2) {
                        vetor_direcao[1] *= -1; // reverte mudança anterior
                    } else {
                        vetor_direcao[0] *= -1;
                    }
                }

                return true;

            }
        }
    }

    return colidiu_inf || colidiu_sup;

}


void Bloco::setShow(bool b)
{
    this->show=b;
}

