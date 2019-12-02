#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <cstdlib>
#include <time.h>
#include <math.h>

#include "extras.h"
#include "Enemy.h"
#include "Player.h"
#include "EnemyRandom.h"
#include "Utils.h"
#include "CurvaLateral.h"

/// Estruturas iniciais para armazenar vertices
//  Você poderá utilizá-las adicionando novos métodos (de acesso por exemplo) ou usar suas próprias estruturas.

/// Constantes
const int QUANT_ENEMY = 45;        // Quantidade de inimigos a ser mostrado
const float RAIO = 0.05;           // Raio da bola
const float VELOCIDADE = 7;        // Velocidade da bola

/// Globals
float zdist = 5.0;
float rotationX = 0.0, rotationY = 0.0;
int   last_x, last_y;
int   width, height;
int mouseX = 500;                  // Posição do mouse
int proj = 1;                      // Indica em que projeção será exibido
Player *player;                    // Bloco do player
Enemy *enemy;                      // Armazena os inimigos blocos
EnemyRandom *randomEnemy;          // Inimigo criado com movimentação random
vertice playerCenter = {0.0,-1.1,0.0}; // Posição do player
float cursor_coords[] = {0.0, 1};  // Coodenadas do cursor
float angulo = 0;                  // Angulo em que o cursor está
bool fullscreen = false;           // Coloca o jogo em fullscreen
bool pause = false;                // Pausa o jogo
bool camera = false;               // Libera movimentação da câmera
bool inicio = false;               // Inicia o jogo
float flipperStep = 0.2;          // Passo de movimentação do player
vertice ball_coords = {0.0,0.0,0.0};  // Coordenadas da bola
float *ball_vector = cursor_coords;   // Vetor de direção da bola
int vidas = 5;                     // Contador de vidas do jogador
int fase = 0;                      // determina em que fase o player esta
bool telaInicial = true;           // Determina se ira desenhar a tela inicial ou não
glcTexture *textureManager;        // manager das texturas
CurvaLateral *curva;
Utils utility;

/// Functions
void init(void)
{
    initLight(rotationX, rotationY); // Função extra para tratar iluminação.

    glEnable(GL_ALPHA_TEST);      // O alpha test descarta fragmentos dependendo de uma comparação (abaixo)
    glAlphaFunc(GL_GREATER, 0.1); // Info: https://www.opengl.org/sdk/docs/man2/xhtml/glAlphaFunc.xml

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // https://www.opengl.org/sdk/docs/man/html/glBlendFunc.xhtml

    textureManager = new glcTexture();            // Criação do arquivo que irá gerenciar as texturas
    textureManager->SetNumberOfTextures(7);       // Estabelece o número de texturas que será utilizado
    textureManager->CreateTexture("textures/telaInicialTexto.png", 0); // Para testar magnificação, usar a imagem marble128
    textureManager->CreateTexture("textures/skybox/cwd_bk.png", 1);
    textureManager->CreateTexture("textures/skybox/cwd_dn.png", 2);
    textureManager->CreateTexture("textures/skybox/cwd_ft.png", 3);
    textureManager->CreateTexture("textures/skybox/cwd_lf.png", 4);
    textureManager->CreateTexture("textures/skybox/cwd_rt.png", 5);
    textureManager->CreateTexture("textures/skybox/cwd_up.png", 6);
    textureManager->CreateTexture("textures/cinza.png", 7);
    textureManager->CreateTexture("textures/roxo.png", 8);
}


