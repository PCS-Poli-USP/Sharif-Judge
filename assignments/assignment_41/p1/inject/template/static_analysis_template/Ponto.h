#ifndef PONTO_H
#define PONTO_H
#define eps 1e-5
#include <math.h>

class Ponto {
public:
    double X;
    double Y;

    Ponto(double x, double y);
    virtual ~Ponto();

    /**
    * Obtem o valor do Ponto na coordenada horizontal (x).
    */
    double getX();

    /**
    * Obtem o valor do Ponto na coordenada vertical (y).
    */
    double getY();



  /**
   * Imprime na saida padrao (cout) o Ponto no formato (x, y).
   * Pule uma linha apos imprimir o Ponto.
   */
  void imprimir();

  /**
   * Informa se este Ponto eh igual a outro.
   * Um Ponto eh igual se os valores x e y dos Pontos sao
   * suficientemente proximos.
   */
  bool eIgual(Ponto* outro);

  // ADICIONE OS ATRIBUTOS NECESSARIOS
};

#endif // PONTO_H
