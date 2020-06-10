#include "SerieTemporal.h"
#include <cmath>

SerieTemporal::SerieTemporal(string nome, string nomeDoCanalY) : Serie(nome, "Tempo", nomeDoCanalY) {
}

SerieTemporal::~SerieTemporal() {
}

void SerieTemporal::adicionar(double valor) {
  int quantidadeAntes = quantidade;
  adicionar(tempo, valor);
  if (quantidade > quantidadeAntes) tempo++;
}

void SerieTemporal::adicionar(double x, double y) {
  const double epsilon = 1e-5/* some small number such as 1e-5 */;

  for (int i = 0; i < quantidade; i++) {
    if (std::abs(x - pontos[i]->getX()) <= epsilon) {
      Ponto* novo = new Ponto(x, y);
      delete pontos[i];
      pontos[i] = novo;
      return;
    }
  }

  if (x >= 1) Serie::adicionar(x, y);
}