void drawTelaInicial() {

    glPushMatrix();
    float aspectRatio = textureManager->GetAspectRatio(0);

    // Calculo abaixo funciona apenas se textura estiver centralizada na origem
    int h = height/100;
    int w = width/100;

//    if( aspectRatio > 1.0f) h  = h/aspectRatio;
//    else                    w  = w*aspectRatio;

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Mudando projeção */
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();

    // Mudança entre visão perspectiva e ortogonal
    int ortho = 5;
    int proj_vision; // muda o y para ajustar a camera durante a mudança de perspectiva

    if (width <= height)
        glOrtho (-ortho, ortho, -ortho*h/w, ortho*h/w, -100.0, 100.0);
    else
        glOrtho (-ortho*w/h, ortho*w/h, -ortho, ortho, -100.0, 100.0);
    proj_vision = 3.5;

    /* Camera e desenhos */

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // mudando a posicao da luz em relacao a posicao da camera

    GLfloat posicao_luz[] = { (float) 0, (float)0, 1000.0, 1.0};
    GLfloat cor_luz[]     = { 1.0, 1.0, 1.0, 1.0};

    // Define parametros da luz
    // Enable/Disable features
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);

    // Just in case we set all vertices to white.
    glColor4f(1,1,1,1);

    gluLookAt (0.0, proj_vision, zdist, 0.0, 3.0, 0.0, 0.0, 1.0, 0.0);

    textureManager->Bind(0);
    glBegin(GL_QUADS);
        glColor4f(1,1,1,1);
        glNormal3f(0,0,1);
        glTexCoord2f(0,0);
        glVertex3f(-w,-h+3,-1);
        glTexCoord2f(1, 0);
        glVertex3f(w, -h+3, -1);
        glTexCoord2f(1,1);
        glVertex3f(w,h+3,-1);
        glTexCoord2f(0,1);
        glVertex3f(-w,h+3,-1);
    glEnd();
    textureManager->Disable();
    glPopAttrib();
    glPopMatrix();
}


void drawSkyBox() {
    // Store the current matrix
    glPushMatrix();

    // Reset and transform the matrix.
//    glLoadIdentity();
//    gluLookAt(
//        0,0,0,
//        0,0,0,
//        0,1,0);
//
    // Enable/Disable features
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);

    // Just in case we set all vertices to white.
    glColor4f(1,1,1,1);

    // Render the front quad
    textureManager->Bind(3);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(  10, -10, -10 );
        glTexCoord2f(1, 0); glVertex3f(  10,  10, -10 );
        glTexCoord2f(1, 1); glVertex3f( -10,  10, -10 );

        glTexCoord2f(0, 1); glVertex3f( -10, -10, -10);
    glEnd();

    // Render the left quad
    textureManager->Bind(4);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(  10, -10,  10 );
        glTexCoord2f(1, 0); glVertex3f(  10, -10, -10 );
        glTexCoord2f(1, 1); glVertex3f(  10,  10, -10 );
        glTexCoord2f(0, 1); glVertex3f(  10,  10,  10 );
    glEnd();

    // Render the back quad
    textureManager->Bind(1);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f( -10, -10,  10 );
        glTexCoord2f(1, 0); glVertex3f(  10, -10,  10 );
        glTexCoord2f(1, 1); glVertex3f(  10,  10,  10 );
        glTexCoord2f(0, 1); glVertex3f( -10,  10,  10 );

    glEnd();

    // Render the right quad
    textureManager->Bind(5);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f( -10, -10, -10 );
        glTexCoord2f(1, 0); glVertex3f( -10, -10,  10 );
        glTexCoord2f(1, 1); glVertex3f( -10,  10,  10 );
        glTexCoord2f(0, 1); glVertex3f( -10,  10, -10 );
    glEnd();

    // Render the top quad
    textureManager->Bind(6);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex3f( -10,  10, -10 );
        glTexCoord2f(0, 0); glVertex3f( -10,  10,  10 );
        glTexCoord2f(1, 0); glVertex3f(  10,  10,  10 );
        glTexCoord2f(1, 1); glVertex3f(  10,  10, -10 );
    glEnd();

    // Render the bottom quad
    textureManager->Bind(2);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f( -10, -10, -10 );
        glTexCoord2f(0, 1); glVertex3f( -10, -10,  10 );
        glTexCoord2f(1, 1); glVertex3f(  10, -10,  10 );
        glTexCoord2f(1, 0); glVertex3f(  10, -10, -10 );
    glEnd();

    // Restore enable bits and matrix
    glPopAttrib();
    glPopMatrix();
}


float calculaDistancia(vertice v1, vertice v2)
{
    return sqrt(pow(v1.x-v2.x, 2) + pow(v1.y-v2.y, 2) + pow(v1.z-v2.z, 2));
}



bool checaColisaoPlayer(vertice origem)
{
    ///Se a distancia entre a origem do player e a origem da bola for <= a soma de seus raios
    ///e a bola esteja acima da parte de baixo do player
    if((calculaDistancia(player->Getorigem(), origem)<=RAIO+1)&&
    ((origem.x>(player->Getorigem().x-0.765))&&(true)))
        return true;
    else
        return false;
}

