#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <cstdlib>
#include <time.h>
#include <math.h>

#include "extras.h"
#include "Prisma.h"

/// Estruturas iniciais para armazenar vertices
//  Você poderá utilizá-las adicionando novos métodos (de acesso por exemplo) ou usar suas próprias estruturas.

/// Constantes
const int QUANT_PRISMAS = 5;       // Quantidade máxima de prismas
const float RAIO = 0.1;            // Raio da bola

/// Globals
float zdist = 5.0;
float rotationX = 0.0, rotationY = 0.0;
int   last_x, last_y;
int   width, height;
Prisma *prismas[QUANT_PRISMAS];    // Vetor que irá conter os prismas
int proj = 1;                      // Indica em que projeção será exibido
float cursor_coords[] = {0.0, 1};  // Coodenadas do cursor
float ball_coords[] = {0.0, 0.0};  // Coordenadas da bola
float *ball_vector;                // Vetor de direção da bola
float angulo = 0;                  // Angulo em que o cursor está
float velocidade = 1.0;            // Velocidade do curso/bola
float xvisor_vel = -3;             // Auxilia para ajustar posição do cursor
bool movimenta = false;            // Indica o movimento da bola
bool limite = false;               // auxiliar para indicar que a colisão com a parede inferior pode ser analisada

/// Functions
void init(void)
{
    initLight(width, height); // Função extra para tratar iluminação.
}

/* Exemplo de cálculo de vetor normal que são definidos a partir dos vértices do triângulo;
  v_2
  ^
  |\
  | \
  |  \       'vn' é o vetor normal resultante
  |   \
  +----> v_1
  v_0
*/
void CalculaNormal(triangle t, vertice *vn)
{
    vertice v_0 = t.v[0],
            v_1 = t.v[1],
            v_2 = t.v[2];
    vertice v1, v2;
    double len;

    /* Encontra vetor v1 */
    v1.x = v_1.x - v_0.x;
    v1.y = v_1.y - v_0.y;
    v1.z = v_1.z - v_0.z;

    /* Encontra vetor v2 */
    v2.x = v_2.x - v_0.x;
    v2.y = v_2.y - v_0.y;
    v2.z = v_2.z - v_0.z;

    /* Calculo do produto vetorial de v1 e v2 */
    vn->x = (v1.y * v2.z) - (v1.z * v2.y);
    vn->y = (v1.z * v2.x) - (v1.x * v2.z);
    vn->z = (v1.x * v2.y) - (v1.y * v2.x);

    /* normalizacao de n */
    len = sqrt(pow(vn->x,2) + pow(vn->y,2) + pow(vn->z,2));

    vn->x /= len;
    vn->y /= len;
    vn->z /= len;
}

void drawCampo(void) {

    //Campo
    glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0,0,1);
        glColor3f (1, 1, 0.3);
        glVertex3f (-3.5, 6.5, 0.0);
        glVertex3f (-3.5, -0.5, 0.0);
        glVertex3f (3.5, -0.5, 0.0);
        glVertex3f (3.5, 6.5, 0.0);
    glEnd();

    // Paredes
    glBegin(GL_TRIANGLE_FAN);
        glNormal3f(1,0,0);
        glColor3f (1, 1, 0.3);
        glVertex3f (-3.5, 6.5, 1.0);
        glVertex3f (-3.5, -0.5, 1.0);
        glVertex3f (-3.5, -0.5, 0.0);
        glVertex3f (-3.5, 6.5, 0.0);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
        glNormal3f(-1,0,0);
        glColor3f (1, 1, 0.3);
        glVertex3f (3.5, 6.5, 0.0);
        glVertex3f (3.5, -0.5, 0.0);
        glVertex3f (3.5, -0.5, 1.0);
        glVertex3f (3.5, 6.5, 1.0);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0,1,0);
        glColor3f (1, 1, 0.3);
        glVertex3f (-3.5, -0.5, 1.0);
        glVertex3f (3.5, -0.5, 1.0);
        glVertex3f (3.5, -0.5, 0.0);
        glVertex3f (-3.5, -0.5, 0.0);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0,-1,0);
        glColor3f (1, 1, 0.3);
        glVertex3f (-3.5, 6.5, 0.0);
        glVertex3f (3.5, 6.5, 0.0);
        glVertex3f (3.5, 6.5, 1.0);
        glVertex3f (-3.5, 6.5, 1.0);
    glEnd();
}

