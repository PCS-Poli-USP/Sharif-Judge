#ifndef SERIE_H
#define SERIE_H

#define NUMERO_MAXIMO_VALORES 10

#include <string>
#include "Ponto.h"
#include <iostream>

using namespace std;

class Serie {
public:

  //Atributos:
  Ponto* Pontos[NUMERO_MAXIMO_VALORES];
  string Nome;
  string NomeDoCanalX;
  string NomeDoCanalY;
  int Quantidade = 0;


  Serie(string Nome, string NomeDoCanalX, string NomeDoCanalY);
  virtual ~Serie();
  // Permite definir o nome, o nomeDoCanalX e o nomeDoCanalY.
  void setNome(string nome);
  void setNomeDoCanalX(string nomeDoCanalX);
  void setNomeDoCanalY(string nomeDoCanalY);

  // Permite obter o nome, o nomeDoCanalX e o nomeDoCanalY.
  string getNome();
  string getNomeDoCanalX();
  string getNomeDoCanalY();

  /**
   * Informa a quantidade de pontos que a Serie possui.
   */
  int getQuantidade();

  /**
   * Informa se a Serie esta vazia.
   */
  bool estaVazia();

  /**
   * Adiciona um novo ponto a Serie, informando sua coordenada x e y.
   * Ignora o valor passado caso o NUMERO_MAXIMO_VALORES tenha
   * sido ultrapassado.
   */
  void adicionar(double x, double y);

  /**
   * Obtem um ponto representando o limite superior da Serie.
   * A coordenada x desse ponto deve ser o maximo valor horizontal
   * existente na Serie e a coordenada y deve ser o maximo valor
   * vertical existente na Serie.
   *
   * Caso a Serie nao tenha valores, deve-se retornar NULL.
   *
   * Por exemplo, para a Serie {(2, 3), (5, 1), (1, 2)} o limite
   * superior eh (5, 3).
   */
  Ponto* getLimiteSuperior();

  /**
   * Obtem um ponto representando o limite inferior da Serie.
   * A coordenada x desse ponto deve ser o minimo valor horizontal
   * existente na Serie e a coordenada y deve ser o minimo valor
   * vertical existente na Serie.
   *
   * Caso a serie nao tenha valores, deve-se retornar NULL.
   *
   * Por exemplo, para a Serie {(2, 3), (5, 1), (1, 2)} o limite
   * inferior eh (1, 1).
   */
  Ponto* getLimiteInferior();

  /**
   * Obtem o ponto que esta na posicao definida da Serie. A contagem de
   * posicoes comeca em 0.
   *
   * Em caso de posicoes invalidas, retorne NULL.
   *
   * Por exemplo, para a Serie {(2, 3), (5, 1), (1, 2)}, getPosicao(0)
   * deve retornar (2, 3) e getPosicao(2) deve retornar (1, 2).
   */
  Ponto* getPosicao(int posicao);

  /**
   * Imprime na saida padrao (cout) o nome da Serie e seus pontos
   * seguindo o formato definido.
   */
  void imprimir();

  // ADICIONE OS ATRIBUTOS NECESSARIOS
};

#endif // SERIE_H
