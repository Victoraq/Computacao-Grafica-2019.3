#ifndef ENEMY_H
#define ENEMY_H

#include "Bloco.h"
#include <vector>
#include <iostream>
using namespace std;

class Enemy
{
    public:
        Enemy(int quant);
        virtual ~Enemy();

        void drawEnemies();
        void colisao(vertice centro, float vetor_direcao[], float raio);
        void colisaoComEnemy(vertice centro, float vetor_direcao[], float raio);
        void resetEnemies();
        int numberOfEnemies();
        void decreaseEnemiesOnScreen();
        int getEnemiesOnScreen();
        void setConf(int conf);

    private:
        int quant;
        vector<Bloco*> enemies;
        int enemiesOnScreen;
        int conf;         // Torna configuravel a forma de apresentacao dos blocos

};

#endif // ENEMY_H
