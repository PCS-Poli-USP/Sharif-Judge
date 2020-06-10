===============================================================================
===== Arquivos injetados pelo sistema de corre√ß√£o:
===== * InterfaceSerial.cpp
===== * InterfaceSerial.h
===== * main.cpp
===== * Tela.cpp
===== * Tela.h
===== * Teste.cpp
===== * TesteEP2Ex2Eixo.cpp
===== * TesteEP2Ex2Ponto.cpp
===== * TesteEP2Ex2Serie.cpp
===== * TesteEP2Ex2SerieTemporal.cpp
===== * Teste.h
===============================================================================


===============================================================================
===== 5
===============================================================================



===============================================================================
===== Eixo.cpp
===============================================================================

#include "Eixo.h"

Eixo::Eixo(string titulo, double minimo, double maximo) : titulo(titulo), minimo(minimo), maximo(maximo) {
}

Eixo::Eixo(double minimo, double maximo) : Eixo("", minimo, maximo) {
}

string Eixo::getTitulo() {
  return titulo;
}

double Eixo::getMinimo() {
  return minimo;
}

double Eixo::getMaximo() {
  return maximo;
}

Eixo::~Eixo() {

}


===============================================================================
===== EixoDinamico.cpp
===============================================================================

#include "EixoDinamico.h"

#include <cmath>

EixoDinamico::EixoDinamico(double minimoPadrao, double maximoPadrao,
                           Serie* base, bool orientacaoHorizontal) :
                           Eixo(minimoPadrao, maximoPadrao),
                           base(base), orientacaoHorizontal(orientacaoHorizontal) {
  if (orientacaoHorizontal)
    Eixo::titulo = base->getNomeDoCanalX();
  else Eixo::titulo = base->getNomeDoCanalY();
}

EixoDinamico::~EixoDinamico() {
}


bool EixoDinamico::minimoMaximoIguais() {
  Ponto* maximo = base->getLimiteSuperior();
  Ponto* minimo = base->getLimiteInferior();

  bool igual;
  const double epsilon = 1e-5;
  if (orientacaoHorizontal) {
    igual = std::abs(minimo->getX() - maximo->getX()) <= epsilon;
  } else {
    igual = std::abs(minimo->getY() - maximo->getY()) <= epsilon;
  }
  delete maximo;
  delete minimo;

  return igual;
}


double EixoDinamico::getMinimo() {
  if (base->getQuantidade() < 2 || minimoMaximoIguais())
    return Eixo::getMinimo();

  Ponto* minimo = base->getLimiteInferior();
  double valor;
  if (orientacaoHorizontal) {
    valor = minimo->getX();
  } else {
    valor = minimo->getY();
  }
  delete minimo;
  return valor;
}

double EixoDinamico::getMaximo() {
  if (base->getQuantidade() < 2 || minimoMaximoIguais())
    return Eixo::getMaximo();

  Ponto* maximo = base->getLimiteSuperior();
  double valor;
  if (orientacaoHorizontal) {
    valor = maximo->getX();
  } else {
    valor = maximo->getY();
  }
  delete maximo;
  return valor;
}



===============================================================================
===== EixoDinamico.h
===============================================================================

#ifndef EIXODINAMICO_H
#define EIXODINAMICO_H

#include "Serie.h"
#include "Eixo.h"

class EixoDinamico : public Eixo {
public:
  /**
   * Cria um EixoDinamico informando o minimo e o maximo padrao,
   * a Serie que sera usada como base e a orientacao (true se for
   * horizontal e false se for vertical).
   */
  EixoDinamico(double minimoPadrao, double maximoPadrao,
               Serie* base, bool orientacaoHorizontal);

  virtual double getMinimo();
  virtual double getMaximo();
  virtual ~EixoDinamico();
private:
  bool minimoMaximoIguais();
  Serie* base;
  bool orientacaoHorizontal;
};

#endif // EIXODINAMICO_H


===============================================================================
===== Eixo.h
===============================================================================

#ifndef EIXO_H
#define EIXO_H

#include <string>

using namespace std;

