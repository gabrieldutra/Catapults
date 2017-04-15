#include <stdio.h>
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

    _novoMapa.dimensoes.width = 512;
    _novoMapa.dimensoes.height = 512;

    _novoMapa.textura = SOIL_load_OGL_texture(
        "textures/grass.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);

    if(_novoMapa.textura == 0) printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    return _novoMapa;
}

/** Mapa - Desenha Mapa
* @param Mapa a ser desenhado
**/
void mapa_desenhaMapa(Mapa *mapa){
    // Começa a usar a cor verde
    glColor3f(0, 1, 0.3);
    glEnable(GL_TEXTURE_2D);

    // WRAP para resolver linhas pretas entre as texturas
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Desenha o mapa
    glBindTexture(GL_TEXTURE_2D, mapa->textura);
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0, 0); glVertex2f(-(mapa->dimensoes.width/2), (mapa->dimensoes.height/2));
        glTexCoord2f(1, 0); glVertex2f((mapa->dimensoes.width/2),  (mapa->dimensoes.height/2));
        glTexCoord2f(1, 1); glVertex2f((mapa->dimensoes.width/2),  -(mapa->dimensoes.height/2));
        glTexCoord2f(0, 1); glVertex2f(-(mapa->dimensoes.width/2), -(mapa->dimensoes.height/2));
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
