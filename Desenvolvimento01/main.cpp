#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <cstdlib>
#include <time.h>
#include <math.h>

#include "extras.h"

/// Estruturas iniciais para armazenar vertices
//  Você poderá utilizá-las adicionando novos métodos (de acesso por exemplo) ou usar suas próprias estruturas.
class vertice
{
    public:
        float x,y,z;
};

class triangle
{
    public:
        vertice v[3];
};

/// Globals
float zdist = 5.0;
float rotationX = 0.0, rotationY = 0.0;
int   last_x, last_y;
int   width, height;
const int QUANT_PRISMAS = 4;
triangle prismas[QUANT_PRISMAS];
int proj = 1;


/// Functions
void init(void)
{
    initLight(width, height); // Função extra para tratar iluminação.
    setMaterials();
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

void drawObject()
{
    vertice vetorNormal;
    vertice v[4] = {{-1.0f, -1.0f,  0.0f},
                    { 1.0f, -1.0f,  0.0f},
                    {-1.0f,  1.0f,  0.0f},
                    { 1.0f,  1.0f, -0.5f}};

    triangle t[2] = {{v[0], v[1], v[2]},
                     {v[1], v[3], v[2]}};

    glBegin(GL_TRIANGLES);
        for(int i = 0; i < 2; i++) // triangulos
        {
            CalculaNormal(t[i], &vetorNormal); // Passa face triangular e endereço do vetor normal de saída
            glNormal3f(vetorNormal.x, vetorNormal.y,vetorNormal.z);
            for(int j = 0; j < 3; j++) // vertices do triangulo
                glVertex3d(t[i].v[j].x, t[i].v[j].y, t[i].v[j].z);
        }
    glEnd();
}

void drawCampo(void) {

    //Campo
    glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0,0,1);
        glColor3f (1, 1, 0.3);
        glVertex3f (-3.5, 3.5, 0.0);
        glVertex3f (-3.5, -3.5, 0.0);
        glVertex3f (3.5, -3.5, 0.0);
        glVertex3f (3.5, 3.5, 0.0);
    glEnd();

    // Paredes
    glBegin(GL_TRIANGLE_FAN);
        glNormal3f(1,0,0);
        glColor3f (1, 1, 0.3);
        glVertex3f (-3.5, 3.5, 1.0);
        glVertex3f (-3.5, -3.5, 1.0);
        glVertex3f (-3.5, -3.5, 0.0);
        glVertex3f (-3.5, 3.5, 0.0);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
        glNormal3f(-1,0,0);
        glColor3f (1, 1, 0.3);
        glVertex3f (3.5, 3.5, 0.0);
        glVertex3f (3.5, -3.5, 0.0);
        glVertex3f (3.5, -3.5, 1.0);
        glVertex3f (3.5, 3.5, 1.0);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0,1,0);
        glColor3f (1, 1, 0.3);
        glVertex3f (-3.5, -3.5, 1.0);
        glVertex3f (3.5, -3.5, 1.0);
        glVertex3f (3.5, -3.5, 0.0);
        glVertex3f (-3.5, -3.5, 0.0);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0,-1,0);
        glColor3f (1, 1, 0.3);
        glVertex3f (-3.5, 3.5, 0.0);
        glVertex3f (3.5, 3.5, 0.0);
        glVertex3f (3.5, 3.5, 1.0);
        glVertex3f (-3.5, 3.5, 1.0);
    glEnd();
}

triangle randomTriangulo(void) {
    // valores do ponto inicial do triangulo
    float xInicial = rand() % 6 - 3;
    float yInicial = rand() % 6 - 3;

    vertice v[3] = {{xInicial,yInicial, 1.0},
                {xInicial,yInicial-0.5, 1.0},
                {xInicial-0.5,yInicial, 1.0}};
    triangle t = {v[0],v[1],v[2]};

    return t;
}

