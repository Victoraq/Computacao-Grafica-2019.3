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

    private:
        int quant;
        vector<Bloco*> enemies;

};

#endif // ENEMY_H
