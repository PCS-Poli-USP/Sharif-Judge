#include "Teste.h"

double distanciaEntrePontos(double x1, double y1, double x2, double y2);

CASO_DE_TESTE (Teste_distanciaEntrePontos_Pontos_no_mesmo_quadrante){
  return nearlyEqual(distanciaEntrePontos(1.0, 1.0, 2.0, 2.0), 1.4, 0.5);
}

CASO_DE_TESTE (Teste_distanciaEntrePontos_Pontos_em_quadrantes_diferentes){
  return nearlyEqual(distanciaEntrePontos(1.0, 1.0, -1.0, -1.0), 2.8, 0.5);
}

CASO_DE_TESTE (Teste_distanciaEntrePontos_Ponto_na_origem){
  return nearlyEqual(distanciaEntrePontos(0.0, 0.0, 1.0, 1.0), 1.4, 0.5);
}

CASO_DE_TESTE (Teste_distanciaEntrePontos_Pontos_iguais){
  return nearlyEqual(distanciaEntrePontos(1.0, 1.0, 1.0, 1.0), 0, 0.1);
}
