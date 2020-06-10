#include "Tela.h"

#include <iostream>
#include <cmath>
#include <stdexcept>
#include <sstream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define SETA_CIMA '^'
#define SETA_DIREITA '>'
#define LINHA_EIXO_ABSCISSAS '-'
#define MARCADOR_EIXO_ABSCISSAS '|'
#define LINHA_EIXO_ORDENADAS '|'
#define MARCADOR_EIXO_ORDENADAS '-'
#define PONTO 'X'
#define ERRO 'O'

#define LARGURA_DA_TELA 74
#define ALTURA_DA_TELA 20

#define LARGURA_TEXTO 6
#define LARGURA_NUMERO 4

#ifndef TELA_CPP
#define TELA_CPP

using namespace std;

Tela::Tela() {
	buffer = new char*[ALTURA_DA_TELA];

	for (int i = 0; i < ALTURA_DA_TELA; i++) {
    buffer[i] = new char[LARGURA_DA_TELA];
	}
	apagar();
}

Tela::~Tela() {
	for (int i = 0; i < ALTURA_DA_TELA; i++) {
    delete[] buffer[i];
	}

	delete[] buffer;
}

void Tela::apagar() {
  limpar();
	for (int h = 0; h < ALTURA_DA_TELA; h++) {
		for (int l = 0; l < LARGURA_DA_TELA; l++) {
			buffer[h][l] = ' ';
		}
	}
}

// http://www.cplusplus.com/articles/4z18T05o/
void Tela::limpar() {

#ifdef _WIN32
  HANDLE                     hStdOut;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD                      count;
  DWORD                      cellCount;
  COORD                      homeCoords = { 0, 0 };

  hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
  if (hStdOut == INVALID_HANDLE_VALUE) return;

  /* Get the number of cells in the current buffer */
  if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
  cellCount = csbi.dwSize.X *csbi.dwSize.Y;

  /* Fill the entire buffer with spaces */
  if (!FillConsoleOutputCharacter(
    hStdOut,
    (TCHAR) ' ',
    cellCount,
    homeCoords,
    &count
    )) return;

  /* Fill the entire buffer with the current colors and attributes */
  if (!FillConsoleOutputAttribute(
    hStdOut,
    csbi.wAttributes,
    cellCount,
    homeCoords,
    &count
    )) return;

  /* Move the cursor home */
  SetConsoleCursorPosition(hStdOut, homeCoords);
#else
  write(1,"\E[H\E[2J",7);
  //cout << "\f";
#endif
}

int Tela::getColunaEixo() {
	return LARGURA_DA_TELA - LARGURA_UTIL - 1;
}

int Tela::getLinhaEixo() {
	return ALTURA_UTIL;
}

void Tela::incluirEixoDasAbscissas() {
  if (abscissas == nullptr) {
    cout << "Eixo das abscissas nao definido." << endl;
    exit(EXIT_FAILURE);
  }

	buffer[getLinhaEixo()][LARGURA_DA_TELA - 1] = SETA_DIREITA;

	// Linha
	for (int i = getColunaEixo(); i < LARGURA_DA_TELA - 1; i++) {
		if (buffer[getLinhaEixo()][i] == ' ')
      buffer[getLinhaEixo()][i] = LINHA_EIXO_ABSCISSAS;
	}

	// Divisoes
	int divisao = 0;
  for (int i = getColunaEixo(); i < LARGURA_DA_TELA - 1; i += DIVISAO_ABSCISSA) {
    buffer[getLinhaEixo() + 1][i] = MARCADOR_EIXO_ABSCISSAS;
    escrever(abscissas->getEscalaMinima() + abscissas->getIncrementoDaDivisao() * divisao, i, getLinhaEixo() + 2, LARGURA_NUMERO);
    divisao++;
  }

    // Titulo
  escrever(abscissas->getTitulo(), LARGURA_DA_TELA - 2 * LARGURA_TEXTO - 2, getLinhaEixo() + 3, LARGURA_TEXTO);
  buffer[getLinhaEixo() + 3][LARGURA_DA_TELA - LARGURA_TEXTO - 1] = '(';
  escrever(abscissas->getUnidade(), LARGURA_DA_TELA - LARGURA_TEXTO, getLinhaEixo() + 3, LARGURA_TEXTO - 2);
  int posicaoFechaParenteses = LARGURA_DA_TELA - 1;
  while (posicaoFechaParenteses > LARGURA_DA_TELA - LARGURA_TEXTO - 1 && buffer[getLinhaEixo() + 3][posicaoFechaParenteses] == ' ') {
    posicaoFechaParenteses--;
  }
  buffer[getLinhaEixo() + 3][posicaoFechaParenteses + 1] = ')';
}

