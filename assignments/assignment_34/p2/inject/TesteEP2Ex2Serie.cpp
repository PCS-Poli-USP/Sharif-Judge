#include <sstream>
#include <regex>

#include "Teste.h"
#include "Ponto.h"
#include "Serie.h"

CASO_DE_TESTE (Serie$_construtor_destrutor_e_getNomes) {
  Serie *s = new Serie("ABC", "DEF", "GHI");
  bool resultado = s->getNome() == "ABC" &&
         s->getNomeDoCanalX() == "DEF" &&
         s->getNomeDoCanalY() == "GHI";
  delete s;
  return resultado;
}

CASO_DE_TESTE (Serie$_adicionar_e_getPosicao_com_posicoes_validas) {
  Serie s1("123", "465", "789");
  s1.adicionar(1, 2);
  Ponto* p1 = s1.getPosicao(0);

  bool resultado = nearlyEqual(p1->getX(), 1) && nearlyEqual(p1->getY(), 2);

  // 2 pontos
  s1.adicionar(3, 4);
  p1 = s1.getPosicao(0);
  Ponto* p2 = s1.getPosicao(1);

  resultado &= nearlyEqual(p1->getX(), 1) && nearlyEqual(p1->getY(), 2) && nearlyEqual(p2->getX(), 3) && nearlyEqual(p2->getY(), 4);

  // 3 pontos
  s1.adicionar(5, 6);
  p1 = s1.getPosicao(0);
  p2 = s1.getPosicao(1);
  Ponto* p3 = s1.getPosicao(2);

  return resultado && nearlyEqual(p1->getX(), 1) && nearlyEqual(p1->getY(), 2) && nearlyEqual(p2->getX(), 3) && nearlyEqual(p2->getY(), 4) && nearlyEqual(p3->getX(), 5) && nearlyEqual(p3->getY(), 6);
}

CASO_DE_TESTE (Serie$_casos_que_getPosicao_retorna_NULL) {
  // sem pontos
  Serie s("987", "654", "321");
  Ponto* p = s.getPosicao(0);

  bool resultado = p == NULL;

  // Posicao negativa
  s.adicionar(1, 2);
  s.adicionar(3, 4);
  p = s.getPosicao(-1);

  resultado &= p == NULL;

  // quantidade maior que quantidade no limite
  s.adicionar(5, 6);
  s.adicionar(7, 8);
  p = s.getPosicao(4);

  resultado &= p == NULL;

  // valor alto de posicao
  p = s.getPosicao(10);

  return resultado && p == NULL;
}

CASO_DE_TESTE (Serie$_getQuantidade) {
  // Sem pontos
  Serie s("KJL", "IOP", "BNM");

  bool resultado = s.getQuantidade() == 0;

  // Varios pontos
  s.adicionar(1, 2);
  s.adicionar(3, 4);
  s.adicionar(5, 6);
  s.adicionar(7, 8);

  return resultado && s.getQuantidade() == 4;
}

CASO_DE_TESTE (Serie$_estaVazia) {
  // Vazio
  Serie s("QWE", "RYT", "UIO");

  bool resultado = s.estaVazia();

  // nao esta vazia
  s.adicionar(1, 2);

  return resultado && !s.estaVazia();
}

CASO_DE_TESTE (Serie$_getLimiteInferior) {
  // Sem pontos
  Serie* s = new Serie("ABC", "DEF", "GHI");
  Ponto* p = s->getLimiteInferior();

  bool resultado = p == NULL;

  // 1 ponto
  s->adicionar(1, 2);
  p = s->getLimiteInferior();

  Ponto* p1 = s->getPosicao(0);

  resultado &= nearlyEqual(p->getX(), 1) && nearlyEqual(p->getY(), 2) && nearlyEqual(p1->getX(), 1) && nearlyEqual(p1->getY(), 2);

  delete s;

  // pontos positivos
  s = new Serie("ABC", "DEF", "GHI");
  s->adicionar(2, 3);
  s->adicionar(1, 2);
  s->adicionar(5, 1);
  p = s->getLimiteInferior();

  p1 = s->getPosicao(0);
  Ponto* p2 = s->getPosicao(1);
  Ponto* p3 = s->getPosicao(2);

  resultado &= nearlyEqual(p->getX(), 1) && nearlyEqual(p->getY(), 1) && nearlyEqual(p1->getX(), 2) && nearlyEqual(p1->getY(), 3) &&
         nearlyEqual(p2->getX(), 1) && nearlyEqual(p2->getY(), 2) && nearlyEqual(p3->getX(), 5) && nearlyEqual(p3->getY(), 1);

  delete s;

  // pontos negativos
  s = new Serie("ABC", "DEF", "GHI");
  s->adicionar(-2, -3);
  s->adicionar(-1, -2);
  s->adicionar(-5, -1);
  p = s->getLimiteInferior();

  p1 = s->getPosicao(0);
  p2 = s->getPosicao(1);
  p3 = s->getPosicao(2);

  resultado &= nearlyEqual(p->getX(), -5) && nearlyEqual(p->getY(), -3) && nearlyEqual(p1->getX(), -2) && nearlyEqual(p1->getY(), -3) &&
         nearlyEqual(p2->getX(), -1) && nearlyEqual(p2->getY(), -2) && nearlyEqual(p3->getX(), -5) && nearlyEqual(p3->getY(), -1);

  delete s;

  // Ultimo ponto e o limite
  s = new Serie("ABC", "DEF", "GHI");
  s->adicionar(2, 1);
  s->adicionar(1, 5);
  s->adicionar(-5, -1);
  p = s->getLimiteInferior();

  p1 = s->getPosicao(0);
  p2 = s->getPosicao(1);
  p3 = s->getPosicao(2);

  resultado &= nearlyEqual(p->getX(), -5) && nearlyEqual(p->getY(), -1) && nearlyEqual(p1->getX(), 2) && nearlyEqual(p1->getY(), 1) &&
               nearlyEqual(p2->getX(), 1) && nearlyEqual(p2->getY(), 5) && nearlyEqual(p3->getX(), -5) && nearlyEqual(p3->getY(), -1);

  delete s;
  return resultado;
}