class Eixo {
public:
  /**
   * Cria um Eixo informando o tÌtulo, o mÌnimo e o m·ximo.
   */
  Eixo(string titulo, double minimo, double maximo);
  virtual ~Eixo();
  virtual string getTitulo();
  virtual double getMinimo();
  virtual double getMaximo();
protected: // Uau... Isso e avancado!
  Eixo(double minimo, double maximo);
  string titulo;
private:
  double minimo, maximo;
};

#endif // EIXO_H


===============================================================================
===== Grafico.cpp
===============================================================================

#include "Grafico.h"
#include "Tela.h"

Grafico::Grafico(Eixo* x, Eixo* y, Serie* serie) : x(x), y(y), serie(serie) {
}

Grafico::~Grafico() {
}

Eixo* Grafico::getEixoX() {
  return x;
}

Eixo* Grafico::getEixoY() {
  return y;
}

Serie* Grafico::getSerie() {
  return serie;
}

void Grafico::desenhar() {
  Tela* t = new Tela();
  t->setEixoX(x->getTitulo(), x->getMinimo(), x->getMaximo());
  t->setEixoY(y->getTitulo(), y->getMinimo(), y->getMaximo());

  for (int i = 0; i < serie->getQuantidade(); i++) {
      Ponto* atual = serie->getPosicao(i);
      t->plotar(serie->getNome(), atual->getX(), atual->getY());
  }
  t->mostrar();
  delete t;
}


===============================================================================
===== Grafico.h
===============================================================================

#ifndef GRAFICO_H
#define GRAFICO_H

#include "Eixo.h"
#include "Serie.h"

class Grafico {
public:
  /**
   * Cria um Grafico informando os Eixos e a Serie.
   */
  Grafico(Eixo* x, Eixo* y, Serie* serie);
  virtual ~Grafico();

  Eixo* getEixoX();
  Eixo* getEixoY();
  Serie* getSerie();

  /**
   * Desenha o Grafico na Tela.
   */
  void desenhar();
private:
  Eixo* x;
  Eixo* y;
  Serie* serie;
};

#endif // GRAFICO_H


===============================================================================
===== InterfaceSerial.cpp
===============================================================================

#include "InterfaceSerial.h"

#include <iostream>

using namespace std;

InterfaceSerial::InterfaceSerial(string porta) {
  nomes[0] = "ACCX";
  nomes[1] = "ACCY";
  nomes[2] = "ACCZ";
  nomes[3] = "MAGX";
  nomes[4] = "MAGY";
  nomes[5] = "MAGZ";

  mapa["ACCX"].push_back(-0.0383);
  mapa["ACCX"].push_back(-0.0164);
  mapa["ACCX"].push_back(-0.0125);
  mapa["ACCX"].push_back(-0.2087);
  mapa["ACCX"].push_back(-0.1537);

  mapa["ACCY"].push_back(0.0249);
  mapa["ACCY"].push_back(-0.0073);
  mapa["ACCY"].push_back(-0.0190);
  mapa["ACCY"].push_back(-0.3584);
  mapa["ACCY"].push_back(-0.2100);

  mapa["ACCZ"].push_back(1.0483);
  mapa["ACCZ"].push_back(0.9922);
  mapa["ACCZ"].push_back(1.0107);
  mapa["ACCZ"].push_back(0.3318);
  mapa["ACCZ"].push_back(0.8881);

  mapa["MAGX"].push_back(25.3);
  mapa["MAGX"].push_back(25.4);
  mapa["MAGX"].push_back(25.5);
  mapa["MAGX"].push_back(24.2);
  mapa["MAGX"].push_back(24.6);

  mapa["MAGY"].push_back(-3071.9);
  mapa["MAGY"].push_back(-384.1);
  mapa["MAGY"].push_back(1254.2);
  mapa["MAGY"].push_back(-2434.3);
  mapa["MAGY"].push_back(-3124.5);

  mapa["MAGZ"].push_back(-2655.7);
  mapa["MAGZ"].push_back(-3168.1);
  mapa["MAGZ"].push_back(-1529.6);
  mapa["MAGZ"].push_back(309.3);
  mapa["MAGZ"].push_back(314.2);
}

