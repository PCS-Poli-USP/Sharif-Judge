#ifndef EIXOGABARITO_H
#define EIXOGABARITO_H

#include <string>
#include "Eixo.h"

using namespace std;

/**
 * Redefine TUDO, para garantir
 */
class EixoGabarito : public Eixo {
public:
  EixoGabarito(string titulo, double minimo, double maximo);
  virtual ~EixoGabarito();
  virtual string getTitulo();
  virtual double getMinimo();
  virtual double getMaximo();
private:
  string tituloGabarito;
  double minimoGabarito, maximoGabarito;
};

#endif // EIXOGABARITO
