#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <math.h>
#include "../props/props.h"
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
    // Começa a usar a cor cinza
    glColor3f(.5, .5, .5);

    // Desenha um triângulo
    glBegin(GL_TRIANGLES);
        glVertex2f(-32, -32);
        glVertex2f( 32,  0);
        glVertex2f(-32,  32);
    glEnd();
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