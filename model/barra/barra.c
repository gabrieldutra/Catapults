#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <math.h>
#include "../props/props.h"
#include "barra.h"

/** Barra - Construtor da Barra
* @return Barra com valores iniciais
**/
Barra barra_criaBarra(Vetor posicao, Dimensoes dimensoes, double valor, double corR, double corG, double corB){
    Barra _novaBarra;
    
    _novaBarra.posicao = posicao;
    _novaBarra.dimensoes = dimensoes;
    _novaBarra.valor = valor;
    _novaBarra.corR = corR;
    _novaBarra.corG = corG;
    _novaBarra.corB = corB;

    return _novaBarra;
}

/** Barra - Desenha Barra
* @param barra a ser desenhada
**/
void barra_desenhaBarra(Barra *barra){
    // Cor preta
    glColor3f(0, 0, 0);

    // Desenha a parte preta da barra
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(-barra->dimensoes.width, -barra->dimensoes.height);
        glVertex2f( -barra->dimensoes.width,  barra->dimensoes.height);
        glVertex2f(barra->dimensoes.width,  barra->dimensoes.height);
        glVertex2f(barra->dimensoes.width,  -barra->dimensoes.height);
    glEnd();

    // Começa a usar a cor da barra com a tonalidade de acordo com o valor
    glColor3f((barra->corR/2)+(barra->corR/2)*(barra->valor/100),
    (barra->corB/2)+(barra->corG/2)*(barra->valor/100),
    (barra->corB/2)+(barra->corB/2)*(barra->valor/100));
}