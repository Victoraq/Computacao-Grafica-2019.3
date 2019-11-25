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
#include "CurvaEsquerda.h"
#include "Utils.h"

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
CurvaEsquerda curva;
Utils utility;

/// Functions
void init(void)
{
    initLight(rotationX, rotationY); // Função extra para tratar iluminação.
}
/*
float calculaDistancia(vertice v1, vertice v2)
{
    return sqrt(pow(v1.x-v2.x, 2) + pow(v1.y-v2.y, 2) + pow(v1.z-v2.z, 2));
}*/

bool checaColisaoPlayer(vertice origem)
{
    ///Se a distancia entre a origem do player e a origem da bola for <= a soma de seus raios
    ///e a bola esteja acima da parte de baixo do player
    if((utility.calculaDistancia(player->Getorigem(), origem)<=RAIO+1)&&
    ((origem.x>(player->Getorigem().x-0.765))&&(true)))
        return true;
    else
        return false;
}

bool checaColisaoCurvaEsquerda()
{
    if(utility.calculaDistancia(ball_coords, curva.getOrigemE())<=RAIO+curva.getRaio())
        return true;
    else
        return false;
}

bool checaColisaoCurvaDireita()
{
    if(utility.calculaDistancia(ball_coords, curva.getOrigemD())<=RAIO+curva.getRaio())
        return true;
    else
        return false;
}

