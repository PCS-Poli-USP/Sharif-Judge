#include "Ponto.h"
#include <cmath>
#include <iostream>

using namespace std;

Ponto::Ponto(double x, double y) : x(x), y(y) {
}

Ponto::~Ponto() {
}

bool Ponto::eIgual (Ponto* outro) {
  const double epsilon = 1e-5/* some small number such as 1e-5 */;
  return std::abs(x - outro->getX()) <= epsilon && std::abs(y - outro->getY()) <= epsilon;
}

void Ponto::imprimir() {
  cout << "(" << x << ", " << y << ")" << endl;
}

double Ponto::getX() {
  return x;
}

double Ponto::getY() {
  return y;
}
