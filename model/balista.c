#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <math.h>
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

    glPushMatrix();
        // Move o sistema de coordenadas para a posição onde deseja-se desenhar
        glTranslatef(balista->posicao.x, balista->posicao.y, 0);
        // Rotaciona o sistema de coordenadas para o ângulo de orientação,
        // no eixo Z (como estamos em 2D, só faz sentido rotacionar em 'z')
        // O ângulo esperado pela glRotate deve estar em graus
        // Os argumentos "0, 0, 1" indicam que a rotação é no eixo Z
        glRotatef(balista->inclinacao, 0, 0, 1);

        // Desenha um triângulo na origem
        glBegin(GL_TRIANGLES);
            glVertex2f(-6, -3);
            glVertex2f( 6,  0);
            glVertex2f(-6,  3);
        glEnd();
    glPopMatrix();
}