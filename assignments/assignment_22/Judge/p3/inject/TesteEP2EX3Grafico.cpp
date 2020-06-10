#include <sstream>
#include <regex>

#include "Teste.h"
#include "Tela.h"
#include "Grafico.h"
#include "EixoGabarito.h"

CASO_DE_TESTE (Grafico$_construtor_e_destrutor) {
  EixoGabarito e1("ABC", 0, 2);
  EixoGabarito e2("DEF", 3, 5);
  Serie s("GHI", "XXX", "YYY");

  Grafico* g = new Grafico(&e1, &e2, &s);
  delete g;

  return true;
}

CASO_DE_TESTE (Grafico$_metodos_get) {
  EixoGabarito e1("ABC", 0, 2);
  EixoGabarito e2("DEF", 3, 5);
  Serie s("GHI", "XXX", "YYY");

  Grafico g(&e1, &e2, &s);

  return nearlyEqual(g.getEixoX()->getMaximo(), 2) &&
         nearlyEqual(g.getEixoX()->getMinimo(), 0) &&
         g.getEixoX()->getTitulo() == "ABC" &&

         nearlyEqual(g.getEixoY()->getMaximo(), 5) &&
         nearlyEqual(g.getEixoY()->getMinimo(), 3) &&
         g.getEixoY()->getTitulo() == "DEF" &&

         g.getSerie()->estaVazia() &&
         g.getSerie()->getNome() == "GHI" &&
         g.getSerie()->getNomeDoCanalX() == "XXX" &&
         g.getSerie()->getNomeDoCanalY() == "YYY";
}

CASO_DE_TESTE (Grafico$_mostrar_0_pontos) {
  EixoGabarito e1("ABC", 0, 2);
  EixoGabarito e2("DEF", 3, 5);
  Serie s("GHI", "XXX", "YYY");

  Grafico g(&e1, &e2, &s);

  g.desenhar();
  return Tela::chamou &&
         Tela::nomeX == "ABC" &&
         Tela::nomeY == "DEF" &&
         nearlyEqual(Tela::minimoX, 0) &&
         nearlyEqual(Tela::maximoX, 2) &&
         nearlyEqual(Tela::minimoY, 3) &&
         nearlyEqual(Tela::maximoY, 5) &&
         Tela::series["GHI"].size() == 0;
}

CASO_DE_TESTE (Grafico$_mostrar_1_ponto) {
  EixoGabarito e1("ABC", 0, 2);
  EixoGabarito e2("DEF", 3, 5);
  Serie s("GHI", "XXX", "YYY");
  s.adicionar(4, 3);

  Grafico g(&e1, &e2, &s);

  g.desenhar();
  return Tela::chamou &&
         Tela::nomeX == "ABC" &&
         Tela::nomeY == "DEF" &&
         nearlyEqual(Tela::minimoX, 0) &&
         nearlyEqual(Tela::maximoX, 2) &&
         nearlyEqual(Tela::minimoY, 3) &&
         nearlyEqual(Tela::maximoY, 5) &&
         Tela::series["GHI"].size() == 1 &&
         nearlyEqual(Tela::series["GHI"][0].first, 4) &&
         nearlyEqual(Tela::series["GHI"][0].second, 3);
}

CASO_DE_TESTE (Grafico$_mostrar_2_pontos) {
  EixoGabarito e1("AAA", 0, 2);
  EixoGabarito e2("BBB", 3, 5);
  Serie s("YUI", "XXX", "YYY");
  s.adicionar(4, 3);
  s.adicionar(5, 2);

  Grafico g(&e1, &e2, &s);

  g.desenhar();
  return Tela::chamou &&
         Tela::nomeX == "AAA" &&
         Tela::nomeY == "BBB" &&
         nearlyEqual(Tela::minimoX, 0) &&
         nearlyEqual(Tela::maximoX, 2) &&
         nearlyEqual(Tela::minimoY, 3) &&
         nearlyEqual(Tela::maximoY, 5) &&
         Tela::series["YUI"].size() == 2 &&
         nearlyEqual(Tela::series["YUI"][0].first, 4) &&
         nearlyEqual(Tela::series["YUI"][0].second, 3) &&
         nearlyEqual(Tela::series["YUI"][1].first, 5) &&
         nearlyEqual(Tela::series["YUI"][1].second, 2);
}

CASO_DE_TESTE (Grafico$_mostrar_n_pontos) {
  EixoGabarito e1("AAA", 0, 2);
  EixoGabarito e2("BBB", 3, 5);
  Serie s("CCC", "XXX", "YYY");
  s.adicionar(4, 3);
  s.adicionar(5, 2);
  s.adicionar(3, 3);
  s.adicionar(5, 2);
  s.adicionar(0, 0);

  Grafico g(&e1, &e2, &s);

  g.desenhar();
  return Tela::chamou &&
         Tela::nomeX == "AAA" &&
         Tela::nomeY == "BBB" &&
         nearlyEqual(Tela::minimoX, 0) &&
         nearlyEqual(Tela::maximoX, 2) &&
         nearlyEqual(Tela::minimoY, 3) &&
         nearlyEqual(Tela::maximoY, 5) &&
         Tela::series["CCC"].size() == 5 &&
         nearlyEqual(Tela::series["CCC"][0].first, 4) &&
         nearlyEqual(Tela::series["CCC"][0].second, 3) &&
         nearlyEqual(Tela::series["CCC"][1].first, 5) &&
         nearlyEqual(Tela::series["CCC"][1].second, 2) &&
         nearlyEqual(Tela::series["CCC"][2].first, 3) &&
         nearlyEqual(Tela::series["CCC"][2].second, 3) &&
         nearlyEqual(Tela::series["CCC"][3].first, 5) &&
         nearlyEqual(Tela::series["CCC"][3].second, 2) &&
         nearlyEqual(Tela::series["CCC"][4].first, 0) &&
         nearlyEqual(Tela::series["CCC"][4].second, 0);
}