void encontraDoisPontosMaisProximos(vertice* p1, vertice* p2)
{
    int i;
    *p1=player->pontosDeConstrucao[1];
    *p2=player->pontosDeConstrucao[0];
    for(int i=1; i<21; i++)
    {
        *p1=player->pontosDeConstrucao[i-1];
        *p2=player->pontosDeConstrucao[i];
        if(ball_coords.x < player->pontosDeConstrucao[i].x)
            break;
    }
}

/// Retorna o jogo para o estado inicial
void reset(bool newgame)
{
    angulo = 0.0;
    cursor_coords[0] = 0.0;
    cursor_coords[1] = 1.0;
    ball_coords = {0, 0, 0};
    player->Setorigem({0.0,-1,0.0});
    player->drawPlayer();
    randomEnemy->resetEnemies();

    if (newgame) {
        enemy->resetEnemies();
        vidas = 5;
    }

    inicio=false;
}


/// Rotaciona vetor a partir de um ângulo
void rotacao(float coords[], float angulo) {
    float x = coords[0] * cos(angulo) - coords[1] * sin(angulo);
    float y = coords[0] * sin(angulo) + coords[1] * cos(angulo);

    float norma = sqrt(pow(x, 2)+pow(y, 2));

    coords[0] = x/norma;
    coords[1] = y/norma;
}


