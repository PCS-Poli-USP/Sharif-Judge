#include "Teste.h"
#include <cmath>

double distanciaDoPontoDeMenorOrdenadaAOrigem(double abscissas[], double ordenadas[], int quantidadeDePontos);

CASO_DE_TESTE (Teste_distanciaDoPontoDeMenorOrdenadaAOrigem_Array_com_um_ponto) {
  double abscissas[] = {1.0};
  double ordenadas[] = {1.0};

  return nearlyEqual (distanciaDoPontoDeMenorOrdenadaAOrigem(abscissas, ordenadas, 1), 1.41421);
}

CASO_DE_TESTE (Teste_distanciaDoPontoDeMenorOrdenadaAOrigem_Primeiro_Ponto_Do_Array) {
  double abscissas[] = {1.0, 2.0, 3.0, 4.0, 5.0};
  double ordenadas[] = {0.5, 2.6, 3.2, 6.0, 9.7};

  return nearlyEqual (distanciaDoPontoDeMenorOrdenadaAOrigem(abscissas, ordenadas, 5), 1.11803);
}

CASO_DE_TESTE (Teste_distanciaDoPontoDeMenorOrdenadaAOrigem_Ponto_No_Meio_Do_Array) {
  double abscissas[] = {1.0, 2.0, 3.0, 4.0, 5.0};
  double ordenadas[] = {2.0, 2.6, 0.8, 6.0, 9.7};

  return nearlyEqual (distanciaDoPontoDeMenorOrdenadaAOrigem(abscissas, ordenadas, 5), 3.10483);
}

CASO_DE_TESTE (Teste_distanciaDoPontoDeMenorOrdenadaAOrigem_Ultimo_Ponto_Do_Array) {
  double abscissas[] = {1.0, 2.0, 3.0, 4.0, 5.0};
  double ordenadas[] = {2.0, 2.6, 3.2, 6.0, 1.0};

  return nearlyEqual (distanciaDoPontoDeMenorOrdenadaAOrigem(abscissas, ordenadas, 5), 5.09902);
}

CASO_DE_TESTE (Teste_distanciaDoPontoDeMenorOrdenadaAOrigem_Ponto_de_ordenada_negativo) {
  double abscissas[] = {3.0, -1.0, 2.0};
  double ordenadas[] = {3.0, -1.0, 2.0};

  return nearlyEqual (distanciaDoPontoDeMenorOrdenadaAOrigem(abscissas, ordenadas, 3), 1.41421);
}
