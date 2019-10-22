
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
        bool Getshow() { return show; }

        void Setorigem(vertice val) { origem = val; }
        void setShow(bool b);

        void drawBloco();
        void setColor(float r, float g, float b);
        bool colisao(vertice centro, float vetor_direcao[], float raio);
        void changeColor(float cor[3]);


    private:
        const float SIZE = 0.25;
        vertice origem;   // Ponto de origem do bloco
        float dim;        // Dimensões do bloco
        float cor[3];     // Vetor de cores rgb
        bool show;        // Indica se o bloco é visualizavel ou não
};

#endif // BLOCO_H