/// Desenha o as paredes que delimitam o campo
void drawCampo(void) {

    // Paredes
    // esquerda superior
    glPushMatrix();
        setColor(0.765, 0.796, 0.851);
        textureManager->Bind(7);
        glBegin(GL_QUADS);
            glNormal3f(0.0,0.0,1.0);
            glTexCoord2f(0,0.0);
            glVertex3f(-4.15,-1.12,0.13);
            glTexCoord2f(0.08,0.0);
            glVertex3f(-3.865,-1.12,0.13);
            glTexCoord2f(0.08,1.0);
            glVertex3f(-3.865,6.04,0.13);
            glTexCoord2f(0,1.0);
            glVertex3f(-4.15,6.04,0.13);
        glEnd();
        textureManager->Disable();
    glPopMatrix();

    // direita superior
    glPushMatrix();
        setColor(0.765, 0.796, 0.851);
        textureManager->Bind(7);
        glBegin(GL_QUADS);
            glNormal3f(0.0,0.0,1.0);
            glTexCoord2f(0,0.0);
            glVertex3f(3.865,-1.12,0.13);
            glTexCoord2f(0.08,0.0);
            glVertex3f(4.15,-1.12,0.13);
            glTexCoord2f(0.08,1.0);
            glVertex3f(4.15,6.04,0.13);
            glTexCoord2f(0,1.0);
            glVertex3f(3.865,6.04,0.13);
        glEnd();
        textureManager->Disable();
    glPopMatrix();

    // esquerda inferior
    glPushMatrix();
        setColor(0.765, 0.796, 0.851);
        textureManager->Bind(7);
        glBegin(GL_QUADS);
            glNormal3f(0.0,0.0,-1.0);
            glTexCoord2f(0,0.0);
            glVertex3f(-4.15,-1.12,-0.13);
            glTexCoord2f(0,1.0);
            glVertex3f(-4.15,6.04,-0.13);
            glTexCoord2f(0.08,1.0);
            glVertex3f(-3.865,6.04,-0.13);
            glTexCoord2f(0.08,0.0);
            glVertex3f(-3.865,-1.12,-0.13);
        glEnd();
        textureManager->Disable();
    glPopMatrix();

    //direita inferior
    glPushMatrix();
        setColor(0.765, 0.796, 0.851);
        textureManager->Bind(7);
        glBegin(GL_QUADS);
            glNormal3f(0.0,0.0,-1.0);
            glTexCoord2f(0,0.0);
            glVertex3f(4.15,-1.12,-0.13);
            glTexCoord2f(0.08,0.0);
            glVertex3f(3.865,-1.12,-0.13);
            glTexCoord2f(0.08,1.0);
            glVertex3f(3.865,6.04,-0.13);
            glTexCoord2f(0,1.0);
            glVertex3f(4.15,6.04,-0.13);
        glEnd();
        textureManager->Disable();
    glPopMatrix();

    //esquerda lateral externa
    glPushMatrix();
        setColor(0.765, 0.796, 0.851);
        textureManager->Bind(7);
        glBegin(GL_QUADS);
            glNormal3f(-1.0,0.0,0.0);
            glTexCoord2f(0.08,0.0);
            glVertex3f(-4.15,-1.12,0.13);
            glTexCoord2f(0.08,1.0);
            glVertex3f(-4.15,6.04,0.13);
            glTexCoord2f(0.0,1.0);
            glVertex3f(-4.15,6.04,-0.13);
            glTexCoord2f(0.0,0.0);
            glVertex3f(-4.15,-1.12,-0.13);
        glEnd();
        textureManager->Disable();
    glPopMatrix();

    //direita lateral externa
    glPushMatrix();
        setColor(0.765, 0.796, 0.851);
        textureManager->Bind(7);
        glBegin(GL_QUADS);
            glNormal3f(1.0,0.0,0.0);
            glTexCoord2f(0.08,0.0);
            glVertex3f(4.15,-1.12,0.13);
            glTexCoord2f(0.0,0.0);
            glVertex3f(4.15,-1.12,-0.13);
            glTexCoord2f(0.0,1.0);
            glVertex3f(4.15,6.04,-0.13);
            glTexCoord2f(0.08,1.0);
            glVertex3f(4.15,6.04,0.13);
        glEnd();
        textureManager->Disable();
    glPopMatrix();

    //esquerda lateral interna
    glPushMatrix();
        setColor(0.765, 0.796, 0.851);
        textureManager->Bind(8);
        glBegin(GL_QUADS);
            glNormal3f(1.0,0.0,0.0);
            glTexCoord2f(0.08,0.0);
            glVertex3f(-3.865,-1.12,0.13);
            glTexCoord2f(0.0,0.0);
            glVertex3f(-3.865,-1.12,-0.13);
            glTexCoord2f(0.0,1.0);
            glVertex3f(-3.865,6.04,-0.13);
            glTexCoord2f(0.08,1.0);
            glVertex3f(-3.865,6.04,0.13);
        glEnd();
        textureManager->Disable();
    glPopMatrix();

    //direita lateral interna
    glPushMatrix();
        setColor(0.765, 0.796, 0.851);
        textureManager->Bind(8);
        glBegin(GL_QUADS);
            glNormal3f(-1.0,0.0,0.0);
            glTexCoord2f(0.08,0.0);
            glVertex3f(3.865,-1.12,0.13);
            glTexCoord2f(0.08,1.0);
            glVertex3f(3.865,6.04,0.13);
            glTexCoord2f(0.0,1.0);
            glVertex3f(3.865,6.04,-0.13);
            glTexCoord2f(0.0,0.0);
            glVertex3f(3.865,-1.12,-0.13);
        glEnd();
        textureManager->Disable();
    glPopMatrix();

    //superior campo superior
    glPushMatrix();
        setColor(0.765, 0.796, 0.851);
        textureManager->Bind(7);
        glBegin(GL_QUADS);
            glNormal3f(0.0,0.0,1.0);
            glTexCoord2f(0,0.0);
            glVertex3f(4.15,6.04,0.13);
            glTexCoord2f(0.08,0.0);
            glVertex3f(4.15,6.28,0.13);
            glTexCoord2f(0.08,1.0);
            glVertex3f(-4.15,6.28,0.13);
            glTexCoord2f(0,1.0);
            glVertex3f(-4.15,6.04,0.13);
        glEnd();
        textureManager->Disable();
    glPopMatrix();

    //superior campo inferior
    glPushMatrix();
        setColor(0.765, 0.796, 0.851);
        textureManager->Bind(7);
        glBegin(GL_QUADS);
            glNormal3f(0.0,0.0,1.0);
            glTexCoord2f(0,0.0);
            glVertex3f(4.15,-1.38,0.13);
            glTexCoord2f(0.08,0.0);
            glVertex3f(4.15,-1.12,0.13);
            glTexCoord2f(0.08,1.0);
            glVertex3f(-4.15,-1.12,0.13);
            glTexCoord2f(0,1.0);
            glVertex3f(-4.15,-1.38,0.13);
        glEnd();
        textureManager->Disable();
    glPopMatrix();

    //inferior campo superior
    glPushMatrix();
        setColor(0.765, 0.796, 0.851);
        textureManager->Bind(7);
        glBegin(GL_QUADS);
            glNormal3f(0.0,0.0,-1.0);
            glTexCoord2f(0,0.0);
            glVertex3f(4.15,6.04,-0.13);
            glTexCoord2f(0,1.0);
            glVertex3f(-4.15,6.04,-0.13);
            glTexCoord2f(0.08,1.0);
            glVertex3f(-4.15,6.28,-0.13);
            glTexCoord2f(0.08,0.0);
            glVertex3f(4.15,6.28,-0.13);
        glEnd();
        textureManager->Disable();
    glPopMatrix();

    //inferior campo inferior
    glPushMatrix();
        setColor(0.765, 0.796, 0.851);
        textureManager->Bind(7);
        glBegin(GL_QUADS);
            glNormal3f(0.0,0.0,-1.0);
            glTexCoord2f(0.08,0.0);
            glVertex3f(4.15,-1.12,-0.13);
            glTexCoord2f(0,0.0);
            glVertex3f(4.15,-1.38,-0.13);
            glTexCoord2f(0,1.0);
            glVertex3f(-4.15,-1.38,-0.13);
            glTexCoord2f(0.08,1.0);
            glVertex3f(-4.15,-1.12,-0.13);
        glEnd();
        textureManager->Disable();
    glPopMatrix();

    //lateral externa campo superior
    glPushMatrix();
        setColor(0.765, 0.796, 0.851);
        textureManager->Bind(7);
        glBegin(GL_QUADS);
            glNormal3f(0.0,1.0,0.0);
            glTexCoord2f(0.0,0.0);
            glVertex3f(4.15,6.28,0.13);
            glTexCoord2f(0.08,0.0);
            glVertex3f(4.15,6.28,-0.13);
            glTexCoord2f(0.08,1.0);
            glVertex3f(-4.15,6.28,-0.13);
            glTexCoord2f(0,1.0);
            glVertex3f(-4.15,6.28,0.13);
        glEnd();
        textureManager->Disable();
    glPopMatrix();

    //lateral externa campo inferior
    glPushMatrix();
        setColor(0.765, 0.796, 0.851);
        textureManager->Bind(7);
        glBegin(GL_QUADS);
            glNormal3f(0.0,-1.0,0.0);
            glTexCoord2f(0.08,1.0);
            glVertex3f(4.15,-1.38,0.13);
            glTexCoord2f(0.08,0.0);
            glVertex3f(-4.15,-1.38,0.13);
            glTexCoord2f(0.0,0.0);
            glVertex3f(-4.15,-1.38,-0.13);
            glTexCoord2f(0.0,1.0);
            glVertex3f(4.15,-1.38,-0.13);
        glEnd();
        textureManager->Disable();
    glPopMatrix();

    //lateral interna campo superior
    glPushMatrix();
        setColor(0.765, 0.796, 0.851);
        textureManager->Bind(8);
        glBegin(GL_QUADS);
            glNormal3f(0.0,-1.0,0.0);
            glTexCoord2f(0.0,0.0);
            glVertex3f(4.15,6.04,0.13);
            glTexCoord2f(0.08,1.0);
            glVertex3f(-4.15,6.04,-0.13);
            glTexCoord2f(0,1.0);
            glVertex3f(-4.15,6.04,0.13);
            glTexCoord2f(0.08,0.0);
            glVertex3f(4.15,6.04,-0.13);
        glEnd();
        textureManager->Disable();
    glPopMatrix();

    //lateral interna campo inferior
    glPushMatrix();
        setColor(0.765, 0.796, 0.851);
        textureManager->Bind(8);
        glBegin(GL_QUADS);
            glNormal3f(0.0,1.0,0.0);
            glTexCoord2f(0.08,1.0);
            glVertex3f(4.15,-1.12,0.13);
            glTexCoord2f(0.0,1.0);
            glVertex3f(4.15,-1.12,-0.13);
            glTexCoord2f(0.0,0.0);
            glVertex3f(-4.15,-1.12,-0.13);
            glTexCoord2f(0.08,0.0);
            glVertex3f(-4.15,-1.12,0.13);
        glEnd();
        textureManager->Disable();
    glPopMatrix();

    // Saida dos inimigos
    glPushMatrix();
        setColor(0.0, 0.0, 0.653);
        glTranslatef(-2,6.05,0.0);
        glScalef(10,1.0, 1.0);
        glutSolidCube(0.25);
    glPopMatrix();

    glPushMatrix();
        setColor(0.0, 0.0, 0.653);
        glTranslatef(2,6.05,0.0);
        glScalef(10,1.0, 1.0);
        glutSolidCube(0.25);
    glPopMatrix();
}


