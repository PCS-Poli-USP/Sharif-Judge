#include "Tela.h"

const int Tela::LARGURA_PADRAO = 400;
const int Tela::ALTURA_PADRAO = 300;

map<string, vector<pair<double, double>>> Tela::series;
double Tela::minimoX = 0, Tela::minimoY = 0, Tela::maximoX = 0, Tela::maximoY = 0;
string Tela::nomeX = "", Tela::nomeY = "";
bool Tela::chamou = false;

Tela::Tela() {
  series.clear();
  minimoX = 0, minimoY = 0, maximoX = 0, maximoY = 0;
  nomeX = "", nomeY = "";
  chamou = false;
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
  chamou = true;
}

void Tela::mostrar() {
  Tela::mostrar(Tela::LARGURA_PADRAO, Tela::ALTURA_PADRAO);
}

Tela::~Tela() {
}