InterfaceSerial::~InterfaceSerial() {
}

void InterfaceSerial::inicializar() {
  cout << "Aperte o botao reset da placa." << endl;
}

double InterfaceSerial::getValor (string canal) {
  return mapa[canal][numeroDeChamadas];
}

bool InterfaceSerial::atualizar() {
  numeroDeChamadas++;
  return true;
}

string* InterfaceSerial::getNomeDosCanais() {
  return nomes;
}

int InterfaceSerial::getQuantidadeDeCanais() {
  return 6;
}


===============================================================================
===== InterfaceSerial.h
===============================================================================

#ifndef INTERFACESERIAL_H
#define INTERFACESERIAL_H

#define ESPERA 500

#include <vector>
#include <map>

using namespace std;

/**
 * Faz a interface com a placa, usando a porta serial
 *
 * Os dados sao lidos seguindo o seguinte formato:
 * T: NOME1, NOME2, NOME3,... // para os titulos
 * A: VALOR1, VALOR2, VALOR3,... // para os valores
 *
 * Baseado em http://playground.arduino.cc/Interfacing/CPPWindows
 * @Version EP2
 */
class InterfaceSerial {
public:
  /**
   * Cria a comunicacao usando uma porta.
   */
  InterfaceSerial(string porta);
  virtual ~InterfaceSerial();

  /**
   * Inicializa a interface serial.
   */
  void inicializar();

  /**
   * Obtem o valor lido para o canal na ultima atualizacao.
   */
  double getValor (string canal);

  /**
   * Espera por ESPERA ms e atualiza os valores dos canais.
   *
   * Em caso de problema de leitura, retorna false. Caso
   * contrario, retorna true.
   */
  bool atualizar();

  /**
   * Obtem um arranjo com todos os nomes dos canais disponiveis
   */
  string* getNomeDosCanais();

  /**
   * Informa a quantidade de canais disponiveis.
   */
  int getQuantidadeDeCanais();

private:
  int numeroDeChamadas = -1;
  string nomes[6];
  map<string, vector<double>> mapa;
};

#endif // INTERFACESERIAL_H


===============================================================================
===== main.cpp
===============================================================================

#include <iostream>
#include <stdexcept>

#include "Serie.h"
#include "SerieTemporal.h"
#include "Tela.h"
#include "InterfaceSerial.h"
#include "Grafico.h"
#include "Eixo.h"
#include "EixoDinamico.h"

using namespace std;

#define COMM "\\\\.\\COM3"

Eixo* criaEixoEstatico() {
	cout << "Informe o titulo: ";
	string titulo;
	cin >> titulo;

	cout << "Valor minimo: ";
	double minimo;
	cin >> minimo;

	cout << "Valor maximo: ";
	double maximo;
	cin >> maximo;

	return new Eixo (titulo, minimo, maximo);
}

Eixo* criaEixoDinamico (Serie* s, bool horizontal) {
	cout << "Valor minimo padrao: ";
	double minimo;
	cin >> minimo;

	cout << "Valor maximo padrao: ";
	double maximo;
	cin >> maximo;

	return new EixoDinamico (minimo, maximo, s, horizontal);
}

