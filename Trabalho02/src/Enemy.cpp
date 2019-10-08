#include "Enemy.h"

Enemy::Enemy(int quant) {


    this->quant = quant;
    this->enemiesOnScreen=quant;
    cout << endl << "EOS " << this->enemiesOnScreen;
    vertice origem = {-3.5,4.7,0.0};

    for (int i = 0; i < this->quant; i++) {
        origem.x = origem.x + 0.7;
        if (i != 0 && i % 9 == 0) {
            origem.x = -2.8;
            origem.y = origem.y - 0.35;
        }
        float color[3] = {1.0,0.0,0.0};

        this->enemies.push_back(new Bloco(origem,2.5,color));
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

void Enemy::colisao(vertice centro, float vetor_direcao[], float raio) {
    bool colidiu;
    for (int i = 0; i < this->quant; i++) {
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
