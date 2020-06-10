#ifndef TELA_H
#define TELA_H

#include <string>
#include <map>
#include <vector>

using namespace std;

/**
 * Classe Tela usando a biblioteca Qt.
 * NAO EH PRECISO ALTERAR ESSA CLASSE. SO ALTERE SE VOCE QUISER
 * FAZER ALGUM TESTE.
 */
class Tela {
public:
  static const int LARGURA_PADRAO;
  static const int ALTURA_PADRAO;

  Tela();
  virtual ~Tela();

  /**
   * Define o eixo X.
   */
  virtual void setEixoX(string nome, double minimo, double maximo);

  /**
   * Define o eixo Y.
   */
  virtual void setEixoY(string nome, double minimo, double maximo);

  /**
   * Plota um ponto (x, y) para uma Serie com o nome informado.
   */
  virtual void plotar(string nomeDaSerie, double x, double y);

  /**
   * Mostra o grafico, criando uma janela com largura e altura
   * informadas. O metodo retorna apenas quando a janela eh fechada.
   */
  virtual void mostrar(int largura, int altura);

  /**
   * Mostra o grafico, criando uma janela com largura e altura padrao.
   * O metodo retorna apenas quando a janela eh fechada.
   */
  virtual void mostrar();

  // Para teste
public:
  static map<string, vector<pair<double, double>>> series;
  static double minimoX, minimoY, maximoX, maximoY;
  static string nomeX, nomeY;
  static bool chamou;
};

#endif // TELA_H
