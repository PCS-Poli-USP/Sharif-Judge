#ifndef SERIE_H
#define SERIE_H

// Numero maximo de valores da serie.
#define NUMERO_MAXIMO_VALORES 10

#include <string>

using namespace std;

/**
 * Representa uma serie, obtida pela placa.
 *
 * Series representam um conjunto de valores sequenciais. Por exemplo,
 * pode-se ter uma serie (1, 2, 3, 5, 7, 11, 13). Por enquanto a serie
 * deve ter no maximo NUMERO_MAXIMO_VALORES. Caso se tente adicionar
 * um valor adicional ao maximo, ele deve ser ignorado.
 *
 * @Version EP1
 */
class Serie {
public:
  void setNome(string nome);
  string getNome();

  /**
   * Informa se a serie esta vazia.
   */
  bool estaVazia();

  /**
   * Informa o numero de valores que a serie possui.
   * Por exemplo, a serie (1, 2, 3, 5) possui tamanho 4.quantidadeDeValores
   */
  int getTamanho();

  /**
   * Obtem o valor que esta na posicao definida da serie. A contagem de
   * posicoes comeca em 0.
   *
   * Em caso de posicoes invalidas, retorne 0.
   *
   * Por exemplo, considere a serie (1, 2, 3, 5, 7, 11, 13). O metodo
   * getValor(0) deve retornar 1; getValor(6) deve retornar 13.
   */
  double getValor(int posicao);

  /**
   * Obtem o maior valor da serie.
   */
  double getMaximo();

  /**
   * Obtem o menor valor da serie.
   */
  double getMinimo();

  /**
   * Adiciona o valor a serie. Ignora o valor passado caso o
   * NUMERO_MAXIMO_VALORES tenha sido ultrapassado.
   *
   * Por exemplo, considere a serie com os valores (1, 2, 3, 5, 7, 11, 13).
   * Ao chamar adicionar(17), a serie ficara (1, 2, 3, 5, 7, 11, 13, 17).
   */
   void adicionar(double valor);
private:
  // TODO: implementar
    double valoresSerie[NUMERO_MAXIMO_VALORES];
    int quantidadeDeValores = 0;
    string name;
    string Ups;
};

#endif // SERIE_H
