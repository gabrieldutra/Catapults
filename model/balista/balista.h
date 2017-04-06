#define BALISTA_TEMPO_RECARGA 500 // Constante do tempo de recarga para a Balista (em ms)

typedef struct balista{
    Vetor posicao;
    double velocidade; // O módulo da velocidade, ela sempre fará vetor com a inclinação
    Dimensoes dimensoes;
    double inclinacao;
    int podeAtirar;
} Balista;

Balista balista_criaBalista();
void balista_desenhaBalista();
