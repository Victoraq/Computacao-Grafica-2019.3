#include "EnemyRandom.h"

EnemyRandom::EnemyRandom(int quant) {

    srand (time(NULL));

    this->quant = quant;

    vertice origemEsquerda = {-2,5.3,0.0};
    vertice origemDireita = {2,5.3,0.0};

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
        this->colidiu.push_back(0);
        this->colidiu.push_back(0);
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
        objectManager->Scale(50);
    }
}

void EnemyRandom::drawEnemies() {
    for (int i = 0; i < this->quant; i++) {
        // se o objeto esta em processo de colisao nao ira desenhado normalmente
        if (this->colidiu.at(i) != 0) {
            this->removeEnemy(i);
            continue;
        }

        vertice posicao = this->enemies.at(i);
        glPushMatrix();
            glTranslatef(posicao.x, posicao.y, posicao.z);
            glTranslatef(0.0, 0.0, 0.0);
            glScalef(this->escala,this->escala,this->escala);
            glTranslatef(0.0, 0.0, 0.0);
            objectManager->SelectObject(0);
            objectManager->SetShadingMode(SMOOTH_SHADING); // Possible values: FLAT_SHADING e SMOOTH_SHADING
            objectManager->SetRenderMode(USE_MATERIAL);     // Possible values: USE_COLOR, USE_MATERIAL, USE_TEXTURE (not available in this example)
            objectManager->Unitize();
            objectManager->Draw();
        glPopMatrix();
    }
}

/// incrementa a posicao do objeto pelo passo passado por parametro
void EnemyRandom::movimenta(float step) {

    for (int i = 0; i < this->quant; i++) {
        this->enemies.at(i).x+=this->direcaoEnemies.at(i)[0]/(step);
        this->enemies.at(i).y+=this->direcaoEnemies.at(i)[1]/(step);
    }

}

void EnemyRandom::colisaoBola(vertice centro, float vetor_direcao[], float raio) {

    for (int i = 0; i < this->quant; i++) {

        if (this->colidiu.at(i) != 0) continue; // se o objeto esta em processo de colisao nao ira colidir

        // Variaveis para calculo de distancia entre paredes

        float x_obj = this->enemies.at(i).x;
        float y_obj = this->enemies.at(i).y;
        float raio_obj = 0.35;
        bool colidiu_inf = false; // verifica em que parede colidiu
        bool colidiu_sup = false;

        // considerando que o objeto esta em uma hitbox cubica simples

        float deltaX, deltaY, dist_inf1, dist_inf2;

        // Parede superior e inferior
        // verifica se está entre os "x" da hitbox
        if (centro.x + raio > x_obj - raio_obj && x_obj + raio_obj > centro.x - raio) {

            if (centro.y + raio > y_obj - raio_obj && centro.y - raio < y_obj + raio_obj ) {
                // Parede inferior
                if (centro.y+raio < y_obj) {
                    vetor_direcao[1] *= -1;
                    colidiu_inf = true;
                }
                // Parede superior
                if (centro.y-raio > y_obj) {
                    vetor_direcao[1] *= -1;
                    colidiu_sup = true;
                }
            }

        }

        // Paredes laterais
        if (centro.y + raio > y_obj - raio_obj && centro.y - raio < y_obj + raio_obj ) {
            if (centro.x + raio > x_obj - raio && x_obj + raio > centro.x - raio) {

                // Parede lateral direita
                if (centro.x+raio < x_obj) {
                    vetor_direcao[0] *= -1;

                    if (colidiu_inf) {
                        // distancia da parede inferior
                        deltaX = x_obj - centro.x;
                        deltaY = y_obj - raio_obj - centro.y;
                        dist_inf1 = sqrt(pow(deltaX,2) + pow(deltaY,2));

                        // distancia da parede lateral direita
                        deltaX = x_obj + raio - centro.x;
                        deltaY = y_obj - centro.y;
                        dist_inf2 = sqrt(pow(deltaX,2) + pow(deltaY,2));

                        if (dist_inf1 > dist_inf2) {
                            vetor_direcao[1] *= -1; // reverte mudança anterior
                        } else {
                            vetor_direcao[0] *= -1;
                        }

                    } else if (colidiu_sup) {
                        // distancia da parede superior
                        deltaX = x_obj - centro.x;
                        deltaY = y_obj + raio_obj - centro.y;
                        dist_inf1 = sqrt(pow(deltaX,2) + pow(deltaY,2));

                        // distancia da parede lateral direita
                        deltaX = x_obj + raio - centro.x;
                        deltaY = y_obj - centro.y;
                        dist_inf2 = sqrt(pow(deltaX,2) + pow(deltaY,2));

                        if (dist_inf1 > dist_inf2) {
                            vetor_direcao[1] *= -1; // reverte mudança anterior
                        } else {
                            vetor_direcao[0] *= -1;
                        }
                    }

                }
                // Parede lateral esquerda
                if (centro.x-raio > x_obj) {
                    vetor_direcao[0] *= -1;

                    if (colidiu_inf) {
                        // distancia da parede inferior
                        deltaX = x_obj - centro.x;
                        deltaY = y_obj - raio_obj - centro.y;
                        dist_inf1 = sqrt(pow(deltaX,2) + pow(deltaY,2));

                        // distancia da parede lateral esquerda
                        deltaX = x_obj - raio - centro.x;
                        deltaY = y_obj - centro.y;
                        dist_inf2 = sqrt(pow(deltaX,2) + pow(deltaY,2));

                        if (dist_inf1 > dist_inf2) {
                            vetor_direcao[1] *= -1; // reverte mudança anterior
                        } else {
                            vetor_direcao[0] *= -1;
                        }

                    } else if (colidiu_sup) {
                        // distancia da parede superior
                        deltaX = x_obj - centro.x;
                        deltaY = y_obj + raio_obj - centro.y;
                        dist_inf1 = sqrt(pow(deltaX,2) + pow(deltaY,2));

                        // distancia da parede lateral esquerda
                        deltaX = x_obj - raio - centro.x;
                        deltaY = y_obj - centro.y;
                        dist_inf2 = sqrt(pow(deltaX,2) + pow(deltaY,2));

                        if (dist_inf1 > dist_inf2) {
                            vetor_direcao[1] *= -1; // reverte mudança anterior
                        } else {
                            vetor_direcao[0] *= -1;
                        }
                    }

                }
            }
        }

        if (colidiu_inf || colidiu_sup) {
            this->colidiu.at(i) = this->escala; // se ocorreu a colisao o objeto comeca a animacao de colisao
            this->removeEnemy(i);
        }

    }

}

