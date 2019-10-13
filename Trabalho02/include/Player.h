#ifndef PLAYER_H
#define PLAYER_H

#include "Bloco.h"


class Player
{
    public:
        Player(vertice origem, float cor[]);

        vertice Getorigem() { return origem; }
        void Setorigem(vertice val) { origem = val; }

        void drawPlayer();
        void setColor(float r, float g, float b);
        bool colisao(vertice centro, float vetor_direcao[], float raio);

    private:
        void rotacao(float coords[], float angulo);

        vertice origem;
        const float SIZE = 0.25;
        float cor[3];
};

#endif // PLAYER_H