int main() {
	try {
		InterfaceSerial* is = new InterfaceSerial (COMM);

		is->inicializar();

    string nome;
		cout << "Informe o nome da serie: ";
		cin >> nome;

		string* nomes = is->getNomeDosCanais();

		cout << "Escolha o canal X:" << endl;
		cout << "0) Tempo" << endl;

		for (int i = 0 ; i < is->getQuantidadeDeCanais(); i++) {
			cout << i + 1 << ") " << nomes[i] << endl;
		}

		int canalX;
		cin >> canalX;

		cout << "Escolha o canal Y:" << endl;

		for (int i = 0 ; i < is->getQuantidadeDeCanais(); i++) {
			cout << i + 1 << ") " << nomes[i] << endl;
		}

		int canalY;
		cin >> canalY;

		Serie* s;

		if (canalX == 0) {
			s = new SerieTemporal (nome, nomes[canalY - 1]);
		} else s = new Serie (nome, nomes[canalX - 1],
			                      nomes[canalY - 1]);

		int pontos;
		cout << "Obter quantos pontos? ";
		cin >> pontos;

		cout << "Obtendo os pontos" << endl;

		for (int i = 0; i < pontos; i++) {
			is->atualizar();
			if (SerieTemporal* t = dynamic_cast<SerieTemporal*> (s) ) {
				t->adicionar (is->getValor (s->getNomeDoCanalY() ) );
			} else {
				s->adicionar (is->getValor (s->getNomeDoCanalX() ),
				              is->getValor (s->getNomeDoCanalY() ) );
			}
		}

		cout << "Gerando o grafico" << endl;
		string resposta;
    Eixo* x;
    cout << "O eixo X e estatico ou dinamico (e/d): ";
    cin >> resposta;

    if (resposta == "e" || resposta == "E")
      x = criaEixoEstatico();
    else x = criaEixoDinamico (s, true);

    Eixo* y;
    cout << "O eixo Y e' estatico ou dinamico (e/d): ";
    cin >> resposta;

    if (resposta == "e" || resposta == "E")
      y = criaEixoEstatico();
    else y = criaEixoDinamico (s, false);

    Grafico* g = new Grafico (x, y, s);
    g->desenhar();

    delete g;
    delete x;
    delete y;

		delete s;
		delete is;
	} catch (runtime_error* re) {
		cout << re->what() << endl;
		delete re;
	} catch (logic_error* le) {
		cout << le->what() << endl;
		delete le;
	}

	return 0;
}


===============================================================================
===== Ponto.cpp
===============================================================================

#include "Ponto.h"
#include <cmath>
#include <iostream>

using namespace std;

Ponto::Ponto(double x, double y) : x(x), y(y) {
}

Ponto::~Ponto() {
}

bool Ponto::eIgual (Ponto* outro) {
  const double epsilon = 1e-5/* some small number such as 1e-5 */;
  return std::abs(x - outro->getX()) <= epsilon && std::abs(y - outro->getY()) <= epsilon;
}

void Ponto::imprimir() {
  cout << "(" << x << ", " << y << ")" << endl;
}

double Ponto::getX() {
  return x;
}

double Ponto::getY() {
  return y;
}


===============================================================================
===== Ponto.h
===============================================================================

#ifndef PONTO_H
#define PONTO_H

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


===============================================================================
===== Serie.cpp
===============================================================================

#include "Serie.h"

#include <iostream>

using namespace std;

Serie::Serie(string nome, string nomeDoCanalX, string nomeDoCanalY) : nome(nome), nomeDoCanalX(nomeDoCanalX), nomeDoCanalY(nomeDoCanalY) {
}

Serie::~Serie() {
  for (int i = 0; i < quantidade; i++) {
    delete pontos[i];
  }

  delete[] pontos;
}


int Serie::getQuantidade() {
  return quantidade;
}

bool Serie::estaVazia() {
  return quantidade == 0;
}

string Serie::getNome() {
  return nome;
}

string Serie::getNomeDoCanalX() {
  return nomeDoCanalX;

}

string Serie::getNomeDoCanalY() {
  return nomeDoCanalY;
}


void Serie::adicionar (double x, double y) {
  if (quantidade > NUMERO_MAXIMO_VALORES) return;
  pontos[quantidade] = new Ponto(x, y);
  quantidade++;
}

Ponto* Serie::getLimiteSuperior() {
  if (quantidade == 0) return NULL;

  double maiorX = pontos[0]->getX();
  double maiorY = pontos[0]->getY();

  for (int i = 1; i < quantidade; i++) {
      if (maiorX < pontos[i]->getX())
        maiorX = pontos[i]->getX();

      if (maiorY < pontos[i]->getY())
        maiorY = pontos[i]->getY();
  }

  return new Ponto(maiorX, maiorY);
}

