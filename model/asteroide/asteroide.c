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
* @param asteroide Asteroide a ser deletado
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