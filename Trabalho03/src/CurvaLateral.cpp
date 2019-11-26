#include "CurvaLateral.h"

CurvaLateral::CurvaLateral()
{
    //ctor
    iniciaPontosConstrucao();
    atualizaPontosConstrucao();
}

CurvaLateral::~CurvaLateral()
{
    //dtor
}

vertice CurvaLateral::getOrigemE()
{
    return this->origemE;
}

vertice CurvaLateral::getOrigemD()
{
    return this->origemD;
}

void CurvaLateral::atualizaPontosConstrucao()
{
    for(int i=0; i<nPontos; i++)
    {
        //printf("[%f][%f][%f]\n", pontosDeConstrucao[i].x, pontosDeConstrucao[i].y,  pontosDeConstrucao[i].z);
        this->pontosDeConstrucaoE[i].x+=this->origemE.x;
        this->pontosDeConstrucaoE[i].y+=this->origemE.y;
        this->pontosDeConstrucaoE[i].z+=this->origemE.z;

        this->pontosDeConstrucaoD[i].x+=this->origemD.x;
        this->pontosDeConstrucaoD[i].y+=this->origemD.y;
        this->pontosDeConstrucaoD[i].z+=this->origemD.z;
    }
}

void CurvaLateral::iniciaPontosConstrucao()
{
    vertice *p=new vertice{raio, 0, 0};
    p=utility.rotaciona(p, utility.grausParaRadianos(-25));
    pontosDeConstrucaoE[0]=*p;
    pontosDeConstrucaoD[0]=*p;
    pontosDeConstrucaoD[0].x*=-1;
    printf("[%f][%f][%f]\n", pontosDeConstrucaoE[0].x, pontosDeConstrucaoE[0].y,  pontosDeConstrucaoE[0].z);
    printf("[%f][%f][%f]\n", pontosDeConstrucaoD[0].x, pontosDeConstrucaoD[0].y,  pontosDeConstrucaoD[0].z);
    for(int i=1; i<nPontos; i++)
    {
        pontosDeConstrucaoE[i]=*utility.rotaciona(p, utility.grausParaRadianos(5));
        pontosDeConstrucaoD[i]=pontosDeConstrucaoE[i];
        pontosDeConstrucaoD[i].x*=-1;
        //printf("[%f][%f][%f]\n", pontosDeConstrucao[i].x, pontosDeConstrucao[i].y,  pontosDeConstrucao[i].z);
    }

}

void CurvaLateral::drawCurvaEsquerda()
{
    for(int i=0; i<nPontos-1; i++)
    {
        glBegin(GL_QUADS);
            glVertex3f(pontosDeConstrucaoE[i].x, pontosDeConstrucaoE[i].y, -1*pontosDeConstrucaoE[i].z);
            glVertex3f(pontosDeConstrucaoE[i+1].x, pontosDeConstrucaoE[i+1].y, -1*pontosDeConstrucaoE[i+1].z);
            glVertex3f(pontosDeConstrucaoE[i+1].x, pontosDeConstrucaoE[i+1].y, pontosDeConstrucaoE[i+1].z);
            glVertex3f(pontosDeConstrucaoE[i].x, pontosDeConstrucaoE[i].y, pontosDeConstrucaoE[i].z);
        glEnd();

        glBegin(GL_POLYGON);
            for(int i=0; i<11; i++)
                glVertex3f(pontosDeConstrucaoE[i].x, pontosDeConstrucaoE[i].y, pontosDeConstrucaoE[i].z);
        glEnd();

        glBegin(GL_POLYGON);
            for(int i=0; i<11; i++)
                glVertex3f(pontosDeConstrucaoE[i].x, pontosDeConstrucaoE[i].y, -1*pontosDeConstrucaoE[i].z);
        glEnd();
    }
}

void CurvaLateral::drawCurvaDireita()
{
    for(int i=0; i<nPontos-1; i++)
    {
        glBegin(GL_QUADS);
            glVertex3f(pontosDeConstrucaoD[i+1].x, pontosDeConstrucaoD[i+1].y, -1*pontosDeConstrucaoD[i+1].z);
            glVertex3f(pontosDeConstrucaoD[i].x, pontosDeConstrucaoD[i].y, -1*pontosDeConstrucaoD[i].z);
            glVertex3f(pontosDeConstrucaoD[i].x, pontosDeConstrucaoD[i].y, pontosDeConstrucaoD[i].z);
            glVertex3f(pontosDeConstrucaoD[i+1].x, pontosDeConstrucaoD[i+1].y, pontosDeConstrucaoD[i+1].z);
        glEnd();

        glBegin(GL_POLYGON);
            for(int i=0; i<11; i++)
                glVertex3f(pontosDeConstrucaoD[10-i].x, pontosDeConstrucaoD[10-i].y, pontosDeConstrucaoD[10-i].z);
        glEnd();

        glBegin(GL_POLYGON);
            for(int i=0; i<11; i++)
                glVertex3f(pontosDeConstrucaoD[10-i].x, pontosDeConstrucaoD[10-i].y, -1*pontosDeConstrucaoD[10-i].z);
        glEnd();
    }
}

float CurvaLateral::getRaio()
{
    return this->raio;
}

vertice CurvaLateral::getNormal(vertice v)
{
    if(utility.calculaDistancia(v, origemE)<utility.calculaDistancia(v, origemD))
    {
        vertice p1, p2;
        for(int i=1; i<nPontos; i++)
        {
            p1=pontosDeConstrucaoE[i-1];
            p2=pontosDeConstrucaoE[i];
            //while(calculaDistancia(v, p2)<=calculaDistancia(v, p1))


        }
    }
}

void CurvaLateral::encontraDoisPontosMaisProximosE(vertice p, vertice* v1, vertice* v2)
{
    vertice pontoMedio, proxPontoMedio;
    float menorDistancia;
    int i;
    for(i=0; i<9; i++)
    {
        pontoMedio=utility.pontoMedio(this->pontosDeConstrucaoE[i], this->pontosDeConstrucaoE[i+1]);
        proxPontoMedio=utility.pontoMedio(this->pontosDeConstrucaoE[i+1], this->pontosDeConstrucaoE[i+2]);
        if(utility.calculaDistancia(p, pontoMedio)<utility.calculaDistancia(p, proxPontoMedio))
            break;
    }
    *v1=pontosDeConstrucaoE[i+1];
    *v2=pontosDeConstrucaoE[i];
}

void CurvaLateral::encontraDoisPontosMaisProximosD(vertice p, vertice* v1, vertice* v2)
{
    vertice pontoMedio, proxPontoMedio;
    float menorDistancia;
    int i;
    for(i=0; i<9; i++)
    {
        pontoMedio=utility.pontoMedio(this->pontosDeConstrucaoD[i], this->pontosDeConstrucaoD[i+1]);
        proxPontoMedio=utility.pontoMedio(this->pontosDeConstrucaoD[i+1], this->pontosDeConstrucaoD[i+2]);
        if(utility.calculaDistancia(p, pontoMedio)<utility.calculaDistancia(p, proxPontoMedio))
            break;
    }
    *v1=pontosDeConstrucaoD[i];
    *v2=pontosDeConstrucaoD[i+1];
}