Ponto* Serie::getLimiteInferior() {
  if (quantidade == 0) return NULL;

  double menorX = pontos[0]->getX();
  double menorY = pontos[0]->getY();

  for (int i = 1; i < quantidade; i++) {
      if (menorX > pontos[i]->getX())
        menorX = pontos[i]->getX();

      if (menorY > pontos[i]->getY())
        menorY = pontos[i]->getY();
  }

  return new Ponto(menorX, menorY);
}

Ponto* Serie::getPosicao (int posicao) {
  if (posicao >= quantidade || posicao < 0) return nullptr;
  return pontos[posicao];
}

void Serie::imprimir() {
  cout << "Serie " << nome << endl;
  for (int i = 0; i < quantidade; i++) {
    pontos[i]->imprimir();
  }
}


===============================================================================
===== Serie.h
===============================================================================

#ifndef SERIE_H
#define SERIE_H

#define NUMERO_MAXIMO_VALORES 10

#include <string>
#include "Ponto.h"

using namespace std;

class Serie {
public:
  /**
   * Cria uma Serie informando o nome dela e o nome dos canais X e Y.
   */
  Serie(string nome, string nomeDoCanalX, string nomeDoCanalY);
  virtual ~Serie();

  // Permite obter o nome, o nomeDoCanalX e o nomeDoCanalY.
  virtual string getNome();
  virtual string getNomeDoCanalX();
  virtual string getNomeDoCanalY();

  /**
   * Informa a quantidade de pontos que a Serie possui.
   */
  virtual int getQuantidade();

  /**
   * Informa se a Serie esta vazia.
   */
  virtual bool estaVazia();

  /**
   * Adiciona um novo ponto a Serie, informando sua coordenada x e y.
   *
   * Ignora o valor passado caso o NUMERO_MAXIMO_VALORES tenha
   * sido ultrapassado.
   */
  virtual void adicionar(double x, double y);

  /**
   * Obtem um ponto representando o limite superior da Serie.
   * A coordenada x desse ponto deve ser o maximo valor horizontal
   * existente na Serie e a coordenada y deve ser o maximo valor
   * vertical existente na Serie.
   *
   * Caso a Serie nao tenha valores, deve-se retornar NULL.
   *
   * Por exemplo, para a Serie {(2, 3), (5, 1), (1, 2)} o limite
   * superior eh (5, 3).
   */
  virtual Ponto* getLimiteSuperior();

  /**
   * Obtem um ponto representando o limite inferior da Serie.
   * A coordenada x desse ponto deve ser o minimo valor horizontal
   * existente na Serie e a coordenada y deve ser o minimo valor
   * vertical existente na Serie.
   *
   * Caso a serie nao tenha valores, deve-se retornar NULL.
   *
   * Por exemplo, para a Serie {(2, 3), (5, 1), (1, 2)} o limite
   * inferior eh (1, 1).
   */
  virtual Ponto* getLimiteInferior();

  /**
   * Obtem o ponto que esta na posicao definida da Serie. A contagem de
   * posicoes comeca em 0.
   *
   * Em caso de posicoes invalidas, retorne NULL.
   *
   * Por exemplo, para a Serie {(2, 3), (5, 1), (1, 2)}, getPosicao(0)
   * deve retornar (2, 3) e getPosicao(2) deve retornar (1, 2).
   */
  virtual Ponto* getPosicao(int posicao);

  /**
   * Imprime na saida padrao (cout) o nome da Serie e seus pontos
   * seguindo o formato definido.
   */
  virtual void imprimir();

protected:
  int quantidade = 0;
  Ponto** pontos = new Ponto*[NUMERO_MAXIMO_VALORES];
private:
  string nome;
  string nomeDoCanalX, nomeDoCanalY;
};

#endif // SERIE_H


===============================================================================
===== SerieTemporal.cpp
===============================================================================

#include "SerieTemporal.h"
#include <cmath>

SerieTemporal::SerieTemporal(string nome, string nomeDoCanalY) : Serie(nome, "Tempo", nomeDoCanalY) {
}

SerieTemporal::~SerieTemporal() {
}

