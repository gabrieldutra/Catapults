typedef struct barra{
    Vetor posicao;
    Dimensoes dimensoes;
    double valor; // Valor de 0 a 100 com o valor da barra
    double corR; // Cor da barra quando cheia
    double corG;
    double corB;
} Barra;

Barra barra_criaBarra(Vetor posicao, Dimensoes dimensoes, double valor, double corR, double corG, double corB);
void barra_desenhaBarra(Barra *barra);