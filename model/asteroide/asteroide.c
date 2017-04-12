#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <math.h>
#include "../props/props.h"
#include "../tiro/tiro.h"
#include "../balista/balista.h"
#include "asteroide.h"


/** Asteroide - Construtor do Asteroide
* @param posicao Vetor da posicao inicial do asteroide
* @param velocidade a velocidade do Asteroide
* @param inclinacao a inclinação do asteroide
* @return Asteroide com as variáveis criadas
**/
Asteroide asteroide_criaAsteroide(Vetor posicao, double velocidade, double inclinacao){
    Asteroide _novoAsteroide;

    Dimensoes _dimensoesAsteroide;
    _dimensoesAsteroide.height = 32;
    _dimensoesAsteroide.width = 32;

    _novoAsteroide.posicao = posicao;
    _novoAsteroide.dimensoes = _dimensoesAsteroide;
    _novoAsteroide.velocidade = velocidade;
    _novoAsteroide.inclinacao = inclinacao;
    return _novoAsteroide;
}

/** Asteroide - Desenha Asteroide
* @param asteroide a ser desenhado
**/
void asteroide_desenhaAsteroide(Asteroide *asteroide){
    int i;
    // Começa a usar a cor cinza
    glColor3f(.5, .5, .5);

    // Desenha um triângulo
    glBegin(GL_TRIANGLE_FAN);
        for(i=0;i<ASTEROIDE_NUMLADOS;i++){
            glVertex3f(asteroide->dimensoes.width*cos(i*2*M_PI/ASTEROIDE_NUMLADOS), asteroide->dimensoes.height*sin(i*2*M_PI/ASTEROIDE_NUMLADOS), 0);
        }

    glEnd();
}

/** Asteroide - Checa colisão do asteróide com o tiro
* @param asteroide a ser verificado
* @param tiro a ser verificado
* @return int teste de colisão
**/
int asteroide_checaColisaoComTiro(Asteroide asteroide, Tiro tiro){
    Vetor _distancia;
    _distancia.x=asteroide.posicao.x-tiro.posicao.x;
    _distancia.y=asteroide.posicao.y-tiro.posicao.y;
    return (vetor_calculaModulo(_distancia)<=asteroide.dimensoes.width);
}

/** Asteroide - Checa colisão do asteróide com a balista
* @param asteroide a ser verificado
* @param balista a ser verificada
* @return int teste de colisão
**/
int asteroide_checaColisaoComBalista(Asteroide asteroide, Balista balista){
    Vetor _distancia;
    _distancia.x=asteroide.posicao.x-balista.posicao.x;
    _distancia.y=asteroide.posicao.y-balista.posicao.y;
    return (vetor_calculaModulo(_distancia)<=asteroide.dimensoes.width+balista.dimensoes.height);
}

/** Lista Asteroide - Adicionar um asteroide na lista
* @param lista Lista que vai receber o elemento
* @param asteroide Asteroide a ser adicionado
**/
ListaAsteroide *listaasteroide_adicionaAsteroide(ListaAsteroide *lista, Asteroide asteroide){
    ListaAsteroide *_novaLista = (ListaAsteroide *) malloc(sizeof(ListaAsteroide));
    _novaLista->asteroide = asteroide;
    _novaLista->proximo = lista;
    return _novaLista;
}

/** Lista Asteroide - Deletar um asteroide da lista
* @param lista Lista que vai ter o elemento removido
* @param asteroide Asteróide a ser deletado
**/
ListaAsteroide *listaasteroide_deletaAsteroide(ListaAsteroide *lista, Asteroide *asteroide){
    // Percorre a lista até achar o elemento
    ListaAsteroide *_elementoAnterior = NULL;
    ListaAsteroide *_primeiroElemento = lista; // Salva o primeiro elemento
    while(lista != NULL){
        if(&lista->asteroide == asteroide){
            if(_elementoAnterior == NULL) return lista->proximo; // Caso o anterior seja NULL apenas retorna
            _elementoAnterior->proximo = lista->proximo; // Pula o elemento atual e retorna a posição inicial da lista
            return _primeiroElemento;
        }
        _elementoAnterior = lista;
        lista = lista->proximo;
    }
    return _primeiroElemento; // Caso não tenha encontrado nada só retorna o primeiro elemento
}

/** Lista Asteroide - Contar asteróides
* @param lista Lista de asteróides
* @return int o número de asteróides no mapa
**/
int listaasteroide_contaAsteroides(ListaAsteroide *lista){
    int _numeroAsteroides = 0;
    // Percorre a lista contando
    while(lista != NULL){
        _numeroAsteroides++;
        lista = lista->proximo;
    }
    return _numeroAsteroides;
}
