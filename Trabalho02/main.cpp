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
Player *player;                     // Bloco do player
Enemy *enemy;                      // Armazena os inimigos
vertice playerCenter = {0.0,-1,0.0}; // Posição do player
float cursor_coords[] = {0.0, 1};  // Coodenadas do cursor
float angulo = 0;                  // Angulo em que o cursor está
bool fullscreen = false;           // Coloca o jogo em fullscreen
bool pause = false;                // Pausa o jogo
bool camera = false;               // Libera movimentação da câmera
bool inicio = false;               // Inicia o jogo
float flipperStep = 0.25;          // Passo de movimentação do player
vertice ball_coords = {0.0,0.0,0.0};  // Coordenadas da bola
float *ball_vector = cursor_coords;   // Vetor de direção da bola
int vidas = 5;                     // Contador de vidas do jogador

/// Functions
void init(void)
{
    initLight(width, height); // Função extra para tratar iluminação.
}

float calculaDistancia(vertice v1, vertice v2)
{
    return sqrt(pow(v1.x-v2.x, 2) + pow(v1.y-v2.y, 2) + pow(v1.z-v2.z, 2));
}

bool colisaoBolaPlayer()
{
    ///Se a distancia entre a origem do player e a origem da bola for <= a soma de seus raios
    ///e a bola esteja acima da parte de baixo do player
    if((calculaDistancia(player->Getorigem(), ball_coords)<=RAIO+1)&&
    ((ball_coords.x>(player->Getorigem().x-0.765))&&(ball_coords.x<(player->Getorigem().x+0.765))))
        return true;
    else
        return false;
}

float calculaNorma(vertice vet)
{
    return sqrt(pow(vet.x, 2) + pow(vet.y, 2) + pow(vet.z, 2));
}

void unitize(vertice vet)
{
    float norma=calculaNorma(vet);
    vet.x/=norma;
    vet.y/=norma;
    vet.z/=norma;
}

float produtoVetorial(vertice v1, vertice v2)
{
    return  (v1.x*v2.x)+(v1.y*v2.y)+(v1.z*v2.z);
}

float calculaAngulo(vertice v1, vertice v2)
{
    return 3.14-acos(produtoVetorial(v1, v2)/(calculaNorma(v1)*calculaNorma(v2)));
}

void rotacao(vertice vet, float angulo)
{
    vet.x=(cos(angulo)*vet.x)-(sin(angulo)*vet.y);
    vet.y=(sin(angulo)*vet.x)+(cos(angulo)*vet.y);
}

void encontraDoisPontosMaisProximos(vertice* pontos, vertice bola, vertice* p1, vertice* p2)
{
    int i;
    for(i=0; i<=20; i++)
    {
        if(bola.x > pontos[i].x)
            break;
    }
    *p1=pontos[i-1];
    *p2=pontos[i];
    /*
    if(calculaDistancia(bola, pontos[0])<=calculaDistancia(bola, pontos[1]))
    {
        vertice p1=pontos[0];
        vertice p2=pontos[1];
    }
    else
    {
        vertice p1=pontos[1];
        vertice p2=pontos[0];
    }
    for(int i=2; i<=20; i++)
    {
        if(calculaDistancia(bola, pontos[i])<=calculaDistancia(bola, *p1))
        {
            p2=p1;
            *p1=pontos[i];
        }
        else if(calculaDistancia(bola, pontos[i])<=calculaDistancia(bola, *p2))
        {
            *p2=pontos[i];
        }
    }
    */
}

vertice calculaNormalDeColisao()
{
    vertice p1, p2, p3;
    //encontraDoisPontosMaisProximos()
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
        glTranslatef(-3.5,2.0,0.0);
        glScalef(1,25.0, 1.0);
        glutSolidCube(0.25);
    glPopMatrix();

    glPushMatrix();
        setColor(0.765, 0.796, 0.851);
        glTranslatef(3.5,2.0,0.0);
        glScalef(1,25.0, 1.0);
        glutSolidCube(0.25);
    glPopMatrix();

    glPushMatrix();
        setColor(0.765, 0.796, 0.851);
        glTranslatef(0,-1.25,0.0);
        glScalef(29,1.0, 1.0);
        glutSolidCube(0.25);
    glPopMatrix();

    glPushMatrix();
        setColor(0.765, 0.796, 0.851);
        glTranslatef(0,5.25,0.0);
        glScalef(29,1.0, 1.0);
        glutSolidCube(0.25);
    glPopMatrix();
}


void colisaoParedes(void) {
    //Parede superior
    if (ball_coords.y+0.26 >= 5.25)
        ball_vector[1] *= -1;

    // Parede inferior
    if (ball_coords.y-0.26 <= -1.25) {
        ball_vector[1] *= -1;
        vidas--;
        if (vidas > 0)
            reset(false);  // Se colidir com a inferior o jogo é resetado
        else
            reset(true);
    }

    // Parede lateral direita
    if (ball_coords.x+0.26 >= 3.5)
        ball_vector[0] *= -1;

    // Parede lateral esquerda
    if (ball_coords.x-0.26 <= -3.5)
        ball_vector[0] *= -1;
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
            glTranslatef(4, i*0.5, 0.0); // Posicionamento inicial da esfera
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

    gluLookAt (0.0, proj_vision, zdist, 0.0, 3.0, 0.0, 0.0, 1.0, 0.0);

    glPushMatrix();
        glRotatef( rotationY, 0.0, 1.0, 0.0 );
        glRotatef( rotationX, 1.0, 0.0, 0.0 );

        setMaterials();
        drawCampo(); // Campo

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

        // Termina o jogo se terminarem os inimigos
        if (enemy->numberOfEnemies() == 0)
            reset(true);

    glPopMatrix();

    if(enemy->getEnemiesOnScreen()==0)
    {
        reset(true);
    }

    ///printf("\nDistancia: %f", calculaDistancia(player->Getorigem(), ball_coords));
    ///printf("\nBall Y: %f\n", ball_coords.y);
    player->imprimePontosDeConstrucao();
    if(colisaoBolaPlayer())
        reset(false);

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
    }

    player->drawPlayer(); // Desenha o player

    enemy->drawEnemies(); // Desenha todos os blocos inimigos

    player->colisao(ball_coords, ball_vector, RAIO);

    enemy->colisao(ball_coords, ball_vector, RAIO);

    colisaoParedes();

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
    if(x>550)
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
    if (x < mouseX && playerCenter.x-flipperStep > -2.5 ) {
        playerCenter.x = playerCenter.x-flipperStep;
        player->Setorigem(playerCenter);
        if (!inicio)
            ball_coords.x = playerCenter.x;
    }
    if (mouseX < x && playerCenter.x+flipperStep < 2.5 ) {
        playerCenter.x = playerCenter.x+flipperStep;
        player->Setorigem(playerCenter);
        if (!inicio)
            ball_coords.x = playerCenter.x;
    }
    mouseX = x; // atualiza a posição do mouse

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
