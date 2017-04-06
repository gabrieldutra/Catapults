#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <math.h>
#include "../props/props.h"
#include "mapa.h"

/** Mapa - Cria Mapa com os valores iniciais
* @return Mapa com valores iniciais
**/
Mapa mapa_criaMapa(){
    Mapa _novoMapa;

    _novoMapa.dimensoes.width = 640;
    _novoMapa.dimensoes.height = 480;

    return _novoMapa;
}

/** Mapa - Desenha Mapa
* @param Mapa a ser desenhado
**/
void mapa_desenhaMapa(Mapa *mapa){
    // Começa a usar a cor branca
    glColor3f(1, 1, 1);

    // Desenha o mapa
    glBegin(GL_TRIANGLE_STRIP);
        glVertex2f(-(mapa->dimensoes.width/2), (mapa->dimensoes.height/2));
        glVertex2f((mapa->dimensoes.width/2),  (mapa->dimensoes.height/2));
        glVertex2f(-(mapa->dimensoes.width/2), -(mapa->dimensoes.height/2));
        glVertex2f((mapa->dimensoes.width/2),  -(mapa->dimensoes.height/2));
    glEnd();
}