void drawPrisma(triangle t) {
    // Triangulo superior
    glBegin(GL_TRIANGLES);
        glNormal3f(0.0, 0.0, 1);
        glColor3f (1, 0.5, 0.5);
        glVertex3f (t.v[0].x, t.v[0].y, 1.0);
        glVertex3f (t.v[1].x, t.v[1].y, 1.0);
        glVertex3f (t.v[2].x, t.v[2].y, 1.0);
    glEnd();

    // Triangulo inferior
    glBegin(GL_TRIANGLES);
        glNormal3f(0.0, 0.0, -1);
        glColor3f (1, 0.5, 0.5);
        glVertex3f (t.v[0].x, t.v[0].y, 0.0);
        glVertex3f (t.v[1].x, t.v[1].y, 0.0);
        glVertex3f (t.v[2].x, t.v[2].y, 0.0);
    glEnd();

    // Paredes

    /* A cada parede é formado um triangulo a partir do triangulo superior para calculo da normal */

    vertice vetorNormal; //vetor normal para as paredes

    //P1
    // vertice e triangulo auxilar para as paredes
    vertice vAux1[3] = {{t.v[0].x, t.v[0].y, 1.0},
               {t.v[0].x, t.v[0].y, 0.0},
               {t.v[1].x, t.v[1].y, 0.0}};
    triangle tAux1 = {vAux1[0], vAux1[1], vAux1[2]};
    CalculaNormal(tAux1, &vetorNormal); // Passa face triangular e endereço do vetor normal de saída

    glBegin(GL_TRIANGLE_FAN);
        glColor3f (1, 0.5, 0.5);
        glNormal3f(vetorNormal.x, vetorNormal.y,vetorNormal.z);
        glVertex3f (t.v[0].x, t.v[0].y, 1.0);
        glVertex3f (t.v[0].x, t.v[0].y, 0.0);
        glVertex3f (t.v[1].x, t.v[1].y, 0.0);
        glVertex3f (t.v[1].x, t.v[1].y, 1.0);
    glEnd();

    //P2

    vertice vAux2[3] = {{t.v[2].x, t.v[2].y, 1.0},
               {t.v[2].x, t.v[2].y, 0.0},
               {t.v[0].x, t.v[0].y, 0.0}};
    triangle tAux2 = {vAux2[0], vAux2[1], vAux2[2]};
    CalculaNormal(tAux2, &vetorNormal); // Passa face triangular e endereço do vetor normal de saída

    glBegin(GL_TRIANGLE_FAN);
        glColor3f (1, 0.5, 0.5);
        glNormal3f(vetorNormal.x, vetorNormal.y,vetorNormal.z);
        glVertex3f (t.v[2].x, t.v[2].y, 1.0);
        glVertex3f (t.v[2].x, t.v[2].y, 0.0);
        glVertex3f (t.v[0].x, t.v[0].y, 0.0);
        glVertex3f (t.v[0].x, t.v[0].y, 1.0);
    glEnd();

    //P3

    vertice vAux3[3] = {{t.v[1].x, t.v[1].y, 1.0},
               {t.v[1].x, t.v[1].y, 0.0},
               {t.v[2].x, t.v[2].y, 0.0}};
    triangle tAux3 = {vAux3[0], vAux3[1], vAux3[2]};
    CalculaNormal(tAux3, &vetorNormal); // Passa face triangular e endereço do vetor normal de saída

    glBegin(GL_TRIANGLE_FAN);
        glColor3f (1, 0.5, 0.5);
        glNormal3f(vetorNormal.x, vetorNormal.y,vetorNormal.z);
        glVertex3f (t.v[1].x, t.v[1].y, 1.0);
        glVertex3f (t.v[1].x, t.v[1].y, 0.0);
        glVertex3f (t.v[2].x, t.v[2].y, 0.0);
        glVertex3f (t.v[2].x, t.v[2].y, 1.0);
    glEnd();
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
        proj_vision = 0.0;
    }
    else { // Visão perspectiva
        gluPerspective(60.0f,(GLfloat)width/(GLfloat)height,0.01f,200.0f);	// Calculate The Aspect Ratio Of The Window
        proj_vision = -6.0;
    }

    /* Camera e desenhos */

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt (0.0, proj_vision, zdist, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glPushMatrix();
        glRotatef( rotationY, 0.0, 1.0, 0.0 );
        glRotatef( rotationX, 1.0, 0.0, 0.0 );
        //drawObject();
        drawCampo();

        //Desenhando os prismas aleatórios
        for (int i = 0; i < QUANT_PRISMAS; i++)
            drawPrisma(prismas[i]);

    glPopMatrix();

    glutSwapBuffers();
}

void idle ()
{
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
    //gerando prismas aleatórios
    for (int i = 0; i < QUANT_PRISMAS; i++)
        prismas[i] = randomTriangulo();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc( mouse );
    glutMotionFunc( motion );
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
