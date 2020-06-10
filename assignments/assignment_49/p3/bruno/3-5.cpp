===============================================================================
===== Arquivos injetados pelo sistema de correÃ§Ã£o:
===== * EixoGabarito.cpp
===== * EixoGabarito.h
===== * InterfaceSerial.cpp
===== * InterfaceSerial.h
===== * main.cpp
===== * Ponto.cpp
===== * Ponto.h
===== * Serie.cpp
===== * Serie.h
===== * SerieTemporal.cpp
===== * SerieTemporal.h
===== * Tela.cpp
===== * Tela.h
===== * Teste.cpp
===== * TesteEP2EX3EixoDinamico.cpp
===== * TesteEP2EX3Grafico.cpp
===== * Teste.h
===============================================================================


===============================================================================
===== 3
===============================================================================



===============================================================================
===== Eixo.cpp
===============================================================================

#include "Eixo.h"
#include <iostream>
using namespace std;
Eixo::Eixo(string titulo, double minimo, double maximo){
    this->titulo=titulo;
    this->maximo=maximo;
    this->minimo=minimo;
}

Eixo::~Eixo(){

}

string Eixo::getTitulo(){
    return titulo;
}

double Eixo::getMinimo(){
    return minimo;
}

double Eixo::getMaximo(){
    return maximo;
}


===============================================================================
===== EixoDinamico.cpp
===============================================================================

#include "EixoDinamico.h"
#include "Serie.h"
#define e 0.00001
#include <cmath>
EixoDinamico::EixoDinamico(double minimoPadrao, double maximoPadrao,Serie*base, bool orientacaoHorizontal)
    : Eixo (orientacaoHorizontal ? base->getNomeDoCanalX() : base->getNomeDoCanalY(), minimoPadrao, maximoPadrao)
{
    int maximo;
    int minimo;
    bool passou = false;
    this -> orientacaoHorizontal = orientacaoHorizontal;
     if (base-> getQuantidade()<= 2){
        this -> maximoPadrao = maximoPadrao;
        this -> minimoPadrao = minimoPadrao;
        passou = true;
    }
    if (passou == false){
        if (orientacaoHorizontal){
            maximo = base -> getLimiteSuperior()->getX();
            minimo = base -> getLimiteInferior()-> getX();
        }
        else{
            maximo = base -> getLimiteSuperior()->getY();
            minimo = base -> getLimiteInferior()-> getY();
        }
        this -> maximoPadrao = maximo;
        this -> minimoPadrao = minimo;
        if (abs(maximo-minimo)< e){
            this -> maximoPadrao = maximoPadrao;
            this -> minimoPadrao = minimoPadrao;
        }
    }
}

EixoDinamico::~EixoDinamico(){

}


===============================================================================
===== EixoDinamico.h
===============================================================================

#ifndef EIXODINAMICO_H
#define EIXODINAMICO_H
#include "Serie.h"
#include "Eixo.h"

class EixoDinamico : public Eixo
{
    public:
        EixoDinamico(double minimoPadrao, double maximoPadrao, Serie*base, bool orientacaoHorizontal);
        virtual ~EixoDinamico();
        double minimoPadrao;
        double maximoPadrao;
        bool orientacaoHorizontal;

    protected:

    private:
};

#endif // EIXODINAMICO_H


===============================================================================
===== Eixo.h
===============================================================================

#ifndef EIXO_H
#define EIXO_H
#include <iostream>
using namespace std;
class Eixo
{
    public:
        Eixo(string titulo, double minimo, double maximo);
        virtual ~Eixo();
        virtual string getTitulo();
        virtual double getMinimo();
        virtual double getMaximo();


    protected:
        string titulo;
        double minimo;
        double maximo;
    private:
};

#endif // EIXO_H


===============================================================================
===== Grafico.cpp
===============================================================================

#include "Grafico.h"
#include "Eixo.h"
#include "Serie.h"
#include "Tela.h"
//#include "Tela.h"
//testar TODAS as possibilidades
Grafico::Grafico(Eixo *x, Eixo *y, Serie *serie){
    this-> x = x;
    this -> y = y;
    this -> serie = serie;
}

Grafico::~Grafico(){
}

Eixo* Grafico::getEixoX(){
    return x;
}

Eixo* Grafico::getEixoY(){
    return y;
}

Serie* Grafico::getSerie(){
    return serie;
}