void Tela::incluirEixoDasOrdenadas() {
  if (ordenadas == nullptr) {
    cout << "Eixo das ordenadas nao definido." << endl;
    exit(EXIT_FAILURE);
  }

  buffer[0][getColunaEixo()] = SETA_CIMA;

  // Linha
	for (int i = 1; i < getLinhaEixo(); i++) {
    if (buffer[i][getColunaEixo()] == ' ')
      buffer[i][getColunaEixo()] = LINHA_EIXO_ORDENADAS;
	}

	// Divisoes
	int divisao = 0;
  for (int i = getLinhaEixo(); i > 0; i -= DIVISAO_ORDENADA) {
    buffer[i][getColunaEixo() - 1] = MARCADOR_EIXO_ORDENADAS;
    escrever(ordenadas->getEscalaMinima() + ordenadas->getIncrementoDaDivisao() * divisao, getColunaEixo() - 1 - LARGURA_NUMERO, i, LARGURA_NUMERO);
    divisao++;
  }

  // Titulo
  escrever(ordenadas->getTitulo(), 0, 1, LARGURA_TEXTO);
  buffer[2][0] = '(';
  escrever(ordenadas->getUnidade(), 1, 2, LARGURA_TEXTO - 2);
  int posicaoFechaParenteses = LARGURA_TEXTO - 1;
  while (posicaoFechaParenteses > 0 && buffer[2][posicaoFechaParenteses] == ' ') {
    posicaoFechaParenteses--;
  }
  buffer[2][posicaoFechaParenteses + 1] = ')';
}

void Tela::escrever(string texto, int x, int y, int largura) {
  // Escreve no maximo largura caracteres
  for (int i = 0; i < largura && i < (int) texto.length() && x + i < LARGURA_DA_TELA; i++) {
    buffer[y][x + i] = texto[i];
  }
}

void Tela::escrever(double valor, int x, int y, int largura) {
  // Escreve no maximo largura caracteres

  // to_string tem problemas no mingw
  ostringstream sstream;
  sstream << valor;
  string texto = sstream.str();

  if (valor > pow(10, largura)) {
    cout << "Valor maior que o possivel de representar: " << texto << endl;
    exit(EXIT_FAILURE);
  }
  escrever(texto, x, y, largura);
}

void Tela::adicionarPontoEm (int x, int y) {
  if (x < 0 || x >= LARGURA_UTIL) {
    cout << "x fora dos limites: " << x << endl;
    exit(EXIT_FAILURE);
  }

  if (y < 0 || y >= ALTURA_UTIL) {
    cout << "y fora dos limites: " << y << endl;
    exit(EXIT_FAILURE);
  }

  buffer[getLinhaEixo() - y][getColunaEixo() + x] = PONTO;
}

void Tela::adicionarForaDoLimiteDasOrdenadasEm(int x, Limite y) {
  if (x >= 0 && x < LARGURA_UTIL) {
    if (y == Limite::SUPERIOR)
      buffer[0][getColunaEixo() + x] = ERRO;
    else buffer[getLinhaEixo() + 1][getColunaEixo() + x] = ERRO;
  }
}

void Tela::adicionarForaDoLimiteDasAbscissasEm(int y, Limite x) {
  if (y >= 0 && y < ALTURA_UTIL) {
    if (x == Limite::SUPERIOR)
      buffer[getLinhaEixo() - y][LARGURA_DA_TELA - 1] = ERRO;
    else buffer[getLinhaEixo() - y][getColunaEixo() - 1] = ERRO;
  }
}

void Tela::adicionarForaDoLimiteEm(Limite x, Limite y) {
  int posicaoX, posicaoY;

  if (x == Limite::SUPERIOR)
    posicaoX = LARGURA_DA_TELA - 1;
  else posicaoX = getColunaEixo() - 1;

  if (y == Limite::SUPERIOR)
    posicaoY = 0;
  else posicaoY = getLinhaEixo() + 1;

  buffer[posicaoY][posicaoX] = ERRO;
}

void Tela::setEixoDasAbscissas (Eixo* abscissas) {
	this->abscissas = abscissas;
}

void Tela::setEixoDasOrdenadas (Eixo* ordenadas) {
	this->ordenadas = ordenadas;
}

void Tela::desenhar() {
  incluirEixoDasAbscissas();
  incluirEixoDasOrdenadas();

	for (int h = 0; h < ALTURA_DA_TELA; h++) {
      for (int l = 0; l < LARGURA_DA_TELA; l++) {
        cout << buffer[h][l];
      }
      cout << endl;
	}
}

#endif
