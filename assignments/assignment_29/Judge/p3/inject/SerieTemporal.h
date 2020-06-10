#ifndef SERIETEMPORAL_H
#define SERIETEMPORAL_H

#include <cmath> // Para resolver outro problema de aluno
#include "Serie.h"

class SerieTemporal : public Serie {
public:
  /**
   * Cria uma SerieTemporal informando o nome da Serie e o nome do
   * canalY. O nome do canal X deve ser obrigatoriamente "Tempo".
   */
  SerieTemporal(string nome, string nomeDoCanalY);
  virtual ~SerieTemporal();

  /**
   * Adiciona um novo Ponto a Serie, no instante seguinte ao do
   * ponto anterior. O primeiro ponto deve ser adicionado no instante 1.
   */
  virtual void adicionar(double valor);

  /**
   * Adiciona um Ponto a Serie informando a coordenada x e y.
   * Caso ja exista um ponto suficientemente proximo ao do valor x
   * informado, ao inves de adicionar o ponto, altere o valor anterior
   * (mantendo sua posicao) ao considerar a nova coordenada y.
   */
  virtual void adicionar(double x, double y);
private:
  double tempo = 1;
};

#endif // SERIETEMPORAL_H
