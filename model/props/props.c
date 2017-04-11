#include <math.h>
#include "props.h"

#define radianoParaGraus(radianos) (radianos * (180.0 / M_PI))
#define grausParaRadianos(graus) ((graus * M_PI) / 180.0)

/** Vetor - Calcula ângulo entre vetores
* @param v1 Vetor 1
* @param v2 Vetor 2
* @return ângulo entre os vetores v1 e v2
**/
double vetor_calculaAngulo(Vetor v1, Vetor v2){
    // Normaliza os 2 vetores
    vetor_normalizaVetor(&v1);
    vetor_normalizaVetor(&v2);

    double _angulo = acos(vetor_produtoEscalar(v1,v2));
    if(v1.y < 0) _angulo=2*M_PI-_angulo; // A função normalmente é definida de 0 a 180, com isso passa a ser de 0 a 360
    return radianoParaGraus(_angulo);
}

/** Vetor - Calcula produto escalar entre vetores
* @param v1 Vetor 1
* @param v2 Vetor 2
* @return o produto escalar entre os vetores v1 e v2
**/
double vetor_produtoEscalar(Vetor v1, Vetor v2){
    return (v1.x*v2.x)+(v2.y+v2.y);
}

/** Vetor - Normaliza vetor
* @param v Vetor a ser normalizado
**/
void vetor_normalizaVetor(Vetor *v){
    double _moduloVetor = vetor_calculaModulo(*v);
    v->x = v->x/_moduloVetor;
    v->y = v->y/_moduloVetor;
}

/** Vetor - Calcula módulo
* @param v Vetor a ter módulo calculado
* @return o módulo de v
**/
double vetor_calculaModulo(Vetor v){
    double _moduloVetor = sqrt(pow(v.x,2)+pow(v.y,2));
    return _moduloVetor;
}