/// Gera triangulo aleatório
triangle randomTriangulo(void) {
    // valores do ponto inicial do triangulo
    float xInicial = rand() % 6 - 3;
    float yInicial = rand() % 6 + 1.5;

    vertice v[3] = {{xInicial,yInicial, 1.0},
                {xInicial,yInicial-0.5, 1.0},
                {xInicial-0.5,yInicial-0.25, 1.0}};
    triangle t = {v[0],v[1],v[2]};

    return t;
}

/// Rotaciona vetor a partir de um ângulo
void rotacao(float coords[], float angulo) {
    float x = coords[0] * cos(angulo) - coords[1] * sin(angulo);
    float y = coords[0] * sin(angulo) + coords[1] * cos(angulo);

    float norma = sqrt(pow(x, 2)+pow(y, 2));

    coords[0] = x/norma;
    coords[1] = y/norma;
}

float calculaAngulo(float x1, float y1, float x2, float y2)
{
    float c;
    float a=(x1*x2)+(y1*y2);
    float b1=sqrt(pow(x1, 2)+pow(y1, 2));
    float b2=sqrt(pow(x2, 2)+pow(y2, 2));

    c=((x1*x2)+(y1*y2))/(sqrt(pow(x1, 2)+pow(y1, 2))*sqrt(pow(x2, 2)+pow(y2, 2)));

    if (acos(c) < 3.14/2)
        return 2*acos(c);
    else
        return -2*acos(c);
}

/// Desenha cursor de direção da bola
void drawCursor(void) {
    // Cursor de direção da bolsa
    glBegin(GL_QUADS);
        glVertex3f (0.1, 0.0, 0.1);
        glVertex3f (cursor_coords[0]+RAIO, cursor_coords[1], 0.1);
        glVertex3f (cursor_coords[0]-RAIO, cursor_coords[1], 0.1);
        glVertex3f (-0.1, 0.0, 0.1);
    glEnd();

    // Cursor de velocidade da bola
    glTranslatef(xvisor_vel,-1.0,0.5);
    glScalef(velocidade,1.0, 1.0);
    glutSolidCube(0.25);
}

void colisaoParedes(void) {
    //Parede superior
    if (ball_coords[1]+0.26 >= 6.5)
        ball_vector[1] *= -1;

    // Parede inferior
    if (ball_coords[1]-0.26 <= -0.5 && limite)
        ball_vector[1] *= -1;

    // Parede lateral direita
    if (ball_coords[0]+0.26 >= 3.5)
        ball_vector[0] *= -1;

    // Parede lateral esquerda
    if (ball_coords[0]-0.26 <= -3.5)
        ball_vector[0] *= -1;
}