void Grafico::desenhar(){
    int i;
    Tela*t= new Tela;
    t-> setEixoX(x->getTitulo(), x-> getMinimo(), x-> getMaximo());
    t-> setEixoY(y->getTitulo(), y-> getMinimo(), y-> getMaximo());
    for(i=0; i < serie->getQuantidade();i++){
        t -> plotar(serie->getNome(), serie-> getPosicao(i)-> getX(), serie-> getPosicao(i)-> getY());
    }
    t-> mostrar();
    delete t;
}


===============================================================================
===== Grafico.h
===============================================================================

#ifndef GRAFICO_H
#define GRAFICO_H
#include "Eixo.h"
#include "Serie.h"
#include "Eixo.h"
class Grafico
{
    public:
        Grafico(Eixo *x, Eixo *y, Serie *serie);
        virtual ~Grafico();
        Eixo* getEixoX();
        Eixo* getEixoY();
        Serie* getSerie();
        void desenhar();

    protected:
        //não sei se ta certo fazer isso
        Eixo*x ;
        Eixo*y ;
        Serie*serie ;
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
#include "Tela.h"
#include "Eixo.h"
#include "EixoDinamico.h"
#include "Grafico.h"
#include "SerieTemporal.h"
#define COMM  "\\\\.\\COM3"
using namespace std;

int main()
{
    InterfaceSerial*inter1 = new InterfaceSerial(COMM);
    inter1-> inicializar();
    string canalX;
    string canalY;
    string nomeSerie;
    int i, quantidadeDePontos;
    double valorX, valorY, valorMax, valorMin;
    string situacao;
    string titulo;
    Eixo*eixoX;
    Eixo* eixoY;
    Grafico* g;
    cout<< "Informe o nome da serie: "<< endl;
    cin >> nomeSerie;
    cout<<"Escolha o canal X:"<< endl;
    cout<<"0) Tempo"<< endl;
    for (i=0; i <= inter1 -> getQuantidadeDeCanais(); i++){
        cout<< i+1<< ") "<< inter1 -> getNomeDosCanais()[i] <<endl;
    }
    cin >> i;
    if(i==0) canalX = "Tempo";
    else{
        canalX = inter1 -> getNomeDosCanais()[i-1];
    }
    cout<<"Escolha o nome do canal Y"<< endl;
    for (i=0; i <= inter1 -> getQuantidadeDeCanais(); i++){
        cout<< i+1<< ") "<< inter1 -> getNomeDosCanais()[i] <<endl;
    }
    cin >> i;
    canalY = inter1 -> getNomeDosCanais()[i-1];
    Serie*s = new Serie(nomeSerie, canalX, canalY);
    SerieTemporal*st = new SerieTemporal(nomeSerie,canalY);
    cout<< "Obter quantos pontos?"<< endl;
    cin >> quantidadeDePontos;
    cout << "Obtendo os pontos"<<endl;
    for (i = 0; i < quantidadeDePontos; i++) {
        if (canalX != "Tempo"){
            inter1-> atualizar();
            valorX = inter1-> getValor (canalX);
            valorY = inter1 -> getValor (canalY);
            s -> adicionar (valorX, valorY);
        }
        if (canalX == "Tempo"){
            inter1-> atualizar();
            valorY = inter1 -> getValor (canalY);
            st -> adicionar (valorY);
        }
    }
    cout << "Gerando o gráfico" << endl;

    cout << "O eixo X e estatico ou dinamico(e/d): " << endl;
    cin >> situacao;

    if (situacao == "e"){
        if (canalX != "Tempo"){
            cout<< "Informe o titulo: "<< endl;
            cin >> titulo;
        }
        if (canalX == "Tempo") titulo = "Tempo";
        cout<< "Valor minimo: "<< endl;
        cin >> valorMin;
        cout<< "Valor maximo: "<< endl;
        cin >> valorMax;
        Eixo* eixoX = new Eixo(titulo,valorMin, valorMax);
    }
    if (situacao == "d"){
        cout<< "Valor minimo padrao: "<< endl;
        cin >> valorMin;
        cout<< "Valor maximo padrao: "<< endl;
        cin >> valorMax;
        if (canalX == "Tempo"){
                EixoDinamico* eixoX = new EixoDinamico(valorMin, valorMax, st, true);
        }
        else{
            EixoDinamico*eixoX = new EixoDinamico(valorMin, valorMax, s, true);
        }
    }
     cout << "O eixo Y e estatico ou dinamico(e/d): " << endl;
    cin >> situacao;

    if (situacao == "e"){
        cout<< "Informe o titulo: "<< endl;
        cin >> titulo;
        cout<< "Valor minimo: "<< endl;
        cin >> valorMin;
        cout<< "Valor maximo: "<< endl;
        cin >> valorMax;
        Eixo* eixoY = new Eixo(titulo,valorMin, valorMax);
    }
    if (situacao == "d"){
        cout<< "Valor minimo padrao: "<< endl;
        cin >> valorMin;
        cout<< "Valor maximo padrao: "<< endl;
        cin >> valorMax;
        if (canalX == "Tempo") EixoDinamico*eixoY = new EixoDinamico(valorMin, valorMax, st, true);
        else{
            EixoDinamico*eixoX = new EixoDinamico(valorMin, valorMax, s, true);
        }
    }
    if (canalX == "Tempo") Grafico*g = new Grafico(eixoX, eixoY, st);
    else{
        Grafico*g = new Grafico(eixoX, eixoY, s);

    }
    g -> desenhar();
    delete g;
    delete eixoX;
    delete eixoY;
    delete st;
    delete s;
    delete inter1;
    return 0;




}




===============================================================================
===== Ponto.cpp
===============================================================================

#include "Ponto.h"
#define e 0.00001
#include <cmath>

// SE PRECISAR USAR COUT using namespace std;

using namespace std;

Ponto::Ponto(double x, double y){
    this->coordenadaX = x;
    this->coordenadaY = y;
}

Ponto::~Ponto(){

}

double Ponto::getX(){
    //se o conteudo da posição indicada for nulo ele fica doidao e para de rodar
    return coordenadaX;
}

double Ponto::getY(){
    //se o conteudo da posição indicada no vetor for nulo ele fica maluco
    return coordenadaY;
}



void Ponto::imprimir(){
    cout<< "( " << getX() << " , " << getY() << " )" << endl;
}

bool Ponto::eIgual(Ponto* outro){
    // ver se o X e o Y sao iguais.
    double Xoutro = outro->getX();
    double Youtro = outro->getY();
    double Xesse = this->getX();
    double Yesse = this->getY();
    if (abs(Xoutro-Xesse)<e && abs(Youtro-Yesse)<e)return true;
    else{
        return false;
    }
}


===============================================================================
===== Ponto.h
===============================================================================

#ifndef PONTO_H
#define PONTO_H
#include <iostream>

using namespace std;

class Ponto
{
    private:

protected:
    double coordenadaX;
    double coordenadaY;
public:
    Ponto(double x, double y);