void SerieTemporal::adicionar(double valor) {
  int quantidadeAntes = quantidade;
  adicionar(tempo, valor);
  if (quantidade > quantidadeAntes) tempo++;
}

void SerieTemporal::adicionar(double x, double y) {
  const double epsilon = 1e-5/* some small number such as 1e-5 */;

  for (int i = 0; i < quantidade; i++) {
    if (std::abs(x - pontos[i]->getX()) <= epsilon) {
      Ponto* novo = new Ponto(x, y);
      delete pontos[i];
      pontos[i] = novo;
      return;
    }
  }

  if (x >= 1) Serie::adicionar(x, y);
}


===============================================================================
===== SerieTemporal.h
===============================================================================

#ifndef SERIETEMPORAL_H
#define SERIETEMPORAL_H

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


===============================================================================
===== Tela.cpp
===============================================================================

#include "Tela.h"
#include <QtCharts/QLineSeries>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>

// Define do Qt para se usar as classes da biblioteca QtCharts
QT_CHARTS_USE_NAMESPACE

const int Tela::LARGURA_PADRAO = 400;
const int Tela::ALTURA_PADRAO = 300;

Tela::Tela() {
}

void Tela::setEixoX(string nome, double minimo, double maximo) {
  minimoX = minimo;
  maximoX = maximo;
  nomeX = nome;
}

void Tela::setEixoY(string nome, double minimo, double maximo) {
  minimoY = minimo;
  maximoY = maximo;
  nomeY = nome;
}

void Tela::plotar(string nomeDaSerie, double x, double y) {
    series[nomeDaSerie].push_back(pair<double, double>(x, y));
  }

void Tela::mostrar(int largura, int altura) {
  // O Qt pede os parametros passados pelo main. Para simplificar o
  // uso da classe Tela, estamos simulando parametros vazios...
  int argc = 1;
  char* argv[1] = {(char*) " "};
  QApplication a(argc, argv);

  // Criando um QChart, o qual permite apresentar diferentes graficos
  QChart* chart = new QChart();

  bool temPontos = false;

  // Criando varias QLineSeries, que representam graficos de linha
  // (se voce chegou ate aqui, esse codigo da uma *dica* do que tera
  // no proximo EP!)
  for (map<string, vector<pair<double, double>>>::iterator i = series.begin(); i != series.end(); i++) {
    QLineSeries* serie = new QLineSeries();
    serie->setPointsVisible(true);
    serie->setPointLabelsClipping(false);
    serie->setPointLabelsVisible(true);
    serie->setName(QString::fromStdString((*i).first));

    // obtendo todos os elementos da serie
    for (vector<pair<double, double>>::iterator j = (*i).second.begin(); j != (*i).second.end(); j++) {
      serie->append((*j).first, (*j).second);
      temPontos = true;
    }

    chart->addSeries(serie);
  }

  // Limpa os pontos adicionados via "plotar"
  series.clear();

  // Cria o grafico com os dados
  QMainWindow window;
  if (temPontos) {
    // Note que o Qt usa uma string diferente, o QString
    chart->createDefaultAxes();
    chart->axisX()->setRange(minimoX, maximoX);
    chart->axisX()->setTitleText(QString::fromStdString(nomeX));
    chart->axisY()->setRange(minimoY, maximoY);
    chart->axisY()->setTitleText(QString::fromStdString(nomeY));

    chart->legend()->setAlignment(Qt::AlignRight);

    // Cria a visualizacao do grafico
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    window.setCentralWidget(chartView);
  } else window.setWindowTitle("Grafico sem pontos");

  window.resize(largura, altura);
  window.show();

  // Essa linha executa a aplicacao, com a janela
  // So existe uma aplicacao por programa
  a.exec();
}

void Tela::mostrar() {
  Tela::mostrar(Tela::LARGURA_PADRAO, Tela::ALTURA_PADRAO);
}

Tela::~Tela() {
}


===============================================================================
===== Tela.h
===============================================================================

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
private:
  map<string, vector<pair<double, double>>> series;
  double minimoX, minimoY, maximoX, maximoY;
  string nomeX, nomeY;
};

#endif // TELA_H