void colisaoParedes(void) {
    float coord_sup = 6.15;
    float coord_inf = -1.0;
    float coord_lat = 4.0;

    //Parede superior
    if (ball_coords.y+0.26 >= coord_sup)
        ball_vector[1] *= -1;

    // Parede inferior
    if (ball_coords.y-0.26 <= coord_inf) {
        ball_vector[1] *= -1;
        vidas--;
        if (vidas > 0)
            reset(false);  // Se colidir com a inferior o jogo é resetado
        else {
            fase = 0;
            enemy->setConf(fase);
            reset(true);
        }
    }

    // Parede lateral direita
    if (ball_coords.x+0.26 >= coord_lat)
        ball_vector[0] *= -1;

    // Parede lateral esquerda
    if (ball_coords.x-0.26 <= -coord_lat)
        ball_vector[0] *= -1;

    // verificando colisao para os inimigos que se movimentam
    randomEnemy->colisaoParedes(coord_sup, coord_inf, coord_lat);
}

void colisaoBolaPlayer(float direction[])
{
    //normalTest();
    vertice p1, p2, p3;
    vertice* normal;
    vertice* obj_direction=new vertice;
    float angulo;

    obj_direction->x=direction[0]; ///copia ball_vector para bola
    obj_direction->y=direction[1];
    obj_direction->z=direction[2];
    utility.unitiza(obj_direction);
    utility.inverte(obj_direction);

    encontraDoisPontosMaisProximos(&p2, &p1); ///pega os pontos e calcula a normal
    p3=p2;
    p3.z*=-1;

    normal=utility.calculaNormal(&p1, &p2, &p3);
    utility.unitiza(normal);


    if(inicio)
    {
        angulo=utility.calculaAnguloRotacao(obj_direction, normal); ///calcula o angulo e gira o vetor
        utility.rotaciona(obj_direction, angulo);
        obj_direction->z=0.25;
        utility.unitiza(obj_direction);
    }

    direction[0]=obj_direction->x; ///copia bola atualizado para ball_vector
    direction[1]=obj_direction->y;
    direction[2]=obj_direction->z;

    //delete bola; ///desaloca os dinamicos
    //delete normal;
}

