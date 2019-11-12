#ifndef PRISMA_H
#define PRISMA_H



class vertice
{
    public:
        float x,y,z;
};

class triangle
{
    public:
        vertice v[3];
};

class Prisma
{
    public:
        Prisma(triangle t);
        ~Prisma();

        triangle topo; // triangulo do topo do prisma
        vertice paredes[3][4]; // paredes do prisma
        vertice normais[3]; // normais de cada parede do prisma

        void draw();

    private:
        void calculaNormal(triangle t, vertice *vn);
};

#endif // PRISMA_H
