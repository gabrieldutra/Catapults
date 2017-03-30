#include "props.h"

#define CATAPULTA_TEMPO_RECARGA 500

typedef struct catapulta{
    Vetor posicao, velocidade, aceleracao;
    Dimensoes dimensoes;
    double inclinacao;
    int podeAtirar;
} Catapulta;

Catapulta cataputa_criaCatapulta();
