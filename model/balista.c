#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <math.h>
#include "props.h"
#include "balista.h"

/** Balista - Cria Balista com os valores iniciais
* @return Balista com valores iniciais
**/
Balista balista_criaBalista(){
    Balista _nova;

    _nova.dimensoes.width = 32;
    _nova.dimensoes.height = 32;
    _nova.posicao.x = 0;
    _nova.posicao.y = 0;
    _nova.velocidade = 0;
    _nova.inclinacao = 0;
    _nova.podeAtirar = BALISTA_TEMPO_RECARGA;

    return _nova;
}

/** Balista - Desenha Balista
* @param Balista a ser desenhada
**/
void balista_desenhaBalista(Balista *balista){
    // Começa a usar a cor azul
    glColor3f(.5, .5, 1);

    // Desenha um triângulo
    glBegin(GL_TRIANGLES);
        glVertex2f(-16, -16);
        glVertex2f( 16,  0);
        glVertex2f(-16,  16);
    glEnd();
}