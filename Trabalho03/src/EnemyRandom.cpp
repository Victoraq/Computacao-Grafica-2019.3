#include "EnemyRandom.h"

EnemyRandom::EnemyRandom(int quant) {

    srand (time(NULL));

    this->quant = quant;

    vertice origemEsquerda = {-XPOS,YPOS,0.0};
    vertice origemDireita = {XPOS,YPOS,0.0};

    float *direcaoEsquerda = new float[2];
    float *direcaoDireita = new float[2];

    direcaoEsquerda[0] = rand() % 2 + 0.2;
    direcaoEsquerda[1] = -1*(rand() % 2 + 0.2);
    direcaoDireita[0] = rand() % 2 + 0.2;
    direcaoDireita[1] = -1*(rand() % 2 + 0.2);


    for (int i = 0; i < quant; i++) {
        if (i % 2 == 0) {
            this->enemies.push_back(origemEsquerda);
            this->direcaoEnemies.push_back(direcaoEsquerda);
        } else {
            this->enemies.push_back(origemDireita);
            this->direcaoEnemies.push_back(direcaoDireita);
        }
        this->status.push_back(-1); // negativo o objeto não é renderizado
    }

    // LOAD OBJECT
    objectManager = new glcWavefrontObject();
    objectManager->SetNumberOfObjects(quant);
    for(int i = 0; i < this->quant; i++)
    {
        objectManager->SelectObject(i);
        objectManager->ReadObject(this->objFile);
        objectManager->SetShadingMode(SMOOTH_SHADING); // Possible values: FLAT_SHADING e SMOOTH_SHADING
        objectManager->SetRenderMode(USE_TEXTURE);     // Possible values: USE_COLOR, USE_MATERIAL, USE_TEXTURE (not available in this example)
        objectManager->Unitize();
        objectManager->FacetNormal();
        objectManager->VertexNormals(90.0);
        objectManager->Scale(50);
    }
}

void EnemyRandom::setMaterial()
{
    // Material utilizado: obsidian
    // Parametros comuns para os dois lados da superfície
    GLfloat objeto_especular[] = { 0.332741, 0.328634, 0.346435, 1.0 };
    GLfloat objeto_brilho[]    = { 0.3f };
    GLfloat objeto_ambient[]   = { 0.05375, 0.05, 0.06625, 1.0 };

    GLfloat objeto_difusa[]    = { 0.18275, 0.17, 0.22525, 1.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, objeto_difusa);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, objeto_brilho);
}