void colisaoCurvas()
{
    vertice p1, p2, p3, normal, ball={ball_vector[0], ball_vector[1], ball_vector[2]};
    float angulo;
    if(checaColisaoCurvaEsquerda())
    {
        curva.encontraDoisPontosMaisProximosE(ball, &p1, &p2);
        p3=p2;
        p3.z*=-1;
        normal=*utility.calculaNormal(&p1, &p2, &p3);
        utility.unitiza(&normal);

        angulo=utility.calculaAngulo(&ball, &normal);
        utility.rotaciona(&ball, angulo);

        ball_vector[0]=ball.x;
        ball_vector[1]=ball.y;
        ball_vector[2]=ball.z;
    }
    else if(checaColisaoCurvaDireita())
    {
        curva.encontraDoisPontosMaisProximosD(ball, &p1, &p2);
        p3=p2;
        p3.z*=-1;
        normal=*utility.calculaNormal(&p1, &p2, &p3);
        utility.unitiza(&normal);

        angulo=utility.calculaAngulo(&ball, &normal);
        utility.rotaciona(&ball, angulo);

        ball_vector[0]=ball.x;
        ball_vector[1]=ball.y;
        ball_vector[2]=ball.z;
    }
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
/*
vertice* iniciaVetor(float x, float y, float z)
{
    vertice* vet=new vertice;
    vet->x=x;
    vet->y=y;
    vet->z=z;

    return vet;
}

vertice* produtoVetorial(vertice* v1, vertice* v2)
{
    vertice* vet=new vertice;
    vet->x=((v1->y*v2->z)-(v2->y*v1->z));
    vet->y=((v1->x*v2->z)-(v2->x*v1->z));
    if(vet->y!=0)
        vet->y*=-1;
    vet->z=((v1->x*v2->y)-(v2->x*v1->y));
    return vet;
}

vertice* calculaNormal(vertice* v1, vertice* v2, vertice* v3)
{

    vertice* vetA=new vertice;
    vertice* vetB=new vertice;

    vetA->x = v1->x-v2->x;
    vetA->y = v1->y-v2->y;
    vetA->z = v1->z-v2->z;

    vetB->x = v3->x-v2->x;
    vetB->y = v3->y-v2->y;
    vetB->z = v3->z-v2->z;

    vertice*c=produtoVetorial(vetA, vetB);

    return c;
}

float RadianosParaGraus(float valor)
{
    return (valor*180)/3.14;
}

float GrausParaRadianos(float valor)
{
    return (valor*3.14159265)/180;
}

float produtoEscalar(vertice* v1, vertice* v2)
{
    return ((v1->x*v2->x)+(v1->y*v2->y)+(v1->z*v2->z));
}

float norma(vertice* vet)
{
    return sqrt(pow(vet->x, 2) + pow(vet->y, 2) + pow(vet->z, 2));
}

void unitiza(vertice* vet)
{
    int vel=1;
    float n=norma(vet);
    vet->x/=n;
    vet->y/=n;
    vet->z/=n;

    vet->x*=vel;
    vet->y*=vel;
    vet->z*=vel;

}


float calculaAngulo(vertice* v1, vertice* v2)
{
    float v =(produtoEscalar(v1, v2))/(norma(v1)*norma(v2));
    //v=3.14-v;
    v=GrausParaRadianos(v);
    float a = RadianosParaGraus(acos(v));
    if(a<90)
        return a;
    else
        (180-a);
}

vertice* rotaciona(vertice* vet, float angulo)
{
    float a, b;

    a = (vet->x*cos(angulo) - vet->y*sin(angulo));
    b = (vet->x*sin(angulo) + vet->y*cos(angulo));

    vet->x=a;
    vet->y=b;

    return vet;
}*/

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
    glPushMatrix();
        setColor(0.765, 0.796, 0.851);
        glTranslatef(-4,2.5,0.0);
        glScalef(1,30.0, 1.0);
        glutSolidCube(0.25);
    glPopMatrix();

    glPushMatrix();
        setColor(0.765, 0.796, 0.851);
        glTranslatef(4,2.5,0.0);
        glScalef(1,30.0, 1.0);
        glutSolidCube(0.25);
    glPopMatrix();

    glPushMatrix();
        setColor(0.765, 0.796, 0.851);
        glTranslatef(0,-1.25,0.0);
        glScalef(33,1.0, 1.0);
        glutSolidCube(0.25);
    glPopMatrix();

    glPushMatrix();
        setColor(0.765, 0.796, 0.851);
        glTranslatef(0,6.15,0.0);
        glScalef(33,1.0, 1.0);
        glutSolidCube(0.25);
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
    ball_coords.y+=0.02;
    vertice p1, p2, p3;
    vertice* normal;
    vertice* obj_direction=new vertice;
    float angulo;

    obj_direction->x=direction[0]; ///copia ball_vector para bola
    obj_direction->y=direction[1];
    obj_direction->z=direction[2];
    utility.unitiza(obj_direction);

    encontraDoisPontosMaisProximos(&p2, &p1); ///pega os pontos e calcula a normal
    p3=p2;
    p3.z*=-1;

    normal=utility.calculaNormal(&p1, &p2, &p3);
    utility.unitiza(normal);


    if(inicio)
    {
        angulo=utility.calculaAngulo(obj_direction, normal); ///calcula o angulo e gira o vetor
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

/*
void drawCurvaEsquerda()
{
    float raio=5;
    vertice origem={-8.4, 2, 0.1};
    vertice *p=new vertice{raio, 0, 0};
    p=rotaciona(p, GrausParaRadianos(-25));
    vertice pontosConstrucao[11];
    pontosConstrucao[0]=*p;
    for(int i=1; i<11; i++)
    {
        pontosConstrucao[i]=*rotaciona(p, GrausParaRadianos(5));
        printf("[%f][%f][%f]\n", pontosConstrucao[i].x, pontosConstrucao[i].y,  pontosConstrucao[i].z);
    }

    for(int i=0; i<11; i++)
    {
        pontosConstrucao[i].x+=origem.x;
        pontosConstrucao[i].y+=origem.y;
        pontosConstrucao[i].z+=origem.z;
    }

    for(int i=0; i<10; i++)
    {
        glBegin(GL_QUADS);
            glVertex3f(pontosConstrucao[i].x, pontosConstrucao[i].y, -1*pontosConstrucao[i].z);
            glVertex3f(pontosConstrucao[i+1].x, pontosConstrucao[i+1].y, -1*pontosConstrucao[i].z);
            glVertex3f(pontosConstrucao[i+1].x, pontosConstrucao[i+1].y, pontosConstrucao[i].z);
            glVertex3f(pontosConstrucao[i].x, pontosConstrucao[i].y, pontosConstrucao[i].z);
        glEnd();

        glBegin(GL_POLYGON);
            for(int i=0; i<11; i++)
                glVertex3f(pontosConstrucao[i].x, pontosConstrucao[i].y, pontosConstrucao[i].z);
        glEnd();

        glBegin(GL_POLYGON);
            for(int i=0; i<11; i++)
                glVertex3f(pontosConstrucao[i].x, pontosConstrucao[i].y, -1*pontosConstrucao[i].z);
        glEnd();
    }

}


void drawCurvaDireita()
{
    float raio=5;
    vertice origem={8.4, 2, 0.1};
    vertice *p=new vertice{raio, 0, 0};
    p=rotaciona(p, GrausParaRadianos(-25));
    vertice pontosConstrucao[11];
    pontosConstrucao[0]=*p;
    for(int i=1; i<11; i++)
    {
        pontosConstrucao[i]=*rotaciona(p, GrausParaRadianos(5));
    }

    for(int i=0; i<11; i++)
        pontosConstrucao[i].x*=-1;

    for(int i=0; i<11; i++)
    {
        pontosConstrucao[i].x+=origem.x;
        pontosConstrucao[i].y+=origem.y;
        pontosConstrucao[i].z+=origem.z;
    }

    for(int i=0; i<10; i++)
    {
        glBegin(GL_QUADS);
            glVertex3f(pontosConstrucao[i].x, pontosConstrucao[i].y, -1*pontosConstrucao[i].z);
            glVertex3f(pontosConstrucao[i+1].x, pontosConstrucao[i+1].y, -1*pontosConstrucao[i].z);
            glVertex3f(pontosConstrucao[i+1].x, pontosConstrucao[i+1].y, pontosConstrucao[i].z);
            glVertex3f(pontosConstrucao[i].x, pontosConstrucao[i].y, pontosConstrucao[i].z);
        glEnd();

        glBegin(GL_POLYGON);
            for(int i=0; i<11; i++)
                glVertex3f(pontosConstrucao[i].x, pontosConstrucao[i].y, pontosConstrucao[i].z);
        glEnd();

        glBegin(GL_POLYGON);
            for(int i=0; i<11; i++)
                glVertex3f(pontosConstrucao[i].x, pontosConstrucao[i].y, -1*pontosConstrucao[i].z);
        glEnd();
    }

}*/


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

    glPushMatrix();
        glRotatef( rotationY, 0.0, 1.0, 0.0 );
        glRotatef( rotationX, 1.0, 0.0, 0.0 );

        setMaterials();
        drawCampo(); // Campo

        //curva.drawCurvaEsquerda();
        curva.drawCurvaEsquerda();
        curva.drawCurvaDireita();

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

    randomEnemy->colisaoBola(ball_coords, ball_vector, RAIO);

    randomEnemy->colisaoBloco(enemy);

    // colisao do player com inimigos que se movimentam
        vector<vertice> enemies_pos = randomEnemy->getPosicoes();
        for (int i = 0; i < enemies_pos.size() ; i++) {

            if (checaColisaoPlayer(enemies_pos.at(i))) {
                randomEnemy->removeEnemy(i);
            }
        }

    colisaoParedes();
    colisaoCurvas();

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
        rotationX += (float) (y - last_y);
        rotationY += (float) (x - last_x);

        last_x = x;
        last_y = y;
    }
}

