typedef struct vetor{
    double x,y;
} Vetor;

typedef struct dimensoes{
    double width, height;
} Dimensoes;

double vetor_calculaAngulo(Vetor v1, Vetor v2);
double vetor_produtoEscalar(Vetor v1, Vetor v2);
void vetor_normalizaVetor(Vetor *v);
double vetor_calculaModulo(Vetor v);

void escreveTexto(void * font, char *s, float x, float y, float z);