void colisaoPrismas(void) {
    for (int i = 0; i < QUANT_PRISMAS; i++) {

        // Parede Superior
        if (ball_coords[1]-RAIO <= prismas[i]->topo.v[0].y && ball_coords[1]+RAIO >= prismas[i]->topo.v[2].y) {
            // Produto interno entre a bola e o prisma
            int PI_ball = ball_coords[0] * prismas[i]->normais[1].x +
                          ball_coords[1] * prismas[i]->normais[1].x;

            // Se a bola está dentro do prisma
            if (ball_coords[0]-RAIO <= prismas[i]->topo.v[0].x && ball_coords[0]+RAIO >= prismas[i]->topo.v[2].x) {
                // Produto interno entre ponto inicial do prisma e sua normal
                int PI_prisma = prismas[i]->topo.v[0].x * prismas[i]->normais[1].x +
                                prismas[i]->topo.v[0].y * prismas[i]->normais[1].y;

                // Angulo para realizar a rotação e testar se a colisão foi de fato com esta parede
                float a = calculaAngulo(ball_vector[0], ball_vector[1], prismas[i]->normais[1].x, prismas[i]->normais[1].y);

                if (PI_prisma <= PI_ball && a >= 0) {
//                    ball_vector[1] *= -1;
                    rotacao(ball_vector,a);
                    break;
                }
            }

        }

        // Parede Inferior
        if (ball_coords[1]-RAIO <= prismas[i]->topo.v[2].y && ball_coords[1]+RAIO >= prismas[i]->topo.v[1].y) {
            // Produto interno entre a bola e o prisma
            int PI_ball = ball_coords[0] * prismas[i]->normais[2].x +
                          ball_coords[1] * prismas[i]->normais[2].x;

            // Se a bola está dentro do prisma
            if (ball_coords[0]-RAIO <= prismas[i]->topo.v[1].x && ball_coords[0]+RAIO >= prismas[i]->topo.v[2].x) {
                // Produto interno entre ponto inicial do prisma e sua normal
                int PI_prisma = prismas[i]->topo.v[2].x * prismas[i]->normais[2].x +
                                prismas[i]->topo.v[2].y * prismas[i]->normais[2].y;

                // Angulo para realizar a rotação e testar se a colisão foi de fato com esta parede
                float a = calculaAngulo(ball_vector[0], ball_vector[1], prismas[i]->normais[2].x, prismas[i]->normais[2].y);

                if (PI_prisma >= PI_ball && a >=0) {
                    //ball_vector[1] *= -1;
                    rotacao(ball_vector,a);
                    break;
                }
            }

        }

        // Parede Lateral
        if (ball_coords[1]-RAIO <= prismas[i]->topo.v[0].y && ball_coords[1]+RAIO >= prismas[i]->topo.v[1].y) {
            // Produto interno entre a bola e o prisma
            int PI_ball = ball_coords[0] * prismas[i]->normais[0].x +
                          ball_coords[1] * prismas[i]->normais[0].x;

            // Se a bola está dentro do prisma
            if (ball_coords[0]-RAIO <= prismas[i]->topo.v[0].x && ball_coords[0]+RAIO >= prismas[i]->topo.v[0].x) {
                // Produto interno entre ponto inicial do prisma e sua normal
                int PI_prisma = prismas[i]->topo.v[1].x * prismas[i]->normais[0].x +
                                prismas[i]->topo.v[1].y * prismas[i]->normais[0].y;

                // Angulo para testar se a colisão foi de fato com esta parede
                float a = calculaAngulo(ball_vector[0], ball_vector[1], prismas[i]->normais[0].x, prismas[i]->normais[0].y);

                if (PI_prisma >= PI_ball && a >= 0) {
                    ball_vector[0] *= -1;
                    break;
                }
            }

        }

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
        proj_vision = 3.0;
    }
    else { // Visão perspectiva
        gluPerspective(60.0f,(GLfloat)width/(GLfloat)height,0.01f,200.0f);	// Calculate The Aspect Ratio Of The Window
        proj_vision = -3.0;
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

        //Desenhando os prismas aleatórios
        for (int i = 0; i < QUANT_PRISMAS; i++) {
            prismas[i]->draw();
        }

        // Esfera
        glPushMatrix();
            setColor(0.0, 1.0, 0.0);
            glTranslatef(ball_coords[0], ball_coords[1], 0.35); // Posicionamento inicial da esfera
            glutSolidSphere(RAIO*2, 100, 100);
        glPopMatrix();

        // Cursor de direção e velocidade
        if (!movimenta)
            drawCursor();

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
    if (movimenta) { // Se foi liberada a movimentação as suas coordenadas serão iteradas com base na velocidade
        ball_coords[0]+=ball_vector[0]/(step/velocidade);
        ball_coords[1]+=ball_vector[1]/(step/velocidade);

        // Auxilia para que a colisão com a parede inferior não ocorra no momento do lançamento
        if (!limite && ball_coords[1] >= RAIO*2)
            limite = true;
    }

    // Colisão com as paredes
    colisaoParedes();

    // Coliso Prismas
    colisaoPrismas();

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
        case 'd':
            if (!movimenta) {
                if (angulo > -60) {
                    angulo = ((int) angulo - 2) % 360;
                    rotacao(cursor_coords, (-2*3.14)/180.0);
                }
            }
            break;
        case 'a':
            if (!movimenta) {
                if (angulo < 60) {
                    angulo = ((int) angulo + 2) % 360;
                    rotacao(cursor_coords, (2*3.14)/180.0);
                }
            }
            break;
        case 'w':
            if (!movimenta) {
                if (xvisor_vel < -1.6) {
                    xvisor_vel += 0.0315;
                    velocidade += 0.25;
                }
            }
            break;
        case 's':
            if (!movimenta) {
                if (xvisor_vel > -3) {
                    xvisor_vel -= 0.0315;
                    velocidade -= 0.25;
                }
            }
            break;
        case ' ':
            if (!movimenta) {
                movimenta = true;
                ball_vector = cursor_coords; // bola pega coordenadas do cursor para indicar direção
            }
            break;
        case 27:
            exit(0);
            break;
    }
}

// Motion callback
void motion(int x, int y )
{
    rotationX += (float) (y - last_y);
    rotationY += (float) (x - last_x);

    last_x = x;
    last_y = y;
}

// Mouse callback
void mouse(int button, int state, int x, int y)
{
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
    //gerando prismas a partir de triangulos aleatórios
    for (int i = 0; i < QUANT_PRISMAS; i++) {
        prismas[i] = new Prisma(randomTriangulo());
    }
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc( mouse );
    glutMotionFunc( motion );
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
