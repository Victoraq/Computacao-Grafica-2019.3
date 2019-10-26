#ifndef ENEMYRANDOM_H
#define ENEMYRANDOM_H

#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
#include <Bloco.h>
#include <Enemy.h>
#include <glcWavefrontObject.h>

using namespace std;

typedef struct
{
    //GLMmodel* pmodel = NULL;
    glcWavefrontObject *pmodel = NULL;
} object;


class EnemyRandom
{
    public:
        EnemyRandom(int quant);

        void drawEnemies();
        void movimenta(float step);
        void colisaoBola(vertice centro, float vetor_direcao[], float raio);
        void colisaoParedes(float coord_sup, float coord_inf, float coord_lat);
        void colisaoBloco(Enemy* blocos);
        void resetEnemies();
        int numberOfEnemies() { return enemies.size(); }
        void removeEnemy(int index);


    private:
        int quant;
        vector<vertice> enemies;
        vector<float*> direcaoEnemies;
        float escala = 0.7;                      // escala do objeto
        // indica o status do objeto.
        // status == 0 -> rederizavel
        // status < 0 -> nao renderizavel
        // status > 0 -> colisao
        vector<float> status;
        int regressao = 10;
        char objFile[200] = "/home/victor/Documentos/UFJF/CG/aula01ex01/data/obj/soccerball.obj";
        glcWavefrontObject *objectManager = NULL;
        float XPOS = 2;
        float YPOS = 5.65;

};

#endif // ENEMYRANDOM_H
