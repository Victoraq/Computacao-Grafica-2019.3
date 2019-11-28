#include "CurvaLateral.h"

CurvaLateral::CurvaLateral()
{
    //ctor
    iniciaPontosConstrucao();
    atualizaPontosConstrucao();

    textureManager = new glcTexture();            // Criação do arquivo que irá gerenciar as texturas
    textureManager->SetNumberOfTextures(2);       // Estabelece o número de texturas que será utilizado
    textureManager->CreateTexture("textures/roxo.png", 0);
    textureManager->CreateTexture("textures/cinza.png", 1);
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

void CurvaLateral::setMaterial(int material) {
    // Material utilizado: esmeralda
    // Parametros comuns para os dois lados da superfície
    if (material == 1) {
        GLfloat objeto_especular[] = { 0.633, 0.727811,	0.633, 1.0 };
        GLfloat objeto_brilho[]    = { 60.0f };
        GLfloat objeto_ambient[]   = { 0.0215,	0.1745,	0.0215, 1.0 };

        GLfloat objeto_difusa[]    = { 0.07568,	0.61424, 0.07568, 1.0 };

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, objeto_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, objeto_difusa);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, objeto_especular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, objeto_brilho);
    } else {
        GLfloat objeto_ambient[]   = { 0.2, 0.2, 0.2, 1.0 };
        GLfloat objeto_difusa[]    = { 0.8, 0.8, 0.8, 1.0 };
        GLfloat objeto_especular[] = { 0.0, 0.0, 0.0, 1.0 };

        glMaterialfv(GL_FRONT, GL_AMBIENT, objeto_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, objeto_difusa);
        glMaterialfv(GL_FRONT, GL_SPECULAR, objeto_especular);
    }
}


void CurvaLateral::drawCurvaEsquerda()
{
    glPushMatrix();

        setMaterial(0);
        // borda lateral
        float passoText = 1.0/(nPontos-1);
        float texture_pos = 0;
        textureManager->Bind(0);
        glPushMatrix();
        for(int i=0; i<nPontos-1; i++)
        {
            glBegin(GL_QUADS);
                    vertice v1 = {pontosDeConstrucaoE[i].x, pontosDeConstrucaoE[i].y, -1*pontosDeConstrucaoE[i].z};
                    vertice v2 = {pontosDeConstrucaoE[i+1].x, pontosDeConstrucaoE[i+1].y, -1*pontosDeConstrucaoE[i+1].z};
                    vertice v3 = {pontosDeConstrucaoE[i].x, pontosDeConstrucaoE[i].y, pontosDeConstrucaoE[i].z};
                    vertice* normal = utility.calculaNormal(&v1,&v2,&v3);
                    glNormal3f(1, 0.0, 0.0);


                    glTexCoord2f(texture_pos,texture_pos);
                    glVertex3f(pontosDeConstrucaoE[i].x, pontosDeConstrucaoE[i].y, -1*pontosDeConstrucaoE[i].z);
                    glTexCoord2f(texture_pos+passoText,texture_pos);
                    glVertex3f(pontosDeConstrucaoE[i+1].x, pontosDeConstrucaoE[i+1].y, -1*pontosDeConstrucaoE[i+1].z);
                    glTexCoord2f(texture_pos+passoText,texture_pos+passoText);
                    glVertex3f(pontosDeConstrucaoE[i+1].x, pontosDeConstrucaoE[i+1].y, pontosDeConstrucaoE[i+1].z);
                    glTexCoord2f(texture_pos,texture_pos+passoText);
                    glVertex3f(pontosDeConstrucaoE[i].x, pontosDeConstrucaoE[i].y, pontosDeConstrucaoE[i].z);

                glEnd();
                texture_pos += passoText;
        }

        textureManager->Bind(1);
        glBegin(GL_POLYGON);
            glNormal3f(0.0,0.0,1.0);
            for(int i=0; i<11; i++) {
                glTexCoord2f((pontosDeConstrucaoE[i].x)/3.868461, (pontosDeConstrucaoE[i].y+1)/5.113091);
                glVertex3f(pontosDeConstrucaoE[i].x, pontosDeConstrucaoE[i].y, pontosDeConstrucaoE[i].z);
            }
        glEnd();

        glBegin(GL_POLYGON);
            glNormal3f(0.0,0.0,-1.0);
            for(int i=10; i>=0; i--) {
                glTexCoord2f((pontosDeConstrucaoE[i].x)/3.868461, (pontosDeConstrucaoE[i].y+1)/5.113091);
                glVertex3f(pontosDeConstrucaoE[i].x, pontosDeConstrucaoE[i].y, -1*pontosDeConstrucaoE[i].z);
            }
        glEnd();
    glPopMatrix();
    textureManager->Disable();
}