void EnemyRandom::colisaoParedes(float coord_sup, float coord_inf, float coord_lat) {

    for (int i = 0; i < this->quant; i++) {
        //Parede superior
        if (this->enemies.at(i).y+0.4 >= coord_sup)
            this->direcaoEnemies.at(i)[1] *= -1;

        // Parede inferior
        if (this->enemies.at(i).y-0.2 <= coord_inf) {
            this->direcaoEnemies.at(i)[1] *= -1;
        }

        // Parede lateral direita
        if (this->enemies.at(i).x+0.4 >= coord_lat)
            this->direcaoEnemies.at(i)[0] *= -1;

        // Parede lateral esquerda
        if (this->enemies.at(i).x-0.4 <= -coord_lat)
            this->direcaoEnemies.at(i)[0] *= -1;
    }

}

void EnemyRandom::colisaoBloco(Enemy* blocos) {
    float raio_obj = 0.35;
    for (int i = 0; i < this->quant; i++) {
        blocos->colisaoComEnemy(this->enemies.at(i), this->direcaoEnemies.at(i), raio_obj);
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
        this->colidiu.at(i) = 0;
        this->colidiu.at(i+1) = 0;
    }
}

/// Irá realizar a animação da colisão e mudar pararâmetro para que eles não sejam mais visíveis
void EnemyRandom::removeEnemy(int index) {

    // a partir do momento em que o objeto esta com escala menor ou igual a zero
    // as colisoes nao serao computadas nem sera desenhado
    if (this->colidiu.at(index) <= 0.0) {
        return;
    }

    vertice posicao = this->enemies.at(index);
    this->colidiu.at(index)-=0.01;  // variavel de colisao diminui a escala do objeto passo a passo

    glPushMatrix();
        glTranslatef(posicao.x, posicao.y, posicao.z);
        glTranslatef(0.0, 0.0, 0.0);
        glScalef(this->colidiu.at(index),this->colidiu.at(index),this->colidiu.at(index));
        glTranslatef(0.0, 0.0, 0.0);
        objectManager->SelectObject(0);
        objectManager->SetShadingMode(SMOOTH_SHADING);
        objectManager->SetRenderMode(USE_MATERIAL);
        objectManager->Unitize();
        objectManager->Draw();
    glPopMatrix();

//    if (this->colidiu.at(index) <= 0.0) {
//        // removendo posicao o vetor de colisao
//        vector<float>::iterator posC = this->colidiu.begin();
//        posC+=1.0;
//
//        //this->colidiu.erase(posC);
//
//        vector<vertice>::iterator posE = this->enemies.end();
//        //posE+=index-1;
//
//        //this->enemies.erase(posE);
//        this->quant--;
//    }
}

