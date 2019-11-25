#ifndef CURVAESQUERDA_H
#define CURVAESQUERDA_H
#include "Bloco.h"
#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <cstdio>
#include "Utils.h"
using namespace std;

const int nPontos=11;

class CurvaEsquerda
{
    public:
        CurvaEsquerda();
        virtual ~CurvaEsquerda();

        vertice getOrigemE();
        vertice getOrigemD();
        void atualizaPontosConstrucao();
        void iniciaPontosConstrucao();
        void drawCurvaEsquerda();
        void drawCurvaDireita();
        float getRaio();
        vertice getNormal(vertice v);
        void encontraDoisPontosMaisProximosE(vertice p, vertice* v1, vertice* v2);
        void encontraDoisPontosMaisProximosD(vertice p, vertice* v1, vertice* v2);


    protected:

    private:
        vertice pontosDeConstrucaoE[nPontos];
        vertice pontosDeConstrucaoD[nPontos];
        vertice origemE={-8.4, 2, 0.1};
        vertice origemD={8.4, 2, 0.1};
        float raio=5;
        Utils utility;
};

#endif // CURVAESQUERDA_H
