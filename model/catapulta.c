#include "catapulta.h"

/** Catapulta - Cria Catapulta com os valores iniciais
*
**/
Catapulta catapulta_criaCatapulta(){
    Catapulta _nova;
    Dimensoes _d;
    _d.width = 32;
    _d.height = 32;
    Vetor _p;
    _p.x = 0;
    _p.y = 0;
    Vetor _v;
    _v.x = 0;
    _v.y = 0;
    Vetor _a;
    _a.x = 0;
    _a.y = 0;

    _nova.dimensoes = _d;
    _nova.posicao = _p;
    _nova.velocidade = _v;
    _nova.aceleracao = _a;
    _nova.inclinacao = 0;
    _nova.podeAtirar = CATAPULTA_TEMPO_RECARGA;

    return _nova;
}
