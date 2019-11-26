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

    textureManager = new glcTexture();            // Criação do arquivo que irá gerenciar as texturas
    textureManager->SetNumberOfTextures(2);       // Estabelece o número de texturas que será utilizado
    textureManager->CreateTexture("textures/metalWave.png", 0); // Para testar magnificação, usar a imagem marble128
    textureManager->CreateTexture("textures/metalFurado.png", 1); // Textura transparente, não sendo múltipla de zero
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


void Player::setMaterial(int material) {
    // Material utilizado: esmeralda
    // Parametros comuns para os dois lados da superfície
    if (material == 1) {
        GLfloat objeto_especular[] = { 0.633, 0.727811,	0.633, 1.0 };
        GLfloat objeto_brilho[]    = { 60.0f };
        GLfloat objeto_ambient[]   = { 0.0215,	0.1745,	0.0215, 1.0 };

        GLfloat objeto_difusa[]    = { 0.07568,	0.61424, 0.07568, 1.0 };

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, objeto_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, objeto_difusa);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, objeto_especular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, objeto_brilho);
    } else {
        GLfloat objeto_ambient[]   = { 0.2, 0.2, 0.2, 1.0 };
        GLfloat objeto_difusa[]    = { 0.8, 0.8, 0.8, 1.0 };
        GLfloat objeto_especular[] = { 0.0, 0.0, 0.0, 1.0 };

        glMaterialfv(GL_FRONT, GL_AMBIENT, objeto_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, objeto_difusa);
        glMaterialfv(GL_FRONT, GL_SPECULAR, objeto_especular);
    }
}


vertice* Player::calculaNormal(vertice* v1, vertice* v2, vertice* v3)
{

    vertice* vetA=new vertice;
    vertice* vetB=new vertice;

    vetA->x = v1->x-v2->x;
    vetA->y = v1->y-v2->y;
    vetA->z = v1->z-v2->z;

    vetB->x = v3->x-v2->x;
    vetB->y = v3->y-v2->y;
    vetB->z = v3->z-v2->z;

    vertice* vet=new vertice;
    vet->x=((v1->y*v2->z)-(v2->y*v1->z));
    vet->y=((v1->x*v2->z)-(v2->x*v1->z));
    if(vet->y!=0)
        vet->y*=-1;
    vet->z=((v1->x*v2->y)-(v2->x*v1->y));

    return vet;
}


/// Desenha player
void Player::drawPlayer() {

    glPushMatrix();
        textureManager->Bind(0);
        setMaterial(0);
        glTranslatef(origem.x, origem.y, origem.z);
        glPushMatrix();
            glBegin(GL_QUADS);
                setMaterial(0);
                glNormal3f(0.0,-1.0,0.0);
                glTexCoord2f(0.0, 0.35);
                glVertex3f(0.7652,0.6437,-0.25);
                glTexCoord2f(0.0, 0.65);
                glVertex3f(0.7652,0.6437,0.25);
                glTexCoord2f(1.0, 0.65);
                glVertex3f(-0.7652,0.6437,0.25);
                glTexCoord2f(1.0, 0.35);
                glVertex3f(-0.7652,0.6437,-0.25);
            glEnd();

        // Desenhando curvas como um conjunto de quadrados
        float atual[2] = {-0.1,0.0};
        float ant[2] = {-0.1,0.0};
        rotacao(ant, (-40*3.14)/180.0);

        float passoText = 1.0/(this->NPONTOS-4);
        float texture_pos = 0;

        textureManager->Bind(0);
        for (int i = 40, j = 0; i <= 140; i+=100/this->NPONTOS, j++) {
            atual[0] = -0.1;
            atual[1] = -0.0;

            rotacao(atual, (-i*3.14)/180.0);
            glBegin(GL_QUADS);
                setMaterial(0);

                vertice v1 = {ant[0],ant[1], 0.25};
                vertice v2 = {atual[0],atual[1], 0.25};
                vertice v3 = {atual[0],atual[1], -0.25};
                vertice* normal = calculaNormal(&v1,&v2,&v3);
                glNormal3f(normal->x, 1.0, 0.0);


                glTexCoord2f(texture_pos,texture_pos);
                glVertex3f(atual[0],atual[1], -0.25);
                glTexCoord2f(texture_pos+passoText,texture_pos);
                glVertex3f(ant[0],ant[1], -0.25);
                glTexCoord2f(texture_pos+passoText,texture_pos+passoText);
                glVertex3f(ant[0],ant[1], 0.25);
                glTexCoord2f(texture_pos,texture_pos+passoText);
                glVertex3f(atual[0],atual[1], 0.25);
            glEnd();

            ant[0] = atual[0];
            ant[1] = atual[1];
            this->pontos[j][0] = atual[0];
            this->pontos[j][1] = atual[1];

            this->pontosDeConstrucao[j]=*iniciaVetor(atual[0]+origem.x, atual[1]+origem.y, 0.25);
            texture_pos += passoText;
        }

        textureManager->Bind(1);
        glBegin(GL_POLYGON);
            glNormal3f(0.0,0.0,1.0);
            for(int i = this->NPONTOS; i >= 0; i--) {
                glTexCoord2f((this->pontos[i][0]+1)/2, this->pontos[i][1]);
                glVertex3f(this->pontos[i][0], this->pontos[i][1], 0.25); // tampa superior
            }
            printf("\n");
        glEnd();

        glBegin(GL_POLYGON);
            glNormal3f(0.0,0.0,-1.0);
            for(int i = 0; i <= this->NPONTOS; i++) {
                glTexCoord2f((this->pontos[i][0]+1)/2, this->pontos[i][1]);
                glVertex3f(this->pontos[i][0], this->pontos[i][1], -0.25);// tampa inferior
            }

        glEnd();

        glPopMatrix();
    glPopMatrix();
    textureManager->Disable();
}


/// Calcula se o objeto passado colidiu com o bloco e muda a sua coordenada
bool Player::colisao(vertice centro, float vetor_direcao[], float raio) {



    for(int i = 0; i <= this->NPONTOS; i++) {

    }

}