/// Desenha cursor de direção da bola
void drawCursor(float x, float y) {
    // Cursor de direção da bolsa
    glTranslatef(x, y, 0.0);
    glBegin(GL_QUADS);
        glVertex3f (0.1, 0.0, 0.0);
        glVertex3f (cursor_coords[0]+RAIO, cursor_coords[1], 0.0);
        glVertex3f (cursor_coords[0]-RAIO, cursor_coords[1], 0.0);
        glVertex3f (-0.1, 0.0, 0.0);
    glEnd();
}


/// Desenha quantidade de vidas
void drawVidas() {

    for (int i = 0; i < vidas; i++) {
        glPushMatrix();
            setColor(1.0, 0.0, 0.0);
            glTranslatef(4.5, i*0.5, 0.0); // Posicionamento inicial da esfera
            glutSolidSphere(RAIO*2, 100, 100);
        glPopMatrix();
    }

}


void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Mudando projeção */
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();

    // Mudança entre visão perspectiva e ortogonal
    int h = height;
    int w = width;
    int ortho = 5;
    int proj_vision; // muda o y para ajustar a camera durante a mudança de perspectiva

    if(proj == -1) { // Visão ortogonal
        if (width <= height)
            glOrtho (-ortho, ortho, -ortho*h/w, ortho*h/w, -100.0, 100.0);
        else
            glOrtho (-ortho*w/h, ortho*w/h, -ortho, ortho, -100.0, 100.0);
        proj_vision = 3.5;
    }
    else { // Visão perspectiva
        gluPerspective(60.0f,(GLfloat)width/(GLfloat)height,0.01f,200.0f);	// Calculate The Aspect Ratio Of The Window
        proj_vision = -3.5;
    }

    /* Camera e desenhos */

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // mudando a posicao da luz em relacao a posicao da camera
    GLfloat posicao_luz[] = { (float) rotationX, (float)rotationY, 1000.0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);

    gluLookAt (0.0, proj_vision, zdist, 0.0, 3.0, 0.0, 0.0, 1.0, 0.0);

    // Mudando a configuracao dos inimigos conforme a fase atual
    enemy->setConf(fase);
    if (telaInicial)
        drawTelaInicial();
    else {
        glPushMatrix();
            glRotatef( rotationY, 0.0, 1.0, 0.0 );
            glRotatef( rotationX, 1.0, 0.0, 0.0 );

            setMaterials();
            drawSkyBox();
            drawCampo(); // Campo

            curva->drawCurvaEsquerda();
            curva->drawCurvaDireita();

            player->drawPlayer(); // Desenha o player

            enemy->drawEnemies(); // Desenha todos os blocos inimigos

            drawVidas();

            // Esfera
            glPushMatrix();
                setColor(1.0, 1.0, 1.0);
                glTranslatef(ball_coords.x, ball_coords.y, 0.0); // Posicionamento inicial da esfera
                glutSolidSphere(RAIO*2, 100, 100);
            glPopMatrix();

            // Cursor de direção
            if (!inicio)
                drawCursor(ball_coords.x, ball_coords.y);
            else
                randomEnemy->drawEnemies(true);

        glPopMatrix();

        // Incrementa a fase se terminarem os inimigos
            if (enemy->numberOfEnemies() == 0 && fase < 3) {
                fase++;
                enemy->setConf(fase);
                reset(true);
            } else if (fase >= 3) {
                reset(true);
                fase = 0;
                enemy->setConf(fase);
            }

            // colisao do player com bola
            if(checaColisaoPlayer(ball_coords))
                colisaoBolaPlayer(ball_vector);
    }

    glutSwapBuffers();
}