CASO_DE_TESTE (Serie$_getLimiteSuperior) {
  Serie* s = new Serie("ABC", "DEF", "GHI");
  Ponto *p, *p1, *p2, *p3;

  p = s->getLimiteSuperior();

  bool resultado = p == NULL;

  // 1 ponto
  s->adicionar(1, 2);
  p = s->getLimiteSuperior();

  p1 = s->getPosicao(0);

  resultado &= nearlyEqual(p->getX(), 1) && nearlyEqual(p->getY(), 2) && nearlyEqual(p1->getX(), 1) && nearlyEqual(p1->getY(), 2);

  delete s;

  // pontos positivos
  s = new Serie("ABC", "DEF", "GHI");
  s->adicionar(2, 3);
  s->adicionar(1, 2);
  s->adicionar(5, 1);
  p = s->getLimiteSuperior();

  p1 = s->getPosicao(0);
  p2 = s->getPosicao(1);
  p3 = s->getPosicao(2);

  resultado &= nearlyEqual(p->getX(), 5) && nearlyEqual(p->getY(), 3) && nearlyEqual(p1->getX(), 2) && nearlyEqual(p1->getY(), 3) &&
         nearlyEqual(p2->getX(), 1) && nearlyEqual(p2->getY(), 2) && nearlyEqual(p3->getX(), 5) && nearlyEqual(p3->getY(), 1);
  delete s;

  // pontos negativos
  s = new Serie("ABC", "DEF", "GHI");
  s->adicionar(-2, -3);
  s->adicionar(-1, -2);
  s->adicionar(-5, -1);
  p = s->getLimiteSuperior();

  p1 = s->getPosicao(0);
  p2 = s->getPosicao(1);
  p3 = s->getPosicao(2);

  resultado &= nearlyEqual(p->getX(), -1) && nearlyEqual(p->getY(), -1) && nearlyEqual(p1->getX(), -2) && nearlyEqual(p1->getY(), -3) &&
         nearlyEqual(p2->getX(), -1) && nearlyEqual(p2->getY(), -2) && nearlyEqual(p3->getX(), -5) && nearlyEqual(p3->getY(), -1);

  delete s;

  // ultimo ponto
  s = new Serie("ABC", "DEF", "GHI");
  s->adicionar(2, 1);
  s->adicionar(1, 5);
  s->adicionar(10, 8);
  p = s->getLimiteSuperior();

  p1 = s->getPosicao(0);
  p2 = s->getPosicao(1);
  p3 = s->getPosicao(2);

  resultado &= nearlyEqual(p->getX(), 10) && nearlyEqual(p->getY(), 8) && nearlyEqual(p1->getX(), 2) && nearlyEqual(p1->getY(), 1) &&
         nearlyEqual(p2->getX(), 1) && nearlyEqual(p2->getY(), 5) && nearlyEqual(p3->getX(), 10) && nearlyEqual(p3->getY(), 8);

  delete s;
  return resultado;
}

CASO_DE_TESTE (Serie$_imprimir) {
  stringstream buffer;
  streambuf* old = cout.rdbuf (buffer.rdbuf());

  regex formato("Serie\\s*Teste\\s*\\r?\\n"
                "\\(\\s*7(\\.\\d*)?\\s*\\,\\s*5(\\.\\d*)?\\s*\\)\\s*\\r?\\n");

  Serie s("Teste", "B", "C");
  s.adicionar(7, 5);

  s.imprimir();

  bool result = buffer && regex_match(buffer.str(), formato);

  // 4 pontos
  stringstream novoBuffer;
  cout.rdbuf (novoBuffer.rdbuf());

  formato = regex("Serie\\s*Teste2\\s*\\r?\\n"
                "\\(\\s*7(\\.\\d*)?\\s*\\,\\s*5(\\.\\d*)?\\s*\\)\\s*\\r?\\n"
                "\\(\\s*7(\\.\\d*)?\\s*\\,\\s*5(\\.\\d*)?\\s*\\)\\s*\\r?\\n"
                "\\(\\s*1(\\.\\d*)?\\s*\\,\\s*2(\\.\\d*)?\\s*\\)\\s*\\r?\\n"
                "\\(\\s*3(\\.\\d*)?\\s*\\,\\s*8(\\.\\d*)?\\s*\\)\\s*\\r?\\n");

  Serie s2("Teste2", "A", "B");
  s2.adicionar(7, 5);
  s2.adicionar(7, 5);
  s2.adicionar(1, 2);
  s2.adicionar(3, 8);

  s2.imprimir();

  result &= novoBuffer && regex_match(novoBuffer.str(), formato);

  std::cout.rdbuf (old);

  return result;
}
