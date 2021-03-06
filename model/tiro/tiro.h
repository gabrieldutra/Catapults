#define TIRO_VELOCIDADE_MAXIMA 15
#define TIRO_VELOCIDADE_MINIMA 8

typedef struct tiro{
    Vetor posicao;
    double velocidade; // O módulo da velocidade, ela sempre fará vetor com a inclinação
    Dimensoes dimensoes;
    double inclinacao;
    int textura;
} Tiro;

typedef struct listaTiro{
    Tiro tiro;
    struct listaTiro *proximo;
} ListaTiro;

Tiro tiro_criaTiro(Vetor posicao, double velocidade, double inclinacao, int *textura);
void tiro_desenhaTiro(Tiro *tiro);
void tiro_atualizaPosicao(Tiro *tiro, Vetor posicao);

ListaTiro *listatiro_adicionaTiro(ListaTiro *lista, Tiro tiro);
ListaTiro *listatiro_deletaTiro(ListaTiro *lista, Tiro *tiro);