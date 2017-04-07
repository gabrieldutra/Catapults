#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <math.h>
#include "../props/props.h"
#include "balista.h"

/** Balista - Cria Balista com os valores iniciais
* @return Balista com valores iniciais
**/
Balista balista_criaBalista(){
    Balista _novaBalista;

    _novaBalista.dimensoes.width = 32;
    _novaBalista.dimensoes.height = 32;
    _novaBalista.posicao.x = 0;
    _novaBalista.posicao.y = 0;
    _novaBalista.velocidade = 0;
    _novaBalista.inclinacao = 0;
    _novaBalista.podeAtirar = BALISTA_TEMPO_RECARGA;

    return _novaBalista;
}

/** Balista - Desenha Balista
* @param Balista a ser desenhada
**/
void balista_desenhaBalista(Balista *balista){
    // Começa a usar a cor azul
    glColor3f(1 - ((float) balista->podeAtirar/BALISTA_TEMPO_RECARGA)/2, .5, .5+ ((float) balista->podeAtirar/BALISTA_TEMPO_RECARGA)/2);

    // Desenha um triângulo
    glBegin(GL_TRIANGLES);
        glVertex2f(-16, -16);
        glVertex2f( 16,  0);
        glVertex2f(-16,  16);
    glEnd();
}