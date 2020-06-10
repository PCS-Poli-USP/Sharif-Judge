#ifndef SERIE_H
#define SERIE_H

#define NUMERO_MAXIMO_VALORES 10

#include <string>
#include "Ponto.h"
// Corrigindo problemas de alunos
#define E 1e-5
#define EPS 1e-5
#define eps 1e-5
#define ERRO 1e-5
// Alunos
// 10336296
// 10337276
// 10355502
// 8994780
// 9267154
// 9358808

using namespace std;

class Serie {
public:
  /**
   * Cria uma Serie informando o nome dela e o nome dos canais X e Y.
   */
  Serie(string nome, string nomeDoCanalX, string nomeDoCanalY);
  virtual ~Serie();

  // Permite obter o nome, o nomeDoCanalX e o nomeDoCanalY.
  virtual string getNome();
  virtual string getNomeDoCanalX();
  virtual string getNomeDoCanalY();

  /**
   * Informa a quantidade de pontos que a Serie possui.
   */
  virtual int getQuantidade();

  /**
   * Informa se a Serie esta vazia.
   */
  virtual bool estaVazia();

  /**
   * Adiciona um novo ponto a Serie, informando sua coordenada x e y.
   *
   * Ignora o valor passado caso o NUMERO_MAXIMO_VALORES tenha
   * sido ultrapassado.
   */
  virtual void adicionar(double x, double y);

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
  virtual Ponto* getLimiteSuperior();

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
  virtual Ponto* getLimiteInferior();

  /**
   * Obtem o ponto que esta na posicao definida da Serie. A contagem de
   * posicoes comeca em 0.
   *
   * Em caso de posicoes invalidas, retorne NULL.
   *
   * Por exemplo, para a Serie {(2, 3), (5, 1), (1, 2)}, getPosicao(0)
   * deve retornar (2, 3) e getPosicao(2) deve retornar (1, 2).
   */
  virtual Ponto* getPosicao(int posicao);

  /**
   * Imprime na saida padrao (cout) o nome da Serie e seus pontos
   * seguindo o formato definido.
   */
  virtual void imprimir();

protected:
  int quantidade = 0;
  Ponto** pontos = new Ponto*[NUMERO_MAXIMO_VALORES];
private:
  string nome;
  string nomeDoCanalX, nomeDoCanalY;
};

#endif // SERIE_H
