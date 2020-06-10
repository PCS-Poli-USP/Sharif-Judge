#include <sstream>
#include <regex>

#include "Teste.h"
#include "SerieTemporal.h"

CASO_DE_TESTE (SerieTemporal$_construtor_e_destrutor) {
  SerieTemporal* s = new SerieTemporal("AAA", "BBB");
  delete s;
  return true;
}

CASO_DE_TESTE(SerieTemporal$_getNome_getNomeDoCanalX_e_getNomeDoCanalY) {
  SerieTemporal s("AAA", "BBB");
  return s.getNome() == "AAA" &&
         s.getNomeDoCanalX() == "Tempo" &&
         s.getNomeDoCanalY() == "BBB";
}

CASO_DE_TESTE(SerieTemporal$_adicionar_com_um_parametro_primeiro_valor) {
  SerieTemporal s("XXX", "YYY");
  s.adicionar(10);

  return s.getQuantidade() == 1 &&
         nearlyEqual(1, s.getPosicao(0)->getX()) &&
         nearlyEqual(10, s.getPosicao(0)->getY());
}

CASO_DE_TESTE(SerieTemporal$_adicionar_com_um_parametro_valores_seguidos) {
  SerieTemporal s("XXX", "YYY");
  s.adicionar(1.1);
  s.adicionar(1.3);
  s.adicionar(1.3);
  s.adicionar(1.2);

  return s.getQuantidade() == 4 &&
         nearlyEqual(1, s.getPosicao(0)->getX()) &&
         nearlyEqual(1.1, s.getPosicao(0)->getY()) &&
         nearlyEqual(2, s.getPosicao(1)->getX()) &&
         nearlyEqual(1.3, s.getPosicao(1)->getY()) &&
         nearlyEqual(3, s.getPosicao(2)->getX()) &&
         nearlyEqual(1.3, s.getPosicao(2)->getY()) &&
         nearlyEqual(4, s.getPosicao(3)->getX()) &&
         nearlyEqual(1.2, s.getPosicao(3)->getY());
}

CASO_DE_TESTE(SerieTemporal$_adicionar_com_dois_parametros_primeiro_valor) {
  SerieTemporal s("XXX", "YYY");
  s.adicionar(2.2, 1.1);

    return s.getQuantidade() == 1 &&
           nearlyEqual(2.2, s.getPosicao(0)->getX()) &&
           nearlyEqual(1.1, s.getPosicao(0)->getY());
}

CASO_DE_TESTE(SerieTemporal$_adicionar_com_dois_parametros_valores_seguidos) {
  SerieTemporal s("XXX", "YYY");
  s.adicionar(2.2, 1.1);
  s.adicionar(2.1, 1.3);
  s.adicionar(2.3, 1.3);
  s.adicionar(2.0, 1.2);

  return s.getQuantidade() == 4 &&
         nearlyEqual(2.2, s.getPosicao(0)->getX()) &&
         nearlyEqual(1.1, s.getPosicao(0)->getY()) &&
         nearlyEqual(2.1, s.getPosicao(1)->getX()) &&
         nearlyEqual(1.3, s.getPosicao(1)->getY()) &&
         nearlyEqual(2.3, s.getPosicao(2)->getX()) &&
         nearlyEqual(1.3, s.getPosicao(2)->getY()) &&
         nearlyEqual(2.0, s.getPosicao(3)->getX()) &&
         nearlyEqual(1.2, s.getPosicao(3)->getY());
}

CASO_DE_TESTE(SerieTemporal$_adicionar_com_dois_parametros_valor_negativo) {
  SerieTemporal s("XXX", "YYY");
  s.adicionar(2.2, 1.1);
  s.adicionar(-1, 1.3);
  s.adicionar(2.3, 1.3);

  return s.getQuantidade() == 2 &&
         nearlyEqual(2.2, s.getPosicao(0)->getX()) &&
         nearlyEqual(1.1, s.getPosicao(0)->getY()) &&
         nearlyEqual(2.3, s.getPosicao(1)->getX()) &&
         nearlyEqual(1.3, s.getPosicao(1)->getY());
}


