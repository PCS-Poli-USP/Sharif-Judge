#include "Teste.h"

int quadranteDoPonto(double x, double y);

CASO_DE_TESTE (Teste_quadranteDoPonto_primeiroQuadrante) {
    return (quadranteDoPonto(1.0, 1.0) == 1);
}

CASO_DE_TESTE (Teste_quadranteDoPonto_segundoQuadrante) {
    return (quadranteDoPonto(-2.0, 3.5) == 2);
}

CASO_DE_TESTE (Teste_quadranteDoPonto_terceiroQuadrante) {
    return (quadranteDoPonto(-10.0, -10.0) == 3);
}

CASO_DE_TESTE (Teste_quadranteDoPonto_quartoQuadrante) {
    return (quadranteDoPonto(0.5, -5.5) == 4);
}

CASO_DE_TESTE (Teste_quadranteDoPonto_EixoX) {
    return (quadranteDoPonto(0.0, 1.0) == 0);
}

CASO_DE_TESTE (Teste_quadranteDoPonto_EixoY) {
    return (quadranteDoPonto(1.0, 0.0) == 0);
}
