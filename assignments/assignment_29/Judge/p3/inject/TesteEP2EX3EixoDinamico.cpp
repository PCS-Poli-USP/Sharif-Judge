#include <sstream>
#include <regex>

#include "Teste.h"
#include "EixoDinamico.h"

CASO_DE_TESTE (EixoDinamico$_construtor_e_titulo) {
  Serie s("ABC", "DEF", "GHI");
  EixoDinamico e1(0, 10, &s, true);
  EixoDinamico e2(0, 10, &s, false);

  return e1.getTitulo() == "DEF" && e2.getTitulo() == "GHI";
}

CASO_DE_TESTE (EixoDinamico$_construtor_e_destrutor) {
  Serie s("xyz", "DEF", "GHI");
  EixoDinamico* e = new EixoDinamico(0, 10, &s, false);
  delete e;

  return true;
}

CASO_DE_TESTE (EixoDinamico$_getMinimo_Serie_vazia) {
  Serie s("xyz", "DEF", "GHI");
  EixoDinamico e1(0, 10, &s, false);
  EixoDinamico e2(0, 10, &s, true);

  return nearlyEqual(e1.getMinimo(), 0) && nearlyEqual(e2.getMinimo(), 0);
}

CASO_DE_TESTE (EixoDinamico$_getMinimo_Serie_com_1_valor) {
  Serie s("xyz", "DEF", "GHI");
  s.adicionar(-1, -1);

  EixoDinamico e1(0, 10, &s, false);
  EixoDinamico e2(0, 10, &s, true);

  return nearlyEqual(e1.getMinimo(), 0) && nearlyEqual(e2.getMinimo(), 0);
}

CASO_DE_TESTE (EixoDinamico$_getMinimo_minimo_e_maximo_iguais) {
  Serie s("xyz", "DEF", "GHI");
  s.adicionar(10, 10);
  s.adicionar(10, 10);
  s.adicionar(10, 10);
  s.adicionar(10, 10);

  EixoDinamico e1(0, 5, &s, false);
  EixoDinamico e2(0, 5, &s, true);

  return nearlyEqual(e1.getMinimo(), 0) && nearlyEqual(e2.getMinimo(), 0);
}


CASO_DE_TESTE (EixoDinamico$_getMinimo_Serie_com_2_valores) {
  Serie s("xyz", "DEF", "GHI");
  s.adicionar(3, 2);
  s.adicionar(1, 6);

  EixoDinamico e1(3, 4, &s, false);
  EixoDinamico e2(2, 7, &s, true);

  return nearlyEqual(e1.getMinimo(), 2) && nearlyEqual(e2.getMinimo(), 1);
}

CASO_DE_TESTE (EixoDinamico$_getMinimo_Serie_com_n_valores) {
  Serie s("xyz", "DEF", "GHI");
  s.adicionar(3, 2);
  s.adicionar(1, 3);
  s.adicionar(0, 4);
  s.adicionar(2, 4.5);
  s.adicionar(4, 6);
  s.adicionar(6, -2);
  s.adicionar(3, 6);

  EixoDinamico e1(-9, 0, &s, false);
  EixoDinamico e2(-10, -2, &s, true);

  return nearlyEqual(e1.getMinimo(), -2) && nearlyEqual(e2.getMinimo(), 0);
}

CASO_DE_TESTE (EixoDinamico$_getMaximo_Serie_vazia) {
  Serie s("xyz", "DEF", "GHI");
  EixoDinamico e1(0, 10, &s, false);
  EixoDinamico e2(0, 10, &s, true);

  return nearlyEqual(e1.getMaximo(), 10) && nearlyEqual(e2.getMaximo(), 10);
}

CASO_DE_TESTE (EixoDinamico$_getMaximo_Serie_com_1_valor) {
  Serie s("xyz", "DEF", "GHI");
  s.adicionar(-1, -1);

  EixoDinamico e1(0, 10, &s, false);
  EixoDinamico e2(0, 10, &s, true);

  return nearlyEqual(e1.getMaximo(), 10) && nearlyEqual(e2.getMaximo(), 10);
}

CASO_DE_TESTE (EixoDinamico$_getMaximo_minimo_e_maximo_iguais) {
  Serie s("xyz", "DEF", "GHI");
  s.adicionar(10, 10);
  s.adicionar(10, 10);
  s.adicionar(10, 10);
  s.adicionar(10, 10);

  EixoDinamico e1(0, 5, &s, false);
  EixoDinamico e2(0, 5, &s, true);

  return nearlyEqual(e1.getMaximo(), 5) && nearlyEqual(e2.getMaximo(), 5);
}


CASO_DE_TESTE (EixoDinamico$_getMaximo_Serie_com_2_valores) {
  Serie s("xyz", "DEF", "GHI");
  s.adicionar(3, 2);
  s.adicionar(1, 6);

  EixoDinamico e1(3, 4, &s, false);
  EixoDinamico e2(2, 7, &s, true);

  return nearlyEqual(e1.getMaximo(), 6) && nearlyEqual(e2.getMaximo(), 3);
}

CASO_DE_TESTE (EixoDinamico$_getMaximo_Serie_com_n_valores) {
  Serie s("xyz", "DEF", "GHI");
  s.adicionar(3, 2);
  s.adicionar(1, 3);
  s.adicionar(0, 4);
  s.adicionar(2, 4.5);
  s.adicionar(4, 6);
  s.adicionar(7, -2);
  s.adicionar(3, 6);

  EixoDinamico e1(-9, 0, &s, false);
  EixoDinamico e2(-10, -2, &s, true);

  return nearlyEqual(e1.getMaximo(), 6) && nearlyEqual(e2.getMaximo(), 7);
}