void idle ()
{
    float t, desiredFrameTime, frameTime;
    static float tLast = 0.0;

    // Get elapsed time
    t = glutGet(GLUT_ELAPSED_TIME);
    // convert milliseconds to seconds
    t /= 1000.0;

    // Calculate frame time
    frameTime = t - tLast;
    // Calculate desired frame time
    desiredFrameTime = 1.0 / (float) (60);

    // Check if the desired frame time was achieved. If not, skip animation.
    if(frameTime <= desiredFrameTime)
        return;

    // **  UPDATE ANIMATION VARIABLES ** //
    float step = 100; // Speed of the animation

    // Movimentação esfera
    if (inicio && !pause) { // Se foi liberada a movimentação as suas coordenadas serão iteradas com base na velocidade
        ball_coords.x+=ball_vector[0]/(step/VELOCIDADE);
        ball_coords.y+=ball_vector[1]/(step/VELOCIDADE);
        randomEnemy->movimenta(step/VELOCIDADE);
    }

    player->drawPlayer(); // Desenha o player

    enemy->drawEnemies(); // Desenha todos os blocos inimigos

    randomEnemy->drawEnemies(!pause); // Desenha novos inimigos se o jogo não estiver pausado

    player->colisao(ball_coords, ball_vector, RAIO);

    enemy->colisao(ball_coords, ball_vector, RAIO);

    randomEnemy->colisaoBola(&ball_coords, ball_vector, RAIO);

    randomEnemy->colisaoBloco(enemy);

    // colisao do player com inimigos que se movimentam
    vector<vertice> enemies_pos = randomEnemy->getPosicoes();
    for (int i = 0; i < enemies_pos.size() ; i++) {

        if (checaColisaoPlayer(enemies_pos.at(i))) {
            randomEnemy->removeEnemy(i);
        }
    }

    colisaoParedes();
    curva->colisaoCurvas(ball_vector, ball_coords, RAIO);
    randomEnemy->colisaoCurvaLateral(curva);

    //Quantidade de vidas
    drawVidas();

    // Update tLast for next time, using static local variable
    tLast = t;
    glutPostRedisplay();
}