/*
    Desenha os inimigos na tela
    parametro newEnemy: permite ou não a criação de novos inimigos
*/
void EnemyRandom::drawEnemies(bool newEnemy) {
    for (int i = 0; i < this->quant; i++) {
        // se o objeto esta em processo de colisao nao ira desenhado normalmente
        if (this->status.at(i) > 0) {
            this->removeEnemy(i);
            continue;
        }
        // se o objeto esta escondido ira ser analisado se ele ira voltar a ser renderizado
        if (this->status.at(i) < 0) {

            // se estiver desabilitado a gerar novos inimigos continuamos o loop renderizando somente quem está visivel
            if (!newEnemy) continue;

            // voltando a desenhar o objeto se o random sortiar um multiplo de um primo especifico
            int desenha = rand() % 1000 + 2;

            if (desenha % 439 == 0) {
                this->status.at(i) = 0.0;

                // se o indice for par aparece na esquerda, se nao, na direita
                if (i % 2 == 0) this->enemies.at(i).x = -XPOS;
                else this->enemies.at(i).x = XPOS;

                this->enemies.at(i).y = YPOS;

            } else
                continue;
        }

        vertice posicao = this->enemies.at(i);
        glPushMatrix();
            setMaterial();
            glTranslatef(posicao.x, posicao.y, posicao.z);
            glTranslatef(0.0, 0.0, 0.0);
            glScalef(this->escala,this->escala,this->escala);
            glTranslatef(0.0, 0.0, 0.0);
            objectManager->SelectObject(0);
            objectManager->SetShadingMode(SMOOTH_SHADING); // Possible values: FLAT_SHADING e SMOOTH_SHADING
            objectManager->SetRenderMode(USE_TEXTURE);     // Possible values: USE_COLOR, USE_MATERIAL, USE_TEXTURE (not available in this example)
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

        if (this->status.at(i) != 0) continue; // se o objeto esta em processo de colisao nao ira colidir

        // Variaveis para calculo de distancia entre paredes

        float x_obj = this->enemies.at(i).x;
        float y_obj = this->enemies.at(i).y;
        float raio_obj = 0.35;
        bool status_inf = false; // verifica em que parede status
        bool status_sup = false;

        // considerando que o objeto esta em uma hitbox cubica simples

        float deltaX, deltaY, dist_inf1, dist_inf2;

        // Parede superior e inferior
        // verifica se está entre os "x" da hitbox
        if (centro.x + raio > x_obj - raio_obj && x_obj + raio_obj > centro.x - raio) {

            if (centro.y + raio > y_obj - raio_obj && centro.y - raio < y_obj + raio_obj ) {
                // Parede inferior
                if (centro.y+raio < y_obj) {
                    vetor_direcao[1] *= -1;
                    status_inf = true;
                }
                // Parede superior
                if (centro.y-raio > y_obj) {
                    vetor_direcao[1] *= -1;
                    status_sup = true;
                }
            }

        }

        // Paredes laterais
        if (centro.y + raio > y_obj - raio_obj && centro.y - raio < y_obj + raio_obj ) {
            if (centro.x + raio > x_obj - raio && x_obj + raio > centro.x - raio) {

                // Parede lateral direita
                if (centro.x+raio < x_obj) {
                    vetor_direcao[0] *= -1;

                    if (status_inf) {
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

                    } else if (status_sup) {
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

                    if (status_inf) {
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

                    } else if (status_sup) {
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

        if (status_inf || status_sup) {
            // se ocorreu a colisao o objeto comeca a animacao de colisao
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


void EnemyRandom::colisaoCurvaLateral(CurvaLateral *curva) {
    float raio_obj = 0.35;
    for (int i = 0; i < this->quant; i++) {
        curva->colisaoCurvas(this->direcaoEnemies.at(i), this->enemies.at(i), raio_obj);
    }
}


void EnemyRandom::resetEnemies()
{
    vertice origemEsquerda = {-XPOS,YPOS,0.0};
    vertice origemDireita = {XPOS,YPOS,0.0};

    float *direcaoEsquerda = new float[2];
    float *direcaoDireita = new float[2];

    direcaoEsquerda[0] = rand() % 2 + 0.2;
    direcaoEsquerda[1] = -1*(rand() % 2 + 0.2);
    direcaoDireita[0] = rand() % 2 + 0.2;
    direcaoDireita[1] = -1*(rand() % 2 + 0.2);

    for (int i = 0; i < quant; i++) {
        if (i % 2 == 0) {
            this->enemies.at(i) = origemEsquerda;
            this->direcaoEnemies.at(i) = direcaoEsquerda;
        } else {
            this->enemies.at(i) = origemDireita;
            this->direcaoEnemies.at(i) = direcaoDireita;
        }
        this->status.at(i) = -1;
    }
}

/// Irá realizar a animação da colisão e mudar pararâmetro para que eles não sejam mais visíveis
void EnemyRandom::removeEnemy(int index) {

    if (this->status.at(index) == 0.0)
        this->status.at(index) = this->escala;
    // a partir do momento em que o objeto esta com escala menor ou igual a zero
    // as colisoes nao serao computadas nem sera desenhado
    if (this->status.at(index) <= 0.0) {
        return;
    }

    vertice posicao = this->enemies.at(index);
    this->status.at(index)-=0.01;  // variavel de colisao diminui a escala do objeto passo a passo

    glPushMatrix();
        setMaterial();
        glTranslatef(posicao.x, posicao.y, posicao.z);
        glTranslatef(0.0, 0.0, 0.0);
        glScalef(this->status.at(index),this->status.at(index),this->status.at(index));
        glTranslatef(0.0, 0.0, 0.0);
        objectManager->SelectObject(0);
        objectManager->SetShadingMode(SMOOTH_SHADING);
        objectManager->SetRenderMode(USE_MATERIAL);
        objectManager->Unitize();
        objectManager->Draw();
    glPopMatrix();

//    if (this->status.at(index) <= 0.0) {
//        // removendo posicao o vetor de colisao
//        vector<float>::iterator posC = this->status.begin();
//        posC+=1.0;
//
//        //this->status.erase(posC);
//
//        vector<vertice>::iterator posE = this->enemies.end();
//        //posE+=index-1;
//
//        //this->enemies.erase(posE);
//        this->quant--;
//    }
}

