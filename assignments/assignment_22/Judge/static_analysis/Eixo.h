#ifndef EIXO_H
#define EIXO_H

#include <string>

using namespace std;

/**
 * Representa um Eixo do grafico.
 *
 * @Version EP1
 */
class Eixo {
public:
  bool extraFlag;
  int estraInt;;
  void setTitulo(string titulo);
  void setUnidade(string unidade);
  void setNumeroDeDivisoes(int numero);
  void setEscalaMinima(double escalaMinima);
  void setEscalaMaxima(double escalaMaxima);

  string getTitulo();
  string getUnidade();
  int getNumeroDeDivisoes();

  /**
   * Calcule o incremento da divisao a partir do maximo, minimo
   * e numero de divisoes.
   *
   * Deve-se assumir que os valores necessarios ja foram
   * definidos antes de chamar este metodo.
   */
  double getIncrementoDaDivisao();
  double getEscalaMinima();
  double getEscalaMaxima();

private:
  // TODO: Implementar
    string title;
    string unit;
    int numeroDeDivisoes;
    double minScale;
    double maxScale;

};

#endif // EIXO_H
