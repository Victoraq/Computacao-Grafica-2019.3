#ifndef CURVALATERAL_H
#define CURVALATERAL_H

#include "Bloco.h"
#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <cstdio>
#include "Utils.h"
#include "glcTexture.h"


using namespace std;

const int nPontos=11;

class CurvaLateral
{
    public:
        CurvaLateral();
        virtual ~CurvaLateral();

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
        bool checaColisaoCurvaEsquerda(vertice coords, float raio);
        bool checaColisaoCurvaDireita(vertice coords, float raio);
        bool colisaoCurvas(float direction_vector[], vertice position, float raio);
        void setMaterial(int material);


    protected:

    private:
        vertice pontosDeConstrucaoE[nPontos];
        vertice pontosDeConstrucaoD[nPontos];
        vertice origemE={-8.4, 2, 0.1};
        vertice origemD={8.4, 2, 0.1};
        float raio=5;
        Utils utility;
        glcTexture *textureManager;
};

#endif // CURVALATERAL_H
