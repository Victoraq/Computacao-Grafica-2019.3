#ifndef BLOCO_H
#define BLOCO_H

class vertice
{
    public:
        float x,y,z;
};

class Bloco
{
    public:
        Bloco(vertice origem, float dim, float cor[]);

        vertice Getorigem() { return origem; }
        void Setorigem(vertice val) { origem = val; }
        void drawBloco();

    private:
        vertice origem;
        float dim;
        float cor[3];
        bool show;
};

#endif // BLOCO_H
