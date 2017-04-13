typedef struct opcao{
    char *opcao;
    struct opcao *proximo;
} Opcao;

typedef struct menu{
    Vetor posicao;
    char *titulo;
    Opcao *opcoes; // Opçoes
    int n_opcoes; // Número de opções
    int opcaoAtual;
    int estaAberto;
} Menu;

Opcao *opcao_adicionaOpcao(Opcao *lista, char *opcao);

Menu menu_criaMenu(Vetor posicao, char *titulo, Opcao *opcoes, int n_opcoes);
void menu_desenhaMenu(Menu *menu);