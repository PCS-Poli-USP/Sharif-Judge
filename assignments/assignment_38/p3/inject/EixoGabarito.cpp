#include "EixoGabarito.h"

EixoGabarito::EixoGabarito(string titulo, double minimo, double maximo) : Eixo(titulo, minimo, maximo), tituloGabarito(titulo), minimoGabarito(minimo), maximoGabarito(maximo) {
}

string EixoGabarito::getTitulo() {
  return tituloGabarito;
}

double EixoGabarito::getMinimo() {
  return minimoGabarito;
}

double EixoGabarito::getMaximo() {
  return maximoGabarito;
}

EixoGabarito::~EixoGabarito() {
}