    virtual ~Ponto();

    double getX();

    double getY();

    void imprimir();

    bool eIgual(Ponto* outro);
};

#endif // PONTO_H


===============================================================================
===== Serie.cpp
===============================================================================

#include "Serie.h"
#include<stdio.h>
#include <iostream>
#include <cstddef>

using namespace std;

Serie::Serie(string nomeSerie, string nomeDoCanalX, string nomeDoCanalY){
    this-> nomeSerie = nomeSerie;
    this-> nomeX = nomeDoCanalX;
    this-> nomeY = nomeDoCanalY;
}

Serie::~Serie(){

}


string Serie::getNome(){
    return nomeSerie;
}

string Serie::getNomeDoCanalX(){
    return nomeX;
}

string Serie::getNomeDoCanalY(){
    return nomeY;
}

int Serie::getQuantidade(){
    return qntd;
}

bool Serie::estaVazia(){
    if (getQuantidade()==0){
        return true;
    }else {
        return false;
    }
}

void Serie::adicionar(double x, double y){ // add popnto numa serie ja ex   ente
    int aux1;
    aux1=getQuantidade();
    Ponto *a = new Ponto(x,y);
    if(aux1 < NUMERO_MAXIMO_VALORES){
        vetorDePontos[aux1] = a;
        Ponto *a = new Ponto(x,y);
        qntd ++;
    }
}

Ponto* Serie::getLimiteSuperior(){
    //Ponto *p = new Ponto(); // para criar um ponto preciso passar seus
    // argumentos X e Y como a ideia original era pegar
    /*    aux1 = vetorDePontos[0]->coordenadaX;
          aux2 = vetorDePontos[0]->coordenadaY;
          p -> setX(aux1);
          p-> setY (aux2);

          */
    int tamanhodovetor = getQuantidade();
    int i = 0;
    double aux1, aux2;
    if (tamanhodovetor == 0){
        return NULL;
    }else {
          aux1 = vetorDePontos[0]->getX();
          aux2 = vetorDePontos[0]->getY();
          Ponto *p = new Ponto(aux1,aux2);
        for (i=1;i < tamanhodovetor; i++){
            if (vetorDePontos[i]->getX() > p->getX()){
                aux1 = vetorDePontos[i]->getX();
            }
            if (vetorDePontos[i]->getY() > p->getY()){
                aux2 = vetorDePontos[i]->getY();
            }
            Ponto *p = new Ponto(aux1,aux2);
        }
        return p;
        }
}

Ponto* Serie::getLimiteInferior(){
    int tamanhodovetor = this -> getQuantidade();
    int i = 0;
    double aux1, aux2;
    if (tamanhodovetor == 0){
        return NULL;
    }else {
          aux1 = vetorDePontos[0]->getX();
          aux2 = vetorDePontos[0]->getY();
          Ponto *p = new Ponto(aux1,aux2);
        for (i=1;i < tamanhodovetor; i++){
            if (vetorDePontos[i]->getX() < p->getX()){
                aux1 = vetorDePontos[i]->getX();
            }
            if (vetorDePontos[i]-> getY()< p->getY()){
                aux2 = vetorDePontos[i]->getY();
            }
            Ponto *p = new Ponto(aux1,aux2);
        }
        return p;
        }
}

Ponto* Serie::getPosicao(int posicao){ // obtem as coordenadas X e Y na posição especificada.
    double x,y;
    if (getQuantidade() == 0) return NULL;
    if(posicao >=0 && posicao <NUMERO_MAXIMO_VALORES){
        // COLOQUEI NUMERO MAXIMO VALORES E NAO QUANTIDADE
        x = vetorDePontos[posicao]->getX();
        y = vetorDePontos[posicao]->getY();
        Ponto*p = new Ponto(x,y);
        return p;
    }else {
        return NULL;
    }
}

void Serie::imprimir(){
    int aux2 = getQuantidade();
    int j = 0;
    //double xaux;
    //double yaux;
    if (aux2 == 0){
        cout<< "Serie "<< nomeSerie<<endl;
        cout<< "EMPTY"<< endl;
    }else{
        cout<< "Serie "<< nomeSerie<<endl;
        while (j<aux2){
            vetorDePontos[j] -> imprimir();
            //xaux = vetordepontos[j]->coordenadaX;
            //yaux = vetordepontos[j]->coordenadaY;
            //cout<< "(" << xaux <<", " <<yaux << ")"<<endl;
            j = j +1;
    }
    }
}



===============================================================================
===== Serie.h
===============================================================================

#ifndef SERIE_H
#define SERIE_H
#include <iostream>
#include "Ponto.h"
#include <string>
#define NUMERO_MAXIMO_VALORES 10
using namespace std;


class Serie
{
    private:
    public:

