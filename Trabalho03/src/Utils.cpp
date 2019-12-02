#include "Utils.h"

Utils::Utils()
{
    //ctor
}

Utils::~Utils()
{
    //dtor
}

float Utils::calculaDistancia(vertice v1, vertice v2)
{
    return sqrt(pow(v1.x-v2.x, 2) + pow(v1.y-v2.y, 2) + pow(v1.z-v2.z, 2));
}

vertice* Utils::iniciaVetor(float x, float y, float z)
{
    vertice* vet=new vertice;
    vet->x=x;
    vet->y=y;
    vet->z=z;

    return vet;
}

vertice* Utils::produtoVetorial(vertice* v1, vertice* v2)
{
    vertice* vet=new vertice;
    vet->x=((v1->y*v2->z)-(v2->y*v1->z));
    vet->y=((v1->x*v2->z)-(v2->x*v1->z));
    if(vet->y!=0)
        vet->y*=-1;
    vet->z=((v1->x*v2->y)-(v2->x*v1->y));
    return vet;
}

vertice* Utils::calculaNormal(vertice* v1, vertice* v2, vertice* v3)
{

    vertice* vetA=new vertice;
    vertice* vetB=new vertice;

    vetA->x = v1->x-v2->x;
    vetA->y = v1->y-v2->y;
    vetA->z = v1->z-v2->z;

    vetB->x = v3->x-v2->x;
    vetB->y = v3->y-v2->y;
    vetB->z = v3->z-v2->z;

    vertice*c=produtoVetorial(vetA, vetB);

    return c;
}

float Utils::radianosParaGraus(float valor)
{
    return (valor*180)/3.14;
}

float Utils::grausParaRadianos(float valor)
{
    return (valor*3.14159265)/180;
}

float Utils::produtoEscalar(vertice* v1, vertice* v2)
{
    return ((v1->x*v2->x)+(v1->y*v2->y)+(v1->z*v2->z));
}

float Utils::norma(vertice* vet)
{
    return sqrt(pow(vet->x, 2) + pow(vet->y, 2) + pow(vet->z, 2));
}

void Utils::unitiza(vertice* vet)
{
    int vel=1;
    float n=norma(vet);
    vet->x/=n;
    vet->y/=n;
    vet->z/=n;

    vet->x*=vel;
    vet->y*=vel;
    vet->z*=vel;

}

float Utils::calculaAngulo(vertice* v1, vertice* v2)
{
    float v =(produtoEscalar(v1, v2))/(norma(v1)*norma(v2));
    //v=3.14-v;
    v=grausParaRadianos(v);
    float a = radianosParaGraus(acos(v));
    if(a<90)
        return a;
    else
        return (180-a);
}

float Utils::calculaAnguloRotacao(vertice* vet, vertice* normal, char colisor)
{
    float aVet, aNormal, anguloVN, res;
    vertice vetOrigem;
    if(colisor=='e')
        vetOrigem={0, -1, 0};
    else if(colisor=='d')
        vetOrigem={0, 1, 0};
    else
        vetOrigem={1, 0, 0};
    inverte(vet);

    aVet=grausParaRadianos(calculaAngulo(vet, &vetOrigem));
    aNormal=grausParaRadianos(calculaAngulo(normal, &vetOrigem));
    anguloVN=calculaAngulo(vet, normal);

    if(aVet<aNormal)
        res= 2*calculaAngulo(vet, normal);
    else if(aVet>aNormal)
        res= (2*(calculaAngulo(vet, normal)))*-1;
    else
        res= 0;

    printf("vet[%f][%f][%f]\n", vet->x, vet->y, vet->z);
    printf("normal[%f][%f][%f]\n", normal->x, normal->y, normal->z);
    printf("res %f", res);

    return res;

}

vertice* Utils::rotaciona(vertice* vet, float angulo)
{
    float a, b;

    a = (vet->x*cos(angulo) - vet->y*sin(angulo));
    b = (vet->x*sin(angulo) + vet->y*cos(angulo));

    vet->x=a;
    vet->y=b;

    return vet;
}

vertice Utils::pontoMedio(vertice v1, vertice v2)
{
    vertice p={(v1.x+v2.x)/2, (v1.y+v2.y)/2, (v1.z+v2.z)/2};
    return p;
}

void Utils::inverte(vertice* p)
{
    p->x*=-1;
    p->y*=-1;
    p->z*=-1;
}
