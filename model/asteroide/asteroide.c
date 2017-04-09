#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <math.h>
#include "../props/props.h"
#include "asteroide.h"

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