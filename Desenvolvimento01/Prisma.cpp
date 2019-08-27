#include "Prisma.h"
#include <math.h>
#include <GL/glut.h>
#include <stdlib.h>

Prisma::Prisma(triangle t)
{
    this->topo = t;

    // Parede 1
    paredes[0][0] = {t.v[0].x, t.v[0].y, 1.0};
    paredes[0][1] = {t.v[0].x, t.v[0].y, 0.0};
    paredes[0][2] = {t.v[1].x, t.v[1].y, 0.0};
    paredes[0][3] = {t.v[1].x, t.v[1].y, 1.0};
    triangle tAux1 = {paredes[0][0], paredes[0][1], paredes[0][2]}; // Calculando normal
    calculaNormal(tAux1, &normais[0]);

    // Parede 2
    paredes[1][0] = {t.v[2].x, t.v[2].y, 1.0};
    paredes[1][1] = {t.v[2].x, t.v[2].y, 0.0};
    paredes[1][2] = {t.v[0].x, t.v[0].y, 0.0};
    paredes[1][3] = {t.v[0].x, t.v[0].y, 1.0};
    triangle tAux2 = {paredes[1][0], paredes[1][1], paredes[1][2]}; // Calculando normal
    calculaNormal(tAux2, &normais[1]);

    // Parede 3
    paredes[2][0] = {t.v[1].x, t.v[1].y, 1.0};
    paredes[2][1] = {t.v[1].x, t.v[1].y, 0.0};
    paredes[2][2] = {t.v[2].x, t.v[2].y, 0.0};
    paredes[2][3] = {t.v[2].x, t.v[2].y, 1.0};
    triangle tAux3 = {paredes[2][0], paredes[2][1], paredes[2][2]}; // Calculando normal
    calculaNormal(tAux3, &normais[2]);
}

Prisma::~Prisma() {}

/// Desenha o modelo 3D do prisma
void Prisma::draw() {
    // Triangulo superior
    glBegin(GL_TRIANGLES);
        glNormal3f(0.0, 0.0, 1);
        glVertex3f (topo.v[0].x, topo.v[0].y, 1.0);
        glVertex3f (topo.v[1].x, topo.v[1].y, 1.0);
        glVertex3f (topo.v[2].x, topo.v[2].y, 1.0);
    glEnd();

    // Triangulo inferior
    glBegin(GL_TRIANGLES);
        glNormal3f(0.0, 0.0, -1);
        glVertex3f (topo.v[0].x, topo.v[0].y, 0.0);
        glVertex3f (topo.v[1].x, topo.v[1].y, 0.0);
        glVertex3f (topo.v[2].x, topo.v[2].y, 0.0);
    glEnd();

    // Paredes

    /* A cada parede é formado um triangulo a partir do triangulo superior para calculo da normal */

    //P1
    glBegin(GL_TRIANGLE_FAN);
        glNormal3f(normais[0].x, normais[0].y,normais[0].z);
        glVertex3f (paredes[0][0].x, paredes[0][0].y, 1.0);
        glVertex3f (paredes[0][1].x, paredes[0][1].y, 0.0);
        glVertex3f (paredes[0][2].x, paredes[0][2].y, 0.0);
        glVertex3f (paredes[0][3].x, paredes[0][3].y, 1.0);
    glEnd();

    //P2

    glBegin(GL_TRIANGLE_FAN);
        glNormal3f(normais[1].x, normais[1].y,normais[1].z);
        glVertex3f (paredes[1][0].x, paredes[1][0].y, 1.0);
        glVertex3f (paredes[1][1].x, paredes[1][1].y, 0.0);
        glVertex3f (paredes[1][2].x, paredes[1][2].y, 0.0);
        glVertex3f (paredes[1][3].x, paredes[1][3].y, 1.0);
    glEnd();

    //P3

    glBegin(GL_TRIANGLE_FAN);
        glNormal3f(normais[2].x, normais[2].y,normais[2].z);
        glVertex3f (paredes[2][0].x, paredes[2][0].y, 1.0);
        glVertex3f (paredes[2][1].x, paredes[2][1].y, 0.0);
        glVertex3f (paredes[2][2].x, paredes[2][2].y, 0.0);
        glVertex3f (paredes[2][3].x, paredes[2][3].y, 1.0);
    glEnd();
}

/// Calcula a normal de um triangulo passado por parâmetro
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
void Prisma::calculaNormal(triangle t, vertice *vn) {
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