// Mouse callback
void mouse(int button, int state, int x, int y)
{
    // inicia o jogo com o botão esquerdo do mouse
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
}

/// Realiza a movimentação a partir do mouse
void mouseMoveFlipper(int x, int y)
{
    if (pause)
        return;

    // Retorna o mouse para uma posição, assim o travando
    /*if(x>550)
    {
        glutWarpPointer(500, 500);
    }
    if(x<450)
    {
        glutWarpPointer(500, 500);
    }
    if (y != 500) {
        glutWarpPointer(x, 500);
    }

    // verifica a movimentação a partir da mudança de direção do mouse
    /*
    if (x < mouseX && playerCenter.x-flipperStep > -3.2 ) {
        playerCenter.x = playerCenter.x-flipperStep/2;
        //playerCenter.x = cos((playerCenter.x+90)*3.14/180);
        player->Setorigem(playerCenter);
        if (!inicio)
            ball_coords.x = playerCenter.x;
    }
    if (mouseX < x && playerCenter.x+flipperStep < 3.2 ) {
        playerCenter.x = playerCenter.x+flipperStep/2;
        //playerCenter.x = cos((playerCenter.x+90)*3.14/180);
        player->Setorigem(playerCenter);
        if (!inicio)
            ball_coords.x = playerCenter.x;
    }
    mouseX = x; // atualiza a posição do mouse
    */
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
    glutWarpPointer(mouseX, mouseX);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc( mouse );
    glutMotionFunc( motion );
    glutPassiveMotionFunc(mouseMoveFlipper);
    glutSetCursor(GLUT_CURSOR_NONE);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(spk);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}