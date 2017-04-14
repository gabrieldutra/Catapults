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

    _novaBalista.dimensoes.width = 64;
    _novaBalista.dimensoes.height = 64;
    _novaBalista.posicao.x = 0;
    _novaBalista.posicao.y = 0;
    _novaBalista.velocidade = 0;
    _novaBalista.inclinacao = 0;
    _novaBalista.podeAtirar = BALISTA_TEMPO_RECARGA;

    _novaBalista.textura[0] = SOIL_load_OGL_texture(
        "textures/balista_carregada.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);

    _novaBalista.textura[1] = SOIL_load_OGL_texture(
        "textures/balista_descarregada.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);

    _novaBalista.textura[2] = SOIL_load_OGL_texture(
        "textures/balista_tiro.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);

    _novaBalista.atualTextura = 0;

    return _novaBalista;
}

/** Balista - Desenha Balista
* @param Balista a ser desenhada
**/
void balista_desenhaBalista(Balista *balista){
   // Começa a usar a cor branca
    glColor3f(1, 1, 1);

    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Desenha o tiro
    glBindTexture(GL_TEXTURE_2D, balista->textura[balista->atualTextura]);
    
    float valorSprite = (float) ((int) abs(balista->posicao.x+balista->posicao.y)%2)/2;
    glRotatef(-90, 0, 0, 1);
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(valorSprite, 0); glVertex2f(-(balista->dimensoes.width/2), -(balista->dimensoes.height/2));
        glTexCoord2f(valorSprite, 1); glVertex2f(-(balista->dimensoes.width/2),  (balista->dimensoes.height/2));
        glTexCoord2f(.5 + valorSprite, 1); glVertex2f((balista->dimensoes.width/2),  (balista->dimensoes.height/2));
        glTexCoord2f(.5 + valorSprite, 0); glVertex2f((balista->dimensoes.width/2),  -(balista->dimensoes.height/2));
    glEnd();
    glDisable(GL_TEXTURE_2D);
}