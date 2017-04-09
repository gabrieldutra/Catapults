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

ListaAsteroide *listaasteroide_adicionaAsteroide(ListaAsteroide *lista, Asteroide asteroide);
ListaAsteroide *listaasteroide_deletaAsteroide(ListaAsteroide *lista, Asteroide *asteroide);