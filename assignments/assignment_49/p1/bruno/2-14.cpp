===============================================================================
===== Arquivos injetados pelo sistema de correção:
===== * InterfaceSerial.cpp
===== * InterfaceSerial.h
===== * main.cpp
===== * Tela.cpp
===== * Tela.h
===== * template
===== * Teste.cpp
===== * TesteEP2Ex1Heranca.cpp
===== * Teste.h
===============================================================================


===============================================================================
===== 2
===============================================================================



===============================================================================
===== Eixo.cpp
===============================================================================

#include "Eixo.h"
#include <string>
using namespace std;
#include<iostream>

Eixo::Eixo(string titulo, double minimo, double maximo){
    this->titulo = titulo;
    this->minimo = minimo;
    this->maximo = maximo;
}

Eixo::~Eixo(){
}

string Eixo::getTitulo(){
    return titulo;
}

double Eixo::getMaximo(){
    return maximo;
}

double Eixo::getMinimo(){
    return minimo;
}


===============================================================================
===== EixoDinamico.cpp
===============================================================================

#include "EixoDinamico.h"
using namespace std;
#include<iostream>
#include "Eixo.h"


EixoDinamico::EixoDinamico(double minimoPadrao, double maximoPadrao, Serie* base, bool orientacaoHorizontal) : Eixo("titulo", minimo, maximo){
        this->base = base;
        this->minimoPadrao = minimoPadrao;
        this->maximoPadrao = maximoPadrao;

    if (!base->estaVazia()){

        if (orientacaoHorizontal){
            this->titulo = base->getNomeDoCanalX();
            if (base->getQuantidade() >= 2){
                this->maximo = base->getLimiteSuperior()->getX();
                this->minimo = base->getLimiteInferior()->getX();
            }
            else{
                this->maximo = maximoPadrao;
                this->minimo = minimoPadrao;
            }
        }
        else{
            this->titulo = base->getNomeDoCanalY();
            if (base->getQuantidade() >= 2){
                this->maximo = base->getLimiteSuperior()->getY();
                this->minimo = base->getLimiteInferior()->getY();
            }
            else{
                this->maximo = maximoPadrao;
                this->minimo = minimoPadrao;
            }
        }
    }
}

EixoDinamico::~EixoDinamico(){}


===============================================================================
===== EixoDinamico.h
===============================================================================

#ifndef EIXODINAMICO_H
#define EIXODINAMICO_H
#include "Serie.h"
#include "Eixo.h"


class EixoDinamico : public Eixo{
    public:
        EixoDinamico(double minimoPadrao, double maximoPadrao, Serie* base, bool orientacaoHorizontal);
        virtual ~EixoDinamico();

    private:
        double minimoPadrao;
        double maximoPadrao;
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
#include <iostream>
using namespace std;
class Eixo{
    public:
        Eixo(string titulo, double minimo, double maximo);
        virtual ~Eixo();

        virtual string getTitulo();
        virtual double getMinimo();
        virtual double getMaximo();

