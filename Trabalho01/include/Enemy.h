#ifndef ENEMY_H
#define ENEMY_H

#include "Bloco.h"
#include <vector>

using namespace std;

class Enemy
{
    public:
        Enemy(int quant);
        virtual ~Enemy();

        void drawEnemies();
        void colisao(vertice centro, float vetor_direcao[], float raio);
        void resetEnemies();
        int numberOfEnemies() { return enemies.size(); }

    private:
        int quant;
        vector<Bloco*> enemies;

};

#endif // ENEMY_H
