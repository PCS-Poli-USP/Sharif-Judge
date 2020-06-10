#ifndef PONTO_H
#define PONTO_H

using namespace std; // Para evitar erro de compilacao de um aluno

class Ponto {
public:
  /**
   * Cria um Ponto informando as coordenadas x e y.
   */
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
   * @param outro Um outro Ponto.
   */
  bool eIgual(Ponto* outro);
private:
  double x, y;
};

#endif // PONTO_H
