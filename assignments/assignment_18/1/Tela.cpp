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
