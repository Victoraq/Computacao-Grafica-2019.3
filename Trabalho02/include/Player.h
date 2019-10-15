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

        vertice pontosDeConstrucao[21];
        void imprimePontosDeConstrucao();

    private:
        void rotacao(float coords[], float angulo);

        vertice origem;
        const int NPONTOS = 20; // numero de pontos para formar a curva
        float cor[3];
        float **pontos;         // armazena pontos da curva
        vertice *normais;        // armazena a normal de cada seguimento da curva
};

#endif // PLAYER_H
