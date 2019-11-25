#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <cmath>
using namespace std;

class vertice
{
    public:
        float x,y,z;
};

class Utils
{
    public:
        Utils();
        virtual ~Utils();

        float calculaDistancia(vertice v1, vertice v2);
        vertice* iniciaVetor(float x, float y, float z);
        vertice* produtoVetorial(vertice* v1, vertice* v2);
        vertice* calculaNormal(vertice* v1, vertice* v2, vertice* v3);
        float radianosParaGraus(float valor);
        float grausParaRadianos(float valor);
        float produtoEscalar(vertice* v1, vertice* v2);
        float norma(vertice* vet);
        void unitiza(vertice* vet);
        float calculaAngulo(vertice* v1, vertice* v2);
        vertice* rotaciona(vertice* vet, float angulo);
        vertice pontoMedio(vertice v1, vertice v2);

    protected:

    private:
};

#endif // UTILS_H
