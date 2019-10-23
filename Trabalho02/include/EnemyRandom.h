#ifndef ENEMYRANDOM_H
#define ENEMYRANDOM_H

#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
#include <Bloco.h>
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
        void colisao(vertice centro, float vetor_direcao[], float raio);
        void colisaoParedes(float coord_sup, float coord_inf, float coord_lat);
        void resetEnemies();
        int numberOfEnemies() { return enemies.size(); }
        void decreaseEnemiesOnScreen();
        int getEnemiesOnScreen();


    private:
        int quant;
        vector<vertice> enemies;
        vector<float*> direcaoEnemies;
        int enemiesOnScreen;
        char objFile[200] = "/home/victor/Documentos/UFJF/CG/aula01ex01/data/obj/soccerball.obj";
        glcWavefrontObject *objectManager = NULL;
};

#endif // ENEMYRANDOM_H
