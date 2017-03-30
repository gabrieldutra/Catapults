#include "props.h"

#define BALISTA_TEMPO_RECARGA 500

typedef struct balista{
    Vetor posicao, velocidade, aceleracao;
    Dimensoes dimensoes;
    double inclinacao;
    int podeAtirar;
} Balista;

Balista balista_criaBalista();
void balista_desenhaBalista();
