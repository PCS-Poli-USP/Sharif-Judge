#include "Tela.h"

const int Tela::LARGURA_PADRAO = 400;
const int Tela::ALTURA_PADRAO = 300;

Tela::Tela() {
}

void Tela::setEixoX(string nome, double minimo, double maximo) {
  minimoX = minimo;
  maximoX = maximo;
  nomeX = nome;
}

void Tela::setEixoY(string nome, double minimo, double maximo) {
  minimoY = minimo;
  maximoY = maximo;
  nomeY = nome;
}

void Tela::plotar(string nomeDaSerie, double x, double y) {
    series[nomeDaSerie].push_back(pair<double, double>(x, y));
  }

void Tela::mostrar(int largura, int altura) {
  // Nada, por enquanto
}

void Tela::mostrar() {
  Tela::mostrar(Tela::LARGURA_PADRAO, Tela::ALTURA_PADRAO);
}

Tela::~Tela() {
}
