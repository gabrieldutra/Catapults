#define ASTEROIDE_VELOCIDADE_MINIMA 1
#define ASTEROIDE_VELOCIDADE_MAXIMA 3 // A velocidade dos asteróides varia entre ASTEROIDE_VELOCIDADE_MINIMA e ASTEROIDE_VELOCIDADE_MAXIMA
#define ASTEROIDE_NUMLADOS 25

typedef struct asteroide{
    Vetor posicao;
    double velocidade; // O módulo da velocidade, ela sempre fará vetor com a inclinação
    Dimensoes dimensoes;
    double inclinacao;
} Asteroide;

typedef struct listaAsteroide{
    Asteroide asteroide;
    struct listaAsteroide *proximo;
} ListaAsteroide;

Asteroide asteroide_criaAsteroide(Vetor posicao, double velocidade, double inclinacao);
void asteroide_desenhaAsteroide(Asteroide *asteroide);
int asteroide_checaColisaoComTiro(Asteroide asteroide, Tiro tiro);
int asteroide_checaColisaoComBalista(Asteroide asteroide, Balista balista);

ListaAsteroide *listaasteroide_adicionaAsteroide(ListaAsteroide *lista, Asteroide asteroide);
ListaAsteroide *listaasteroide_deletaAsteroide(ListaAsteroide *lista, Asteroide *asteroide);
int listaasteroide_contaAsteroides(ListaAsteroide *lista);
