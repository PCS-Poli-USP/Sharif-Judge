#ifndef TELA_H_INCLUDED
#define TELA_H_INCLUDED

#ifndef TELA_H
#define TELA_H

// Constantes
#define LARGURA_UTIL 61
#define ALTURA_UTIL 16

#define DIVISAO_ABSCISSA 10
#define DIVISAO_ORDENADA 5

#include "Eixo.h"

using namespace std;

/**
 * Eh um tipo que define apenas dois valores: SUPERIOR e INFERIOR.
 * Para usar eh so fazer: Limite::SUPERIOR ou Limite::INFERIOR.
 */
enum Limite{SUPERIOR, INFERIOR};

/**
 * Representa a tela onde o grafico sera plotado.
 * @Version EP1
 */
class Tela {
public:
  // Nao se preocupem com esses dois metodos.
  // Ignorem eles por enquanto!
  Tela();
  ~Tela();

  /**
   * Apaga a tela.
   */
  void apagar();

  /**
   * Adiciona um ponto na tela, nas coordenadas x e y da Tela.
   *
   * A tela tem tamanho LARGURA_UTIL x ALTURA_UTIL. Portanto
   * 0 <= x < LARGURA_UTIL e 0 <= y < ALTURA_UTIL.
   */
  void adicionarPontoEm (int x, int y);

  /**
   * Adiciona um ponto fora do limite do eixo das ordenadas.
   *
   * Por exemplo, se deseja-se desenhar o ponto (0, 11) no Grafico,
   * mas o Eixo y tem maximo 10, o ponto deve ser colocado no limite
   * superior das ordenadas e no ponto equivalente a x = 0.
   */
  void adicionarForaDoLimiteDasOrdenadasEm(int x, Limite y);

  /**
   * Adiciona um ponto fora do limite do eixo das abscissas.
   *
   * Por exemplo, se deseja-se desenhar o ponto (0, 11), mas o Eixo x tem
   * minimo 5, o ponto deve ser colocado no limite inferior das
   * abscissas e no ponto equivalente a y = 11.
   */
  void adicionarForaDoLimiteDasAbscissasEm(int y, Limite x);

  /**
   * Adiciona um ponto fora do limite de ambos os eixos.
   *
   * Por exemplo, se deseja-se desenhar o ponto (0, 11), mas o Eixo x tem
   * minimo 5 e o Eixo y tem maximo 10 e esse ponto estaria fora da escala,
   * o ponto deve ser colocado no limite inferior das abscissas e no superior
   * das ordenadas.
   */
  void adicionarForaDoLimiteEm(Limite x, Limite y);

  /**
   * Define o eixo das abscissas.
   */
  void setEixoDasAbscissas(Eixo* abscissas);

  /**
   * Define o eixo das ordenadas.
   */
  void setEixoDasOrdenadas(Eixo* ordenadas);

  /**
   * Desenha a tela, colocando os eixos definidos e os pontos adicionados.
   */
  void desenhar();

private:
  void incluirEixoDasAbscissas();
  void incluirEixoDasOrdenadas();
  int getColunaEixo();
  int getLinhaEixo();
  void limpar();
  void escrever(string texto, int x, int y, int largura);
  void escrever(double valor, int x, int y, int largura);
  char** buffer;
  Eixo *abscissas = nullptr;
  Eixo *ordenadas = nullptr;
  int IASDF;
};

#endif // TELA_H
#endif
