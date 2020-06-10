#include "Serie.h"

#include <iostream>

using namespace std;

Serie::Serie(string nome, string nomeDoCanalX, string nomeDoCanalY) : nome(nome), nomeDoCanalX(nomeDoCanalX), nomeDoCanalY(nomeDoCanalY) {
}

Serie::~Serie() {
  for (int i = 0; i < quantidade; i++) {
    delete pontos[i];
  }

  delete[] pontos;
}


int Serie::getQuantidade() {
  return quantidade;
}

bool Serie::estaVazia() {
  return quantidade == 0;
}

string Serie::getNome() {
  return nome;
}

string Serie::getNomeDoCanalX() {
  return nomeDoCanalX;

}

string Serie::getNomeDoCanalY() {
  return nomeDoCanalY;
}


void Serie::adicionar (double x, double y) {
  if (quantidade > NUMERO_MAXIMO_VALORES) return;
  pontos[quantidade] = new Ponto(x, y);
  quantidade++;
}

Ponto* Serie::getLimiteSuperior() {
  if (quantidade == 0) return NULL;

  double maiorX = pontos[0]->getX();
  double maiorY = pontos[0]->getY();

  for (int i = 1; i < quantidade; i++) {
      if (maiorX < pontos[i]->getX())
        maiorX = pontos[i]->getX();

      if (maiorY < pontos[i]->getY())
        maiorY = pontos[i]->getY();
  }

  return new Ponto(maiorX, maiorY);
}

Ponto* Serie::getLimiteInferior() {
  if (quantidade == 0) return NULL;

  double menorX = pontos[0]->getX();
  double menorY = pontos[0]->getY();

  for (int i = 1; i < quantidade; i++) {
      if (menorX > pontos[i]->getX())
        menorX = pontos[i]->getX();

      if (menorY > pontos[i]->getY())
        menorY = pontos[i]->getY();
  }

  return new Ponto(menorX, menorY);
}

Ponto* Serie::getPosicao (int posicao) {
  if (posicao >= quantidade || posicao < 0) return nullptr;
  return pontos[posicao];
}

void Serie::imprimir() {
  cout << "Serie " << nome << endl;
  for (int i = 0; i < quantidade; i++) {
    pontos[i]->imprimir();
  }
}
