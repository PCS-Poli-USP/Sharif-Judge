#include <sstream>
#include <regex>

#include "Teste.h"
#include "Ponto.h"

CASO_DE_TESTE (Ponto$_construtor_destrutor_getX_getY) {
  Ponto* p = new Ponto(2.1, 1.7);

  bool resultado = nearlyEqual(p->getX(), 2.1) && nearlyEqual(p->getY(), 1.7);
  delete p;

  return resultado;
}

CASO_DE_TESTE (Ponto$_eIgual_com_valores_iguais) {
  Ponto* p1 = new Ponto(1.6, 1.8);
  Ponto* p2 = new Ponto(1.6, 1.8);
  Ponto* p3 = new Ponto(-0.8, -0.4);
  Ponto* p4 = new Ponto(-0.8, -0.4);

  bool resultado = p1->eIgual(p2) && p2->eIgual(p1) &&
         p3->eIgual(p4) && p4->eIgual(p3);

  delete p1;
  delete p2;
  delete p3;
  delete p4;
  return resultado;
}

CASO_DE_TESTE (Ponto$_eIgual_com_valores_diferentes) {
  bool resultado = false;
  {
    Ponto p1(-0.8, 0.4), p2(-0.8, 0.6);
    Ponto p3(-0.8, 0.4), p4(-0.6, 0.4);

    resultado = !p1.eIgual(&p2) && !p2.eIgual(&p1) &&
           !p3.eIgual(&p4) && !p4.eIgual(&p3);
  }

  // x e y diferentes
  {

    Ponto p1(0, 4), p2(3, 2);

    resultado &= !p1.eIgual(&p2) && !p2.eIgual(&p1);
  }

  return resultado;
}

CASO_DE_TESTE (Ponto$_eIgual_com_o_mesmo_objeto) {
  Ponto p1(-1.1, -1.1);

  return p1.eIgual(&p1) && p1.eIgual(&p1);
}

CASO_DE_TESTE (Ponto$_imprimir) {
  stringstream buffer;
  streambuf* old = cout.rdbuf (buffer.rdbuf());

  regex formato("\\(\\s*1(\\.\\d*)?\\s*\\,\\s*2(\\.\\d*)?\\s*\\)\\s*\\r?\\n");

  Ponto p1(1, 2);
  p1.imprimir();

  bool result = buffer && regex_match(buffer.str(), formato);

  stringstream outroBuffer;
  cout.rdbuf (outroBuffer.rdbuf());

  formato = regex("\\(\\s*1(\\.\\d*)?\\s*\\,\\s*2(\\.\\d*)?\\s*\\)\\s*\\r?\\n\\(\\s*3(\\.\\d*)?\\s*\\,\\s*5(\\.\\d*)?\\s*\\)\\s*\\r?\\n");

  Ponto p2(3, 5);
  p1.imprimir();
  p2.imprimir();

  result &= outroBuffer && regex_match(outroBuffer.str(), formato);
  std::cout.rdbuf (old);

  return result;
}
