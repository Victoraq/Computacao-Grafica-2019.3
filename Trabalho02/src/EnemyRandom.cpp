#include "EnemyRandom.h"

EnemyRandom::EnemyRandom(int quant) {

    //srand (time(NULL));

    this->quant = quant;
    this->enemiesOnScreen=quant;

    vertice origemEsquerda = {-2,5.65,0.0};
    vertice origemDireita = {2,5.65,0.0};

    float direcaoEsquerda[] = {rand() % 10 + 1, -1*(rand() % 10 + 1)};
    float direcaoDireita[] = {rand() % 10 + 1, -1*(rand() % 10 + 1)};

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

    objectManager->SelectObject(0);
    objectManager->SetShadingMode(SMOOTH_SHADING); // Possible values: FLAT_SHADING e SMOOTH_SHADING
    objectManager->SetRenderMode(USE_MATERIAL);     // Possible values: USE_COLOR, USE_MATERIAL, USE_TEXTURE (not available in this example)
    objectManager->Unitize();
    objectManager->Draw();
}

void EnemyRandom::movimenta(float step) {

    for (int i = 0; i < this->quant; i++) {
        this->enemies.at(i).x+=this->direcaoEnemies.at(i)[0]/(step);
        this->enemies.at(i).y+=this->direcaoEnemies.at(i)[1]/(step);
        printf("\n Direcao: %f %f %f \n",this->direcaoEnemies.at(i)[0], this->direcaoEnemies.at(i)[1]);
    }

}

void EnemyRandom::colisao(vertice centro, float vetor_direcao[], float raio) {
//    bool colidiu;
//    for (int i = 0; i < this->quant; i++) {
//        // Só é verificada a colisão se o inimigo for visualizavel
//        if (this->enemies.at(i)->Getshow()) {
//
//            colidiu = this->enemies.at(i)->colisao(centro, vetor_direcao, raio);
//
//            this->enemies.at(i)->setShow(!colidiu); // Se colidiu, ele não é mais visualizavel
//            if(!this->enemies.at(i)->Getshow())
//            {
//                this->decreaseEnemiesOnScreen();
//            }
//        }
//    }
}

void EnemyRandom::resetEnemies()
{
//    for(vector<Bloco*>::iterator p=this->enemies.begin(); p!=this->enemies.end(); p++)
//        (*p)->setShow(true);
//    this->enemiesOnScreen=quant;
}

void EnemyRandom::decreaseEnemiesOnScreen()
{
    this->enemiesOnScreen--;
}

int EnemyRandom::getEnemiesOnScreen()
{
    return this->enemiesOnScreen;
}