CASO_DE_TESTE(SerieTemporal$_uso_dos_dois_adicionar_sem_pontos_proximos) {
  SerieTemporal s("XXX", "YYY");
  s.adicionar(1.1);
  s.adicionar(2.1, 1.3);
  s.adicionar(1.3);
  s.adicionar(3.0, 1.2);

  return s.getQuantidade() == 4 &&
         nearlyEqual(1, s.getPosicao(0)->getX()) &&
         nearlyEqual(1.1, s.getPosicao(0)->getY()) &&
         nearlyEqual(2.1, s.getPosicao(1)->getX()) &&
         nearlyEqual(1.3, s.getPosicao(1)->getY()) &&
         nearlyEqual(2, s.getPosicao(2)->getX()) &&
         nearlyEqual(1.3, s.getPosicao(2)->getY()) &&
         nearlyEqual(3, s.getPosicao(3)->getX()) &&
         nearlyEqual(1.2, s.getPosicao(3)->getY());
}

CASO_DE_TESTE(SerieTemporal$_uso_dos_dois_adicionar_com_pontos_proximos_no_de_um_parametro) {
  SerieTemporal s("XXX", "YYY");
  s.adicionar(1.1);
  s.adicionar(2, 1.3);
  s.adicionar(1.5);
  s.adicionar(3.0, 1.2);

  return s.getQuantidade() == 3 &&
         nearlyEqual(1, s.getPosicao(0)->getX()) &&
         nearlyEqual(1.1, s.getPosicao(0)->getY()) &&
         nearlyEqual(2, s.getPosicao(1)->getX()) &&
         nearlyEqual(1.5, s.getPosicao(1)->getY()) &&
         nearlyEqual(3.0, s.getPosicao(2)->getX()) &&
         nearlyEqual(1.2, s.getPosicao(2)->getY());
}

CASO_DE_TESTE(SerieTemporal$_uso_dos_dois_adicionar_com_pontos_proximos_no_de_dois_parametros) {
  SerieTemporal s("XXX", "YYY");
  s.adicionar(1.1);
  s.adicionar(1, 1.3);
  s.adicionar(2.0, 1.2);
  s.adicionar(1.5);

  return s.getQuantidade() == 2 &&
         nearlyEqual(1, s.getPosicao(0)->getX()) &&
         nearlyEqual(1.3, s.getPosicao(0)->getY()) &&
         nearlyEqual(2, s.getPosicao(1)->getX()) &&
         nearlyEqual(1.5, s.getPosicao(1)->getY());
}

// Testes similares ao de uma Serie normal
CASO_DE_TESTE (SerieTemporal$_casos_que_getPosicao_retorna_NULL) {
  // sem pontos
  SerieTemporal s("987", "321");
  Ponto* p = s.getPosicao(0);

  bool resultado = p == NULL;

  // Posicao negativa
  s.adicionar(2);
  s.adicionar(4);
  p = s.getPosicao(-1);

  resultado &= p == NULL;

  // quantidade maior que quantidade no limite
  s.adicionar(9, 6);
  s.adicionar(8);
  p = s.getPosicao(4);

  resultado &= p == NULL;

  // valor alto de posicao
  p = s.getPosicao(10);

  return resultado && p == NULL;
}

CASO_DE_TESTE (SerieTemporal$_getQuantidade) {
  // Sem pontos
  SerieTemporal s("KJL", "BNM");

  bool resultado = s.getQuantidade() == 0;

  // Varios pontos
  s.adicionar(2);
  s.adicionar(2);
  s.adicionar(7, 6);
  s.adicionar(8);

  return resultado && s.getQuantidade() == 4;
}

CASO_DE_TESTE (SerieTemporal$_estaVazia) {
  // Vazio
  SerieTemporal s("QWE", "UIO");

  bool resultado = s.estaVazia();

  // nao esta vazia
  s.adicionar(2);

  return resultado && !s.estaVazia();
}