void CurvaLateral::drawCurvaDireita()
{
    glPushMatrix();
        setMaterial(0);
        // borda lateral
        float passoText = 1.0/(nPontos-1);
        float texture_pos = 0;
        textureManager->Bind(0);
        glPushMatrix();
        for(int i=0; i<nPontos-1; i++)
        {
            glBegin(GL_QUADS);
                vertice v1 = {pontosDeConstrucaoD[i].x, pontosDeConstrucaoD[i].y, -1*pontosDeConstrucaoD[i].z};
                vertice v2 = {pontosDeConstrucaoD[i+1].x, pontosDeConstrucaoD[i+1].y, -1*pontosDeConstrucaoD[i+1].z};
                vertice v3 = {pontosDeConstrucaoD[i].x, pontosDeConstrucaoD[i].y, pontosDeConstrucaoD[i].z};
                vertice* normal = utility.calculaNormal(&v1,&v2,&v3);
                glNormal3f(-1, 0.0, 0.0);

                glTexCoord2f(texture_pos+passoText,texture_pos);
                glVertex3f(pontosDeConstrucaoD[i+1].x, pontosDeConstrucaoD[i+1].y, -1*pontosDeConstrucaoD[i+1].z);
                glTexCoord2f(texture_pos,texture_pos);
                glVertex3f(pontosDeConstrucaoD[i].x, pontosDeConstrucaoD[i].y, -1*pontosDeConstrucaoD[i].z);
                glTexCoord2f(texture_pos,texture_pos+passoText);
                glVertex3f(pontosDeConstrucaoD[i].x, pontosDeConstrucaoD[i].y, pontosDeConstrucaoD[i].z);
                glTexCoord2f(texture_pos+passoText,texture_pos+passoText);
                glVertex3f(pontosDeConstrucaoD[i+1].x, pontosDeConstrucaoD[i+1].y, pontosDeConstrucaoD[i+1].z);
            glEnd();
            texture_pos += passoText;
        }
        glPopMatrix();


        textureManager->Bind(1);
        glBegin(GL_POLYGON);
            glNormal3f(0.0,0.0,1.0);
            for(int i=0; i<11; i++) {
                glTexCoord2f((pontosDeConstrucaoD[10-i].x)/3.868461, (pontosDeConstrucaoD[10-i].y+1)/5.113091);
                glVertex3f(pontosDeConstrucaoD[10-i].x, pontosDeConstrucaoD[10-i].y, pontosDeConstrucaoD[10-i].z);
            }
        glEnd();

        glBegin(GL_POLYGON);
            glNormal3f(0.0,0.0,-1.0);
            for(int i=10; i>=0; i--) {
                glTexCoord2f((pontosDeConstrucaoD[10-i].x)/3.868461, (pontosDeConstrucaoD[10-i].y+1)/5.113091);
                glVertex3f(pontosDeConstrucaoD[10-i].x, pontosDeConstrucaoD[10-i].y, -1*pontosDeConstrucaoD[10-i].z);
            }
        glEnd();
    glPopMatrix();
    textureManager->Disable();
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
