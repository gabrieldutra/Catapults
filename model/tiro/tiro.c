#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <math.h>
#include "../props/props.h"
#include "tiro.h"

/** Tiro - Construtor do Tiro
* @param posicao Vetor da posicao inicial do tiro
* @param velocidade a velocidade do tiro
* @param inclinacao a inclinação do tiro
* @return tiro com as variáveis criadas
**/
Tiro tiro_criaTiro(Vetor posicao, double velocidade, double inclinacao){
    Tiro _novoTiro;

    Dimensoes _dimensoesTiro;
    _dimensoesTiro.height = 4;
    _dimensoesTiro.width = 16;

    _novoTiro.posicao = posicao;
    _novoTiro.dimensoes = _dimensoesTiro;
    _novoTiro.velocidade = velocidade;
    _novoTiro.inclinacao = inclinacao;
    return _novoTiro;
}

/** Tiro - Desenha Tiro
* @param tiro a ser desenhado
**/
void tiro_desenhaTiro(Tiro *tiro){
    // Começa a usar a cor vermelha
    glColor3f(1, .5, .5);

    // Desenha o tiro
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(-(tiro->dimensoes.width/2), -(tiro->dimensoes.height/2));
        glVertex2f(-(tiro->dimensoes.width/2),  (tiro->dimensoes.height/2));
        glVertex2f((tiro->dimensoes.width/2),  (tiro->dimensoes.height/2));
        glVertex2f((tiro->dimensoes.width/2),  -(tiro->dimensoes.height/2));
    glEnd();
}

/** Tiro - Atualizar posicao
* @param tiro que vai ter sua posição alterada
* @param posicao a nova posição
**/
void tiro_atualizaPosicao(Tiro *tiro, Vetor posicao){
    tiro->posicao = posicao;
}

/** Lista Tiro - Adicionar um tiro na lista
* @param lista Lista que vai receber o elemento
* @param tiro Tiro a ser adicionado
**/
ListaTiro *listatiro_adicionaTiro(ListaTiro *lista, Tiro tiro){
    ListaTiro *_novaLista = (ListaTiro *) malloc(sizeof(ListaTiro));
    _novaLista->tiro = tiro;
    _novaLista->proximo = lista;
    return _novaLista;
}

/** Lista Tiro - Deletar um tiro da lista
* @param lista Lista que vai ter o elemento removido
* @param tiro Tiro a ser deletado
**/
ListaTiro *listatiro_deletaTiro(ListaTiro *lista, Tiro *tiro){
    // Percorre a lista até achar o elemento
    ListaTiro *_elementoAnterior = NULL;
    ListaTiro *_primeiroElemento = lista; // Salva o primeiro elemento
    while(lista != NULL){
        if(&lista->tiro == tiro){
            if(_elementoAnterior == NULL) return lista->proximo; // Caso o anterior seja NULL apenas retorna
            _elementoAnterior->proximo = lista->proximo; // Pula o elemento atual e retorna a posição inicial da lista
            return _primeiroElemento;
        }
        _elementoAnterior = lista;
        lista = lista->proximo;
    }
    return _primeiroElemento; // Caso não tenha encontrado nada só retorna o primeiro elemento
}
