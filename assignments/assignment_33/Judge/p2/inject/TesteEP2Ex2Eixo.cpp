#include "Teste.h"
#include "Eixo.h"

CASO_DE_TESTE (Eixo$_construtor_e_metodos_get) {
  Eixo e("TituloA", 5, 8);

  return e.getTitulo() == "TituloA" &&
         e.getMinimo() == 5 &&
         e.getMaximo() == 8;
}

CASO_DE_TESTE (Eixo$_construtor_e_destrutor) {
  Eixo *e = new Eixo("AAAA", 0, 0);
  delete e;

  return true;
}