        Serie(string nome, string nomeDoCanalX, string nomeDoCanalY);
        virtual ~Serie();
        Ponto* vetorDePontos[NUMERO_MAXIMO_VALORES];
        virtual int getQuantidade();
        virtual bool estaVazia();
        virtual void adicionar(double x, double y);
        virtual Ponto* getLimiteSuperior();
        virtual Ponto* getLimiteInferior();
        virtual Ponto* getPosicao(int posicao);
        virtual void imprimir();
        string getNome();
        string getNomeDoCanalX();
        string getNomeDoCanalY();
    protected:
        // Permite obter o nome, o nomeDoCanalX e o nomeDoCanalY.

         string nomeX;
        string nomeY;
        string nomeSerie;
        int qntd = 0; // quantidade de elementos da serie
};

#endif // SERIE_H


===============================================================================
===== SerieTemporal.cpp
===============================================================================

#include "SerieTemporal.h"
#include "Serie.h"
#define e 0.00001
#include <cmath>

SerieTemporal::SerieTemporal (string nome, string NomeDoCanalY):Serie(nome,"Tempo", NomeDoCanalY){
}

SerieTemporal::~SerieTemporal(){
}

void SerieTemporal::adicionar (double valor){
    int i = tempo;
    SerieTemporal::adicionar(i, valor);
    tempo ++;
}

void SerieTemporal::adicionar (double x,double y){
    int i;
    bool passou = false;
    Ponto*p = new Ponto(x,y);
    for(i = 0;i < getQuantidade();i++ ){
        if(abs(p->getX()-vetorDePontos[i]->getX())< e){
                vetorDePontos[i] = p;
                passou = true;
        }
    }
    if(!passou) Serie::adicionar(x,y);
}


===============================================================================
===== SerieTemporal.h
===============================================================================

#ifndef SERIETEMPORAL_H
#define SERIETEMPORAL_H
#include "Serie.h"

class SerieTemporal:public Serie
{
    public:
        SerieTemporal (string nome, string NomeDoCanalY);
        virtual ~SerieTemporal();
        virtual void adicionar (double valor);
        virtual void adicionar (double x,double y);

    protected:
    int tempo;
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


