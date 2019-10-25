#include "Enemy.h"

Enemy::Enemy(int quant) {

    this->quant = quant;
    this->enemiesOnScreen=quant;
    this->conf = 0;

    vertice origem = {-3.5,4.7,0.0};

    for (int i = 0; i < this->quant; i++) {
        origem.x = origem.x + 0.7;
        if (i != 0 && i % 9 == 0) {
            origem.x = -2.8;
            origem.y = origem.y - 0.35;
        }

        float color[3] = {1.0, 0.0, 0.0};

        this->enemies.push_back(new Bloco(origem,2.5,color));
    }

}

Enemy::~Enemy()
{
    //dtor
}

void Enemy::drawEnemies() {
    for (int i = 0; i < this->quant; i++) {
        // comparacoes das configuracoes
        if (this->conf == 0 && i % 2 != 0) continue;
        if (this->conf == 1 && i % 2 == 0) continue;

        this->enemies.at(i)->drawBloco();
    }
}

void Enemy::colisao(vertice centro, float vetor_direcao[], float raio) {
    bool colidiu;
    for (int i = 0; i < this->quant; i++) {
        // comparacoes das configuracoes
        if (this->conf == 0 && i % 2 != 0) continue;
        if (this->conf == 1 && i % 2 == 0) continue;

        // Só é verificada a colisão se o inimigo for visualizavel
        if (this->enemies.at(i)->Getshow()) {

            colidiu = this->enemies.at(i)->colisao(centro, vetor_direcao, raio);

            this->enemies.at(i)->setShow(!colidiu); // Se colidiu, ele não é mais visualizavel
            if(!this->enemies.at(i)->Getshow())
            {
                this->decreaseEnemiesOnScreen();
            }
        }
    }
}

void Enemy::colisaoComEnemy(vertice centro, float vetor_direcao[], float raio) {
    bool colidiu;
    for (int i = 0; i < this->quant; i++) {
        // comparacoes das configuracoes
        if (this->conf == 0 && i % 2 != 0) continue;
        if (this->conf == 1 && i % 2 == 0) continue;

        // Só é verificada a colisão se o inimigo for visualizavel
        if (this->enemies.at(i)->Getshow()) {

            colidiu = this->enemies.at(i)->colisao(centro, vetor_direcao, raio);

        }
    }
}

void Enemy::resetEnemies()
{
    for(vector<Bloco*>::iterator p=this->enemies.begin(); p!=this->enemies.end(); p++)
        (*p)->setShow(true);
    this->enemiesOnScreen=quant;
}

void Enemy::decreaseEnemiesOnScreen()
{
    this->enemiesOnScreen--;
}

int Enemy::getEnemiesOnScreen()
{
    return this->enemiesOnScreen;
}

void Enemy::setConf(int conf) {
    this->conf = conf;

    float color[3];
    if (this->conf == 0) {
        color[0] = 1.0;
        color[1] = 0.0;
        color[2] = 0.0;
    }
    if (this->conf == 1) {
        color[0] = 0.0;
        color[1] = 0.0;
        color[2] = 0.8;
    }
    if (this->conf == 2) {
        color[0] = 1.0;
        color[1] = 0.0;
        color[2] = 1.0;
    }

    for (int i = 0; i < this->quant; i++) {
        this->enemies.at(i)->changeColor(color);
    }

}


int Enemy::numberOfEnemies() {
    int counter = 0;
    for (int i = 0; i < this->quant; i++) {
        if (this->conf == 0 && i % 2 != 0) continue;
        else if (this->conf == 1 && i % 2 == 0) continue;

        if (this->enemies.at(i)->Getshow()) counter++;
    }
    return counter;
}
