typedef struct tiro{
    Vetor posicao;
    double velocidade; // O módulo da velocidade, ela sempre fará vetor com a inclinação
    Dimensoes dimensoes;
    double inclinacao;
} Tiro;

typedef struct listaTiro{
    Tiro tiro;
    struct listaTiro *proximo;
} ListaTiro;

Tiro tiro_criaTiro(Vetor posicao, double velocidade, double inclinacao);
void tiro_desenhaTiro(Tiro *tiro);
void tiro_atualizaPosicao(Tiro *tiro, Vetor posicao);

ListaTiro *listatiro_adicionaTiro(ListaTiro *lista, Tiro tiro);
ListaTiro *listatiro_deletaTiro(ListaTiro *lista, Tiro *tiro);