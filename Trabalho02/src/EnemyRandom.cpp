#include "EnemyRandom.h"

EnemyRandom::EnemyRandom(int quant) {

    //srand (time(NULL));

    this->quant = quant;
    this->enemiesOnScreen=quant;

    vertice origemEsquerda = {-2,5.65,0.0};
    vertice origemDireita = {2,5.65,0.0};

    float *direcaoEsquerda = new float[2];
    float *direcaoDireita = new float[2];

    direcaoEsquerda[0] = rand() % 2 + 0.2;
    direcaoEsquerda[1] = -1*(rand() % 2 + 0.2);
    direcaoDireita[0] = rand() % 2 + 0.2;
    direcaoDireita[1] = -1*(rand() % 2 + 0.2);


    for (int i = 0; i < quant; i+=2) {
        this->enemies.push_back(origemEsquerda);
        this->enemies.push_back(origemDireita);
        this->direcaoEnemies.push_back(direcaoEsquerda);
        this->direcaoEnemies.push_back(direcaoDireita);
    }

    // LOAD OBJECT
    objectManager = new glcWavefrontObject();
    objectManager->SetNumberOfObjects(quant);
    for(int i = 0; i < this->quant; i++)
    {
        objectManager->SelectObject(i);
        objectManager->ReadObject(this->objFile);
        objectManager->Unitize();
        objectManager->FacetNormal();
        objectManager->VertexNormals(90.0);
        objectManager->Scale(5);
    }
}

void EnemyRandom::drawEnemies() {
for (int i = 0; i < quant; i++) {
        printf("\n Direcao: %f %f %f \n",this->direcaoEnemies.at(i)[0], this->direcaoEnemies.at(i)[1]);
    }
    for (int i = 0; i < this->quant; i++) {
        vertice posicao = this->enemies.at(i);
        glPushMatrix();
            glColor3f(0.1,0.3,0.5);
            printf("%f %f %f \n",posicao.x, posicao.y);
            glTranslatef(posicao.x, posicao.y, posicao.z);
            objectManager->SelectObject(0);
            objectManager->SetShadingMode(SMOOTH_SHADING); // Possible values: FLAT_SHADING e SMOOTH_SHADING
            objectManager->SetRenderMode(USE_MATERIAL);     // Possible values: USE_COLOR, USE_MATERIAL, USE_TEXTURE (not available in this example)
            objectManager->Unitize();
            objectManager->Draw();
        glPopMatrix();
    }
}

void EnemyRandom::movimenta(float step) {

    for (int i = 0; i < this->quant; i++) {
        printf("\n Antes Direcao: %f %f %f \n",this->direcaoEnemies.at(i)[0], this->direcaoEnemies.at(i)[1]);
        this->enemies.at(i).x+=this->direcaoEnemies.at(i)[0]/(step);
        this->enemies.at(i).y+=this->direcaoEnemies.at(i)[1]/(step);
        printf("\n Direcao: %f %f %f \n",this->direcaoEnemies.at(i)[0], this->direcaoEnemies.at(i)[1]);
    }

}

void EnemyRandom::colisao(vertice centro, float vetor_direcao[], float raio) {

//    for (int i = 0; i < this->quant; i++) {
//    }
}

void EnemyRandom::colisaoParedes(float coord_sup, float coord_inf, float coord_lat) {

    for (int i = 0; i < this->quant; i++) {
        //Parede superior
        if (this->enemies.at(i).y+0.26 >= coord_sup)
            this->direcaoEnemies.at(i)[1] *= -1;

        // Parede inferior
        if (this->enemies.at(i).y-0.26 <= coord_inf) {
            this->direcaoEnemies.at(i)[1] *= -1;
        }

        // Parede lateral direita
        if (this->enemies.at(i).x+0.26 >= coord_lat)
            this->direcaoEnemies.at(i)[0] *= -1;

        // Parede lateral esquerda
        if (this->enemies.at(i).x-0.26 <= -coord_lat)
            this->direcaoEnemies.at(i)[0] *= -1;
    }

}

void EnemyRandom::resetEnemies()
{
    vertice origemEsquerda = {-2,5.65,0.0};
    vertice origemDireita = {2,5.65,0.0};

    float *direcaoEsquerda = new float[2];
    float *direcaoDireita = new float[2];

    direcaoEsquerda[0] = rand() % 2 + 0.2;
    direcaoEsquerda[1] = -1*(rand() % 2 + 0.2);
    direcaoDireita[0] = rand() % 2 + 0.2;
    direcaoDireita[1] = -1*(rand() % 2 + 0.2);

    for (int i = 0; i < quant; i+=2) {
        this->enemies.at(i) = origemEsquerda;
        this->enemies.at(i+1) = origemDireita;
        this->direcaoEnemies.at(i) = direcaoEsquerda;
        this->direcaoEnemies.at(i+1) = direcaoDireita;
    }
}

void EnemyRandom::decreaseEnemiesOnScreen()
{
    this->enemiesOnScreen--;
}

int EnemyRandom::getEnemiesOnScreen()
{
    return this->enemiesOnScreen;
}

