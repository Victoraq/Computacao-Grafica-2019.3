#include "Player.h"
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


Player::Player(vertice origem, float cor[])
{
    this->origem = origem;
    this->cor[0] = cor[0];
    this->cor[1] = cor[1];
    this->cor[2] = cor[2];
    this->pontos = new float*[NPONTOS+1];
    for (int i = 0; i < NPONTOS+1; i++) {
        this->pontos[i] = new float[2];
    }
}


/// Rotaciona vetor a partir de um ângulo
void Player::rotacao(float coords[], float angulo) {
    float x = coords[0] * cos(angulo) - coords[1] * sin(angulo);
    float y = coords[0] * sin(angulo) + coords[1] * cos(angulo);

    float norma = sqrt(pow(x, 2)+pow(y, 2));

    coords[0] = x/norma;
    coords[1] = y/norma;
}


void Player::setColor(float r, float g, float b)
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


/// Desenha player
void Player::drawPlayer() {

    glPushMatrix();
        setColor(cor[0],cor[1],cor[2]);
        glTranslatef(origem.x, origem.y, origem.z);
        glPushMatrix();
            glBegin(GL_QUADS);
                setColor(cor[0],cor[1],cor[2]);
                glVertex3f(0.7652,0.6437,-0.25);
                glVertex3f(-0.7652,0.6437,-0.25);
                glVertex3f(-0.7652,0.6437,0.25);
                glVertex3f(0.7652,0.6437,0.25);
            glEnd();

        // Desenhando curvas como um conjunto de quadrados
        float atual[2] = {-0.1,0.0};
        float ant[2] = {-0.1,0.0};
        rotacao(ant, (-40*3.14)/180.0);

        int index=0;
        this->pontosDeConstrucao[index].x=atual[0];
        this->pontosDeConstrucao[index].y=atual[1];
        this->pontosDeConstrucao[index].z=0.25;

        for (int i = 40, j = 0; i <= 140; i+=100/this->NPONTOS, j++) {
            atual[0] = -0.1;
            atual[1] = -0.0;

            rotacao(atual, (-i*3.14)/180.0);

            if(index<=20)
            {
                this->pontosDeConstrucao[index].x=atual[0];
                this->pontosDeConstrucao[index].y=atual[1];
                this->pontosDeConstrucao[index].z=0.25;
                index++;
            }


            glBegin(GL_QUADS);
                setColor(cor[0],cor[1],cor[2]);
                glVertex3f(ant[0],ant[1], 0.25);
                glVertex3f(ant[0],ant[1], -0.25);
                glVertex3f(atual[0],atual[1], -0.25);
                glVertex3f(atual[0],atual[1], 0.25);
            glEnd();

            ant[0] = atual[0];
            ant[1] = atual[1];
            this->pontos[j][0] = atual[0];
            this->pontos[j][1] = atual[1];
        }

        glBegin(GL_POLYGON);

            for(int i = 0; i <= this->NPONTOS; i++) {
                glVertex3f(this->pontos[i][0], this->pontos[i][1], 0.25); // tampa superior
            }

        glEnd();

        glBegin(GL_POLYGON);

            for(int i = 0; i <= this->NPONTOS; i++) {
                glVertex3f(this->pontos[i][0], this->pontos[i][1], -0.25);// tampa inferior
            }

        glEnd();

        glPopMatrix();
    glPopMatrix();
}


/// Calcula se o objeto passado colidiu com o bloco e muda a sua coordenada
bool Player::colisao(vertice centro, float vetor_direcao[], float raio) {



    for(int i = 0; i <= this->NPONTOS; i++) {

    }

}


void Player::imprimePontosDeConstrucao()
{
    for(int i=0; i<=20; i++)
        printf("[%f][%f][%f]\n", pontosDeConstrucao[i].x, pontosDeConstrucao[i].y, pontosDeConstrucao[i].z);
    printf("\n");
}
