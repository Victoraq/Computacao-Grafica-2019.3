#ifndef PLAYER_H
#define PLAYER_H

#include "Bloco.h"
//#include <GL/glext.h>
#include <iostream>
#include "glcTexture.h"


class Player
{
    public:
        Player(vertice origem, float cor[]);

        vertice Getorigem() { return origem; }
        void Setorigem(vertice val) { origem = val; }

        void drawPlayer();
        void setColor(float r, float g, float b);
        void setMaterial(int material);
        bool colisao(vertice centro, float vetor_direcao[], float raio);
        vertice* calculaNormal(vertice* v1, vertice* v2, vertice* v3);

        vertice pontosDeConstrucao[21];
        vertice normaisDasFaces[20];
        void atualizaPontosDeConstrucao();
        void imprimePontosDeConstrucao();

        vertice* iniciaVetor(float x, float y, float z)
        {
            vertice* vet=new vertice;
            vet->x=x;
            vet->y=y;
            vet->z=z;

            return vet;
        }

    private:
        void rotacao(float coords[], float angulo);

        vertice origem;
        const int NPONTOS = 10; // numero de pontos para formar a curva
        float cor[3];
        float **pontos;         // armazena pontos da curva
        vertice *normais;        // armazena a normal de cada seguimento da curva
        glcTexture *textureManager;
};

#endif // PLAYER_H
