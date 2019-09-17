#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <cstdlib>
#include <time.h>
#include <math.h>

#include "extras.h"
#include "Enemy.h"

/// Estruturas iniciais para armazenar vertices
//  Você poderá utilizá-las adicionando novos métodos (de acesso por exemplo) ou usar suas próprias estruturas.

/// Constantes
const int QUANT_ENEMY = 45;        // Quantidade de inimigos a ser mostrado
const float RAIO = 0.1;            // Raio da bola
const float VELOCIDADE = 5;      // Velocidade da bola

/// Globals
float zdist = 5.0;
float rotationX = 0.0, rotationY = 0.0;
int   last_x, last_y;
int   width, height;
int mouseX = 500;                  // Posição do mouse
int proj = 1;                      // Indica em que projeção será exibido
Bloco *player;                     // Bloco do player
Enemy *enemy;                      // Armazena os inimigos
vertice playerCenter = {0.0,-0.25,0.0}; // Posição do player
bool fullscreen = false;
bool pause = false;                // Pausa o jogo
bool camera = false;               // Libera movimentação da câmera
bool inicio = false;               // Inicia o jogo
float flipperStep=0.25;            // Passo de movimentação do player
vertice ball_coords = {0.0, 0.0, 0.0};  // Coordenadas da bola
//float *ball_vector;                // Vetor de direção da bola
float ball_vector[] = {1.0,0.85};

/// Functions
void init(void)
{
    initLight(width, height); // Função extra para tratar iluminação.
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

        player->drawBloco(); // Desenha o player

        enemy->drawEnemies(); // Desenha todos os blocos inimigos

        // Esfera
        glPushMatrix();
            setColor(1.0, 1.0, 1.0);
            glTranslatef(ball_coords.x, ball_coords.y, 0.0); // Posicionamento inicial da esfera
            glutSolidSphere(RAIO, 100, 100);
        glPopMatrix();


    glPopMatrix();

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
    if (inicio) { // Se foi liberada a movimentação as suas coordenadas serão iteradas com base na velocidade
        ball_coords.x+=ball_vector[0]/(step/VELOCIDADE);
        ball_coords.y+=ball_vector[1]/(step/VELOCIDADE);
    }

    player->drawBloco(); // Desenha o player

    enemy->drawEnemies(); // Desenha todos os blocos inimigos

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
        case 123:
            printf("andoadn");
            glutFullScreen();
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
    if (button == GLUT_LEFT_BUTTON)
        inicio = true;
    if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
    {
        last_x = x;
        last_y = y;
    }
    if(button == 3) // Scroll up
    {
        zdist+=1.0f;
    }
    if(button == 4) // Scroll Down
    {
        zdist-=1.0f;
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
    if (x < mouseX && playerCenter.x-flipperStep > -2.8) {
        playerCenter.x = playerCenter.x-flipperStep;
        player->Setorigem(playerCenter);
    }
    if (mouseX < x && playerCenter.x+flipperStep < 2.8) {
        playerCenter.x = playerCenter.x+flipperStep;
        player->Setorigem(playerCenter);
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
    player = new Bloco(playerCenter,5,color); // inicializa o player
    enemy = new Enemy(QUANT_ENEMY);       // inicializa os inimigos
    glutWarpPointer(mouseX, mouseX);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc( mouse );
    glutMotionFunc( motion );
    glutPassiveMotionFunc(mouseMoveFlipper);
    glutSetCursor(GLUT_CURSOR_NONE);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