CASO_DE_TESTE (SerieTemporal$_getLimiteInferior) {
  // Sem pontos
  SerieTemporal* s = new SerieTemporal("ABC", "GHI");
  Ponto* p = s->getLimiteInferior();

  bool resultado = p == NULL;

  // 1 ponto
  s->adicionar(2);
  p = s->getLimiteInferior();

  Ponto* p1 = s->getPosicao(0);

  resultado &= nearlyEqual(p->getX(), 1) && nearlyEqual(p->getY(), 2) && nearlyEqual(p1->getX(), 1) && nearlyEqual(p1->getY(), 2);

  delete s;

  // pontos positivos
  s = new SerieTemporal("ABC", "GHI");
  s->adicionar(3);
  s->adicionar(5, 1);
  s->adicionar(8);
  p = s->getLimiteInferior();

  p1 = s->getPosicao(0);
  Ponto* p2 = s->getPosicao(1);
  Ponto* p3 = s->getPosicao(2);

  resultado &= nearlyEqual(p->getX(), 1) && nearlyEqual(p->getY(), 1) && nearlyEqual(p1->getX(), 1) && nearlyEqual(p1->getY(), 3) &&
         nearlyEqual(p2->getX(), 5) && nearlyEqual(p2->getY(), 1) && nearlyEqual(p3->getX(), 2) && nearlyEqual(p3->getY(), 8);

  delete s;

  // pontos negativos
  s = new SerieTemporal("ABC", "GHI");
  s->adicionar(2, -3);
  s->adicionar(-2);
  s->adicionar(-1);
  p = s->getLimiteInferior();

  p1 = s->getPosicao(0);
  p2 = s->getPosicao(1);

  resultado &= nearlyEqual(p->getX(), 1) && nearlyEqual(p->getY(), -2) && nearlyEqual(p1->getX(), 2) && nearlyEqual(p1->getY(), -1) &&
         nearlyEqual(p2->getX(), 1) && nearlyEqual(p2->getY(), -2);

  delete s;

  // Ultimo ponto e o limite
  s = new SerieTemporal("ABC", "GHI");
  s->adicionar(2, 1);
  s->adicionar(3, 5);
  s->adicionar(1, -1);
  p = s->getLimiteInferior();

  p1 = s->getPosicao(0);
  p2 = s->getPosicao(1);
  p3 = s->getPosicao(2);

  resultado &= nearlyEqual(p->getX(), 1) && nearlyEqual(p->getY(), -1) && nearlyEqual(p1->getX(), 2) && nearlyEqual(p1->getY(), 1) &&
               nearlyEqual(p2->getX(), 3) && nearlyEqual(p2->getY(), 5) && nearlyEqual(p3->getX(), 1) && nearlyEqual(p3->getY(), -1);

  delete s;
  return resultado;
}

CASO_DE_TESTE (SerieTemporal$_getLimiteSuperior) {
  SerieTemporal* s = new SerieTemporal("ABC", "GHI");
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
  s = new SerieTemporal("ABC", "GHI");
  s->adicionar(3);
  s->adicionar(2, 2);
  s->adicionar(5, 1);
  p = s->getLimiteSuperior();

  p1 = s->getPosicao(0);
  p2 = s->getPosicao(1);
  p3 = s->getPosicao(2);

  resultado &= nearlyEqual(p->getX(), 5) && nearlyEqual(p->getY(), 3) && nearlyEqual(p1->getX(), 1) && nearlyEqual(p1->getY(), 3) &&
         nearlyEqual(p2->getX(), 2) && nearlyEqual(p2->getY(), 2) && nearlyEqual(p3->getX(), 5) && nearlyEqual(p3->getY(), 1);
  delete s;

  // pontos negativos
  s = new SerieTemporal("ABC", "GHI");
  s->adicionar(4, -3);
  s->adicionar(-2);
  s->adicionar(2, -1);
  p = s->getLimiteSuperior();

  p1 = s->getPosicao(0);
  p2 = s->getPosicao(1);
  p3 = s->getPosicao(2);

  resultado &= nearlyEqual(p->getX(), 4) && nearlyEqual(p->getY(), -1) && nearlyEqual(p1->getX(), 4) && nearlyEqual(p1->getY(), -3) &&
         nearlyEqual(p2->getX(), 1) && nearlyEqual(p2->getY(), -2) && nearlyEqual(p3->getX(), 2) && nearlyEqual(p3->getY(), -1);

  delete s;

  // ultimo ponto
  s = new SerieTemporal("ABC", "GHI");
  s->adicionar(1);
  s->adicionar(5);
  s->adicionar(10, 8);
  p = s->getLimiteSuperior();

  p1 = s->getPosicao(0);
  p2 = s->getPosicao(1);
  p3 = s->getPosicao(2);

  resultado &= nearlyEqual(p->getX(), 10) && nearlyEqual(p->getY(), 8) && nearlyEqual(p1->getX(), 1) && nearlyEqual(p1->getY(), 1) &&
         nearlyEqual(p2->getX(), 2) && nearlyEqual(p2->getY(), 5) && nearlyEqual(p3->getX(), 10) && nearlyEqual(p3->getY(), 8);

  delete s;
  return resultado;
}