void reshape (int w, int h)
{
    width = w;
    height = h;

    glViewport (0, 0, (GLsizei) w, (GLsizei) h);

}

void keyboard (unsigned char key, int x, int y)
{
    if (!telaInicial) {
        switch (tolower(key))
        {
            case 'p': // Mudança de perspectiva
                proj *= -1;
                break;
            case 'r': // Reseta o jogo
                reset(true);
                break;
            case ' ': // Pausa o jogo
                pause = !pause;
                break;
            case 'c': // Libera movimentação da câmera
                camera = !camera;
                pause = camera;
                break;
            case 27:
                exit(0);
                break;
        }
    } else {
        switch (tolower(key))
        {
            case ' ': // Pausa o jogo
                telaInicial = !telaInicial;
                break;
            case 27:
                exit(0);
                break;
        }
    }
}


/// Teclas especiais do teclado
void spk(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_F12:
        //do something here
        glutFullScreen();
        break;
    }
}


// Motion callback
void motion(int x, int y )
{
    if (camera) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        rotationX += (float) (y - last_y);
        rotationY += (float) (x - last_x);

        last_x = x;
        last_y = y;

        // mudanco posicao da luz em relacao a rotacao
        glDisable(GL_LIGHTING);                 // Habilita luz
        glDisable(GL_LIGHT0);                   // habilita luz 0
        glDisable(GL_DEPTH_TEST);

        // mudando a posicao da luz em relacao a posicao da camera
        GLfloat posicao_luz[] = { (float) rotationX, (float)rotationY, 1000.0, 1.0};
        GLfloat cor_luz[]     = { 1.0, 1.0, 1.0, 1.0};

        // Define parametros da luz
        glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz);
        glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz);
        glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);

        glEnable(GL_LIGHTING);                 // Habilita luz
        glEnable(GL_LIGHT0);                   // habilita luz 0
        glEnable(GL_DEPTH_TEST);

        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    }
}

// Mouse callback
void mouse(int button, int state, int x, int y)
{
    // inicia o jogo com o botão esquerdo do mouse
    if (!telaInicial) {
        if (button == GLUT_LEFT_BUTTON && !inicio) {
            inicio = true;
        }
        if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
        {
            last_x = x;
            last_y = y;
        }
        if(button == 3) // Scroll up
        {
            // Scroll up rotaciona o cursor para a esquerda
            if (!inicio) {
                if (angulo > -60) {
                    angulo = ((int) angulo - 2) % 360;
                    rotacao(cursor_coords, (-2*3.14)/180.0);
                }
            }
        }
        if(button == 4) // Scroll Down
        {
            // Scroll down rotaciona o cursor para a direita
            if (!inicio) {
                if (angulo < 60) {
                    angulo = ((int) angulo + 2) % 360;
                    rotacao(cursor_coords, (2*3.14)/180.0);
                }
            }
        }
    } else {
        if (button == GLUT_LEFT_BUTTON) {
            telaInicial = false;
        }
    }
}

/// Realiza a movimentação a partir do mouse
void mouseMoveFlipper(int x, int y)
{
    if (pause)
        return;
    // Retorna o mouse para uma posição, assim o travando
    if(player->Getorigem().x>=3)
    {
        glutWarpPointer(900, 500);
    }
    if(player->Getorigem().x<=-3)
    {
        glutWarpPointer(100, 500);
    }
    if (y != 500) {
        glutWarpPointer(x, 500);
    }
    playerCenter.x=(x/156.25)-3.2;
    player->Setorigem(playerCenter);
}

/// Main
int main(int argc, char** argv)
{
    // Mudando random seed
    srand(time(NULL));

    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (1000, 600);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    init ();

    // Cria bloco do player
    float color[3] = {0.0,1.0,0.0};
    player = new Player(playerCenter,color); // inicializa o player
    enemy = new Enemy(QUANT_ENEMY);       // inicializa os inimigos
    randomEnemy = new EnemyRandom(2);
    curva = new CurvaLateral();
    glutWarpPointer(mouseX, mouseX);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc( mouse );
    glutMotionFunc( motion );
    glutPassiveMotionFunc(mouseMoveFlipper);
    glutSetCursor(GLUT_CURSOR_NONE);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(spk);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