    protected:
        double minimo;
        double maximo;
        string titulo;

};

#endif // EIXO_H


===============================================================================
===== Grafico.cpp
===============================================================================

#include "Grafico.h"
#include "Tela.h"

Grafico::Grafico(Eixo* x, Eixo* y, Serie* serie){
    eixoX = x;
    eixoY = y;
    this->serie = serie;
}

Grafico::~Grafico(){
}

Eixo* Grafico::getEixoX(){
    return eixoX;
}

Eixo* Grafico::getEixoY(){
    return eixoY;
}

Serie* Grafico::getSerie(){
    return serie;
}

void Grafico::desenhar(){
    Tela* t = new Tela;

    t->setEixoX(eixoX->getTitulo(), eixoX->getMinimo(), eixoX->getMaximo());
    t->setEixoY(eixoY->getTitulo(), eixoY->getMinimo(), eixoY->getMaximo());

    for (int i = 0; i < serie->getQuantidade(); i++){
        t->plotar(serie->getNome(), serie->pontos[i]->getX(), serie->pontos[i]->getY());
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
class Grafico
{
    public:
        Grafico(Eixo* x, Eixo* y, Serie* serie);
        virtual ~Grafico();

        Eixo* getEixoX();
        Eixo* getEixoY();
        Serie* getSerie();

        void desenhar();
    protected:
        Eixo* eixoX;
        Eixo* eixoY;
        Serie* serie;
    private:
};

#endif // GRAFICO_H


===============================================================================
===== InterfaceSerial.cpp
===============================================================================

#include "InterfaceSerial.h"

// Baseado em http://playground.arduino.cc/Interfacing/CPPWindows

// Parametros da placa
#define BAUD_RATE CBR_9600
#define BYTE_SIZE 8
#define STOP_BITS ONESTOPBIT
#define PARITY NOPARITY

#define WAIT_TIME 500

#define TAMANHO_BUFFER 256
#define HEADER_TITULO 'T'
#define HEADER_DADO 'A'

#include <stdio.h>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <cctype>

using namespace std;

InterfaceSerial::InterfaceSerial(string porta) : porta(porta) {
  canais = NULL;
  quantidadeDeCanais = 0;
}

InterfaceSerial::~InterfaceSerial() {
  desconectar();
  if (canais != NULL) {
      delete[] canais;
      delete[] valores;
  }
}

void InterfaceSerial::conectar() {
  if (conectado) return;

  // Tenta conectar a uma porta
  this->serial = CreateFile (porta.c_str(),
                             GENERIC_READ | GENERIC_WRITE,
                             0,
                             NULL,
                             OPEN_EXISTING,
                             FILE_ATTRIBUTE_NORMAL,
                             NULL);

  // Verifica se conseguiu conectar
  if (this->serial == INVALID_HANDLE_VALUE) {
    // Se nao conseguiu, apresenta um erro
    if (GetLastError() == ERROR_FILE_NOT_FOUND) {
      throw new runtime_error ("ERRO: Porta " + this->porta +
                               " nao disponivel.");
    } else throw new
      runtime_error ("ERRO: nao foi possivel conectar.");
  } else {
    // Se conectado, define os parametros de comunicacao
    DCB dcbSerialParams = {0};

    //Tenta pegar o atual
    if (!GetCommState (this->serial, &dcbSerialParams) ) {
      throw new runtime_error ("ERRO: nao foi possivel pegar os parametros atuais da interface serial.");
    } else {
      // Define os parametros para a placa
      dcbSerialParams.BaudRate = BAUD_RATE;
      dcbSerialParams.ByteSize = BYTE_SIZE;
      dcbSerialParams.StopBits = STOP_BITS;
      dcbSerialParams.Parity = PARITY;

      // (Mantido da comunicacao com o Arduino, mas talvez desnecessario para a placa FRDM-KL25Z)
      // Setting the DTR to Control_Enable ensures that the Arduino is properly
      // reset upon establishing a connection
      dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

      // Define os parametros e verifica sua aplicacao adequada
      if (!SetCommState (serial, &dcbSerialParams) ) {
        throw new runtime_error ("ERRO: nao foi possivel definir os parametros da porta serial");
      } else {
        // Se deu tudo certo, conectou!!
        this->conectado = true;
        // Limpa o buffer
        PurgeComm (this->serial, PURGE_RXCLEAR | PURGE_TXCLEAR);
        // Espera para a placa ficar preparada
        Sleep (WAIT_TIME);
      }
    }
  }
}

int InterfaceSerial::ler (char *buffer,
                          unsigned int numeroDeCaracteres) {
  DWORD bytesLidos;
  unsigned int paraLer;

  // Usa o ClearCommError para pegar o status da porta serial
  ClearCommError (this->serial, &this->errors,
                  &this->status);

  // Verifica se ha algo para ler
  if (this->status.cbInQue > 0) {
    // Se ha, verifica se tem dado suficiente para ler. Se nao, le apenas os
    // caracteres disponiveis para evitar que a aplicacao fique esperando
    if (this->status.cbInQue > numeroDeCaracteres) {
      paraLer = numeroDeCaracteres;
    } else {
      paraLer = this->status.cbInQue;
    }

    // Tenta ler o numero de caracteres e
    // retorna o numero de bytes lidos em caso de sucesso
    if (ReadFile (this->serial, buffer, paraLer, &bytesLidos,
                  NULL) ) {
      return bytesLidos;
    }
  }

  // Se nada foi lido, ou houve um erro, retorna 0
  return 0;

}

void InterfaceSerial::desconectar() {
  if (this->conectado) {
    this->conectado = false;

    // Fecha o handler da serial
    CloseHandle (this->serial);
  }
}


void InterfaceSerial::inicializar() {
  if (conectado) {
    // Desconectado... e limpando
    desconectar();

    if (canais != NULL) {
      delete[] canais;
      quantidadeDeCanais = 0;
    }
  }

  conectar();

  // espera pelo cabecalho
  cout << "Aperte o botao reset da placa." << endl;

  // A: VALOR1, VALOR2, VALOR3,...
  bool encontrouHeader = false;
  char buffer[TAMANHO_BUFFER];

  do {
    int tamanho = ler (buffer, TAMANHO_BUFFER);

    if (tamanho != 0) {
      int posicaoHeader = 0;

      do {
        posicaoHeader = encontraHeader (buffer, posicaoHeader,
                                        tamanho);
      } while (posicaoHeader < tamanho
               && buffer[posicaoHeader] != HEADER_TITULO);

      if (posicaoHeader < tamanho) {
        // Encontrou o Header do titulo
        encontrouHeader = true;

        // Vector sera visto somente na Aula 11!
        vector<string>* lista = new vector<string>();

        int inicio = posicaoHeader + 2; // desprezando o ':'
        int i = inicio;

        for (; i < tamanho && buffer[i] != '\n'; i++) {
          if (buffer[i] == ',' || buffer[i] == '\r') {
            // Separadores de caracteres
            if (inicio != i) {
              // Ha algo
              string canal (buffer + inicio, i - inicio);
              lista->push_back (canal);
              inicio = i + 1;
            } else {
              // So tinha espaco ate entao
              inicio = i + 1;
            }
          } else if (buffer[i] == ' ') {
            // Desprezando
            inicio = i + 1;
          }
        }

        if (buffer[i] != '\n') {
          // O header nao veio completo
          cout << "Erro de leitura. Aperte novamente o botao de reset."
               << endl;
          encontrouHeader = false;
          canais = NULL;
        } else if (lista->size() == 0) {
          // Nao encontrou nada
          canais = NULL;
        } else {
          canais = new string[lista->size()];
          valores = new double[lista->size()]; // ja alocando
          quantidadeDeCanais = 0;

          for (vector<string>::iterator it = lista->begin(); it != lista->end(); it++) {
            canais[quantidadeDeCanais++] = *it;
          }
        }

        delete lista;
      } else {
        Sleep (WAIT_TIME);
      }
    } else {
      Sleep (WAIT_TIME);
    }
  } while (!encontrouHeader);
}

int InterfaceSerial::encontraHeader (char* buffer,
                                     int inicio, int tamanho) {
  int posicaoHeader = inicio;

  while (posicaoHeader < tamanho
         && buffer[posicaoHeader] != '\n') {
    posicaoHeader++;
  }

  // O proximo caracter eh um potencial header
  posicaoHeader++;

  return posicaoHeader;
}

double InterfaceSerial::getValor (string canal) {
  if (!conectado)
    throw new logic_error ("Necessario inicializar a interface serial antes de obter valor");

  for (int i = 0; i < quantidadeDeCanais; i++) {
    if (canais[i] == canal) {
      return valores[i];
    }
  }

  throw new runtime_error("Canal " + canal + " nao encontrado");
}

bool InterfaceSerial::atualizar() {
  if (!conectado)
    throw new logic_error ("Necessario inicializar a interface serial antes de atualizar");

  if (quantidadeDeCanais == 0)
    return false;

  // Esperando: codigo especifico para o windows
  Sleep (ESPERA);

  char buffer[TAMANHO_BUFFER];

  int tamanho = ler (buffer, TAMANHO_BUFFER);

  if (tamanho != 0) {
    int posicaoHeader = 0;

    while (posicaoHeader < tamanho
           && buffer[posicaoHeader] != HEADER_DADO) {
      // Aqui eh diferente pois nao tem os pula linha
      posicaoHeader = encontraHeader (buffer, posicaoHeader,
                                      tamanho);
    }

    if (posicaoHeader < tamanho) {
      // Encontrou o Header do dado
      int canalAtual = 0;

      int inicio = posicaoHeader + 2; // desprezando o ':'

      for (int i = inicio; i < tamanho
           && canalAtual < quantidadeDeCanais
           && buffer[i] != '\n'; i++) {
        if (buffer[i] == ',' || buffer[i] == '\r') {
          if (inicio != i) {
            // Encontrou numero
            string dado (buffer + inicio, i - inicio);

            valores[canalAtual] = atof (dado.c_str() );

            canalAtual++;
            inicio = i + 1;
          } else {
            // So tinha espaco ate entao
            inicio = i + 1;
          }
        } else if (!isdigit (buffer[i]) && buffer[i] != '-'
                   && buffer[i] != '.') {
          // Nao e um caracter valido
          inicio = i + 1;
        }
      }

      return true;
    }
  }

  return false;
}

string* InterfaceSerial::getNomeDosCanais() {
  return canais;
}

int InterfaceSerial::getQuantidadeDeCanais() {
  return quantidadeDeCanais;
}


===============================================================================
===== InterfaceSerial.h
===============================================================================

#ifndef INTERFACESERIAL_H
#define INTERFACESERIAL_H

#define ESPERA 500

#include <string>
#include <windows.h>

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
  // Metodos internos, de apoio
  void conectar();
  void desconectar();
  int ler (char *buffer, unsigned int numeroDeCaracteres);
  int encontraHeader (char* buffer, int inicio, int tamanho);

  string* canais = NULL;
  double* valores;
  int quantidadeDeCanais = 0;
  string porta;
  bool conectado = false;

  // Variaveis para comunicacao em baixo nivel - Windows
  HANDLE serial;
  COMSTAT status;
  DWORD errors;
};

#endif // INTERFACESERIAL_H


===============================================================================
===== main.cpp
===============================================================================

#include <iostream>

#include "Serie.h"
#include "Ponto.h"
#include "InterfaceSerial.h"
#include "Eixo.h"
#include "Grafico.h"
#include "SerieTemporal.h"
#include "EixoDinamico.h"

#define COMM "\\\\.\\COM3"
#include<string>

#define NUMERO_MAXIMO_VALORES 10

using namespace std;

int main() {
  InterfaceSerial* is = new InterfaceSerial();
  is->inicializar(COMM);

  string nome;
  string* canais = is->getNomeDosCanais();
  string canalX,
         canalY,
         titulo;

  int numeroDeCanais,
      numeroDoCanalX,
      numeroDoCanalY;
  double minimo,
         maximo,
         minimoPadrao,
         maximoPadrao;
  char tipo;
  bool horizontal;

  cout << "Aperte o botao reset da placa" << endl;
  cout << "Informe o nome da serie: " ;
  cin >> nome;
  cout << "Escolha o canal X: " << endl;
  for (numeroDeCanais = 1; numeroDeCanais <= is->getQuantidadeDeCanais(); numeroDeCanais++){
    cout << numeroDeCanais << ") " << canais[numeroDeCanais - 1] << endl;
  }
  cin >> numeroDoCanalX;
  canalX  = canais[numeroDoCanalX - 1];

  cout << "Escolha o canal Y: " << endl;
  for (numeroDeCanais = 1; numeroDeCanais <= is->getQuantidadeDeCanais(); numeroDeCanais++){
    cout << numeroDeCanais << ") " << canais[numeroDeCanais-1] << endl;
  }
  cin >> numeroDoCanalY;
  canalY = canais[numeroDoCanalY - 1];


  // Obtem o numero de Pontos a adicionar
  int quantidade;
  cout << "Obter quantos pontos? ";
  cin >> quantidade;
  cout << endl;

  // Cria a Serie
  SerieTemporal *s = new SerieTemporal (nome, canalY);

  cout << "Obtendo os pontos" << endl;
  for (int i = 0; i < quantidade; i++){
    is->atualizar();
    s->adicionar(is->getValor(canalY));
  }
  s->imprimir();
  s->getLimiteInferior()->imprimir();
  s->getLimiteSuperior()->imprimir();
  cout << "Gerando grafico" << endl;
  cout << "O eixo X e estatico ou dinamico (e/d): " << endl;
  cin >> tipo;
  if (tipo == 'e' || tipo == 'E'){
    horizontal = true;
    cout << "Informe o titulo: ";
    cin >> titulo;
    cout << endl;
    cout << "valor minimo: ";
    cin >> minimo;
    cout << endl;
    cout << "valor maximo: ";
    cin >> maximo;
    cout << endl;
    Eixo* eixoX = new Eixo(titulo, minimo, maximo);
    cout << "O eixo Y e estatico ou dinamico (e/d): " << endl;
    cin >> tipo;
    if (tipo == 'e' || tipo == 'E'){
        horizontal = false;
        cout << "Informe o titulo: ";
        cin >> titulo;
        cout << endl;
        cout << "valor minimo: ";
        cin >> minimo;
        cout << endl;
        cout << "valor maximo: ";
        cin >> maximo;
        cout << endl;
        Eixo* eixoY = new Eixo(titulo, minimo, maximo);
        Grafico* g = new Grafico(eixoX, eixoY, s);
        g->desenhar();
        g->~Grafico();
    }
    else if (tipo == 'd' || tipo == 'D'){
        horizontal = false;
        cout << "Vamor minimo padrao: ";
        cin >> minimoPadrao;
        cout << endl;
        cout << "Valor maximo padrao: ";
        cin >> maximoPadrao;
        cout << endl;
        Eixo* eixoDinamicoY = new EixoDinamico(minimoPadrao, maximoPadrao, s, horizontal);
        Grafico* g = new Grafico(eixoX, eixoDinamicoY, s);
        g->desenhar();
        g->~Grafico();
    }

  }
  else if (tipo == 'd' || tipo == 'D'){
    horizontal = true;
    cout << "Vamor minimo padrao: ";
    cin >> minimoPadrao;
    cout << endl;
    cout << "Valor maximo padrao: ";
    cin >> maximoPadrao;
    cout << endl;
    Eixo* eixoDinamicoX = new EixoDinamico(minimoPadrao, maximoPadrao, s, horizontal);
    cout << "O eixo Y e estatico ou dinamico (e/d): " << endl;
    cin >> tipo;
    if (tipo == 'e' || tipo == 'E'){
        horizontal = false;
        cout << "Informe o titulo";
        cin >> titulo;
        cout << endl;
        cout << "valor minimo: ";
        cin >> minimo;
        cout << endl;
        cout << "valor maximo: ";
        cin >> maximo;
        cout << endl;
        Eixo* eixoY = new Eixo(titulo, minimo, maximo);
        Grafico* g = new Grafico(eixoDinamicoX, eixoY, s);
        g->desenhar();
        g->~Grafico();
    }
    else if (tipo == 'd' || tipo == 'D'){
        horizontal = false;
        cout << "Vamor minimo padrao: ";
        cin >> minimoPadrao;
        cout << endl;
        cout << "Valor maximo padrao: ";
        cin >> maximoPadrao;
        cout << endl;
        Eixo* eixoDinamicoY = new EixoDinamico(minimoPadrao, maximoPadrao, s, horizontal);
        Grafico* g = new Grafico(eixoDinamicoX, eixoDinamicoY, s);
        g->desenhar();
        g->~Grafico();
    }
  }

  s->~Serie();


  return 0;
}



===============================================================================
===== Ponto.cpp
===============================================================================

#include "Ponto.h"
using namespace std;
#include<iostream>

Ponto::Ponto(double x, double y){
    Xponto = x;
    Yponto = y;
}

Ponto::~Ponto(){

}

double Ponto::getX(){
    return Xponto;
}

double Ponto::getY(){
    return Yponto;
}

void Ponto::imprimir(){
    cout << "(" << Xponto << ", " << Yponto << ")" << endl;
}

bool Ponto::eIgual(Ponto *p){
    if ( (Xponto - p->getX() <= Erro && Xponto - p->getX() > - Erro ) && (Yponto - p->getY() <= Erro && Yponto - p->getY() > - Erro  )){
        return true;
    }
    else
        return false;
}



===============================================================================
===== Ponto.h
===============================================================================

#ifndef PONTO_H
#define PONTO_H

class Ponto {
public:
    double Xponto,
           Yponto,
           Erro = 1e-5;
    bool igual = false;

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
   */
  bool eIgual(Ponto* outro);

  // ADICIONE OS ATRIBUTOS NECESSARIOS
};

#endif // PONTO_H


===============================================================================
===== Serie.cpp
===============================================================================

#include "Serie.h"
#include "Ponto.h"
#include<iostream>
using namespace std;

Serie::Serie(string nome, string nomeDoCanalX, string nomeDoCanalY){
    this->nome = nome;
    this->nomeDoCanalX = nomeDoCanalX;
    this->nomeDoCanalY = nomeDoCanalY;
}

Serie::~Serie(){
}

string Serie::getNome(){
    return nome;
}

string Serie::getNomeDoCanalX(){
    return nomeDoCanalX;
}

string Serie::getNomeDoCanalY(){
    return nomeDoCanalY;
}

int Serie::getQuantidade(){
    return quantidade;
}

bool Serie::estaVazia(){

    if (quantidade == 0)
        vazia = true;
    else
        vazia = false;

    return vazia;
}

void Serie::adicionar(double x, double y){

    pontos[quantidade] = new Ponto(x, y);

    quantidade += 1;

}

Ponto* Serie::getPosicao(int posicao){

    if (posicao < NUMERO_MAXIMO_VALORES)
        return pontos[posicao - 1];
    else
        return NULL;
}

Ponto* Serie::getLimiteSuperior(){
    int i;
    double xMaximo, yMaximo;
    xMaximo = pontos[0]->getX();
    yMaximo = pontos[0]->getY();


    if(vazia == false){
        for (i = 1; i < quantidade; i++){
            if (pontos[i]->getX() > xMaximo)
               xMaximo = pontos[i]->getX();
            if (pontos[i]->getY() > yMaximo)
                yMaximo = pontos[i]->getY();
        }

        Ponto *psup = new Ponto(xMaximo, yMaximo);

        return psup;

    }
    return NULL;
}

Ponto* Serie::getLimiteInferior(){
    int i;
    double xMinimo, yMinimo;
    xMinimo = pontos[0]->getX();
    yMinimo = pontos[0]->getY();

    if(vazia == false){
        for (i = 1; i < quantidade; i++){
            if (pontos[i]->getX() < xMinimo)
               xMinimo = pontos[i]->getX();
            if (pontos[i]->getY() < yMinimo)
                yMinimo = pontos[i]->getY();
        }

        Ponto *pinf = new Ponto(xMinimo, yMinimo);

        return pinf;
    }
    return NULL;
}

void Serie::imprimir(){
    int i;
    cout << "Serie " << nome << endl;
    for (i = 0; i < quantidade; i++)
        pontos[i]->imprimir();
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
  string nome,
         nomeDoCanalX,
         nomeDoCanalY;

  int quantidade = 0;

  bool vazia = false;

  Ponto* pontos[NUMERO_MAXIMO_VALORES];


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
   * existente na Serie e a coordenpontos[0]->getX()ada y deve ser o minimo valor
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

  // ADICIONE OS ATRIBUTOS NECESSARIOS
};

#endif // SERIE_H


===============================================================================
===== SerieTemporal.cpp
===============================================================================

#include "SerieTemporal.h"
#include "Serie.h"
using namespace std;
#include <iostream>

#define NUMERO_MAXIMO_VALORES 10
SerieTemporal::SerieTemporal(string nome, string nomeDoCanalY) : Serie(nome, "tempo", nomeDoCanalY){}

SerieTemporal::~SerieTemporal(){
}

void SerieTemporal::adicionar(double valor){
    if(tempo >= 1 && quantidade < NUMERO_MAXIMO_VALORES){
            pontos[quantidade] = new Ponto (tempo, valor);
            tempo = tempo + 1;
            quantidade = quantidade + 1;
    }
}

void SerieTemporal::adicionar(double x, double y){
    if (x >= 1 && quantidade < NUMERO_MAXIMO_VALORES){
        for (int i = 0; i < quantidade; i++){
            if ((pontos[i]->getX() - x < 1e-5) && (pontos[i]->getX() - x > -1e-5)){
                pontos[i] = new Ponto (x, y);
                tempo = x;
                return;
            }
        }
        pontos[quantidade] = new Ponto (x, y);
        tempo = x;
        quantidade += 1;
    }
}


===============================================================================
===== SerieTemporal.h
===============================================================================

#ifndef SERIETEMPORAL_H
#define SERIETEMPORAL_H

#include "Serie.h"


class SerieTemporal : public Serie{
    public:
        SerieTemporal(string nome, string nomeDoCanalY);
        virtual ~SerieTemporal();

        virtual void adicionar(double valor);

        virtual void adicionar(double x, double y);

    protected:
        double tempo = 1;

    private:
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


class Tela {
public:
  static const int LARGURA_PADRAO;
  static const int ALTURA_PADRAO;

  Tela();
  virtual ~Tela();
  virtual void setEixoX(string nome, double minimo, double maximo);
  virtual void setEixoY(string nome, double minimo, double maximo);
  virtual void plotar(string nomeDaSerie, double x, double y);
  virtual void mostrar(int largura, int altura);
  virtual void mostrar();
private:
  map<string, vector<pair<double, double> > > series;
  double minimoX, minimoY, maximoX, maximoY;
  string nomeX, nomeY;
};

#endif // TELA_H


