#include "Enemy.h"

Enemy::Enemy(int quant) {

    this->quant = quant;
    vertice origem = {-3.5,4.7,0.0};

    for (int i = 0; i < this->quant; i++) {
        origem.x = origem.x + 0.7;
        if (i != 0 && i % 9 == 0) {
            origem.x = -2.8;
            origem.y = origem.y - 0.35;
        }
        float a[3] = {0.5,1.0,0.5};

        this->enemies.push_back(new Bloco(origem,2.5,a));
    }

}

Enemy::~Enemy()
{
    //dtor
}

void Enemy::drawEnemies() {
    for (int i = 0; i < this->quant; i++) {
        this->enemies.at(i)->drawBloco();
    }
}
