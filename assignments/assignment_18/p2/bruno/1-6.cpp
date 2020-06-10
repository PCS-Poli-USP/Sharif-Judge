===============================================================================
===== Arquivos injetados pelo sistema de correção:
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
===== 1
===============================================================================



===============================================================================
===== Eixo.cpp
===============================================================================

#include "Eixo.h"
#include "iostream"
using namespace std;

Eixo::Eixo(string titulo, double minimo, double maximo) :
titulo(titulo), minimo(minimo), maximo(maximo)
{

}

Eixo::~Eixo()
{
   cout << "Eixo destruido" << endl;
}

string Eixo::getTitulo()
{
    return titulo;
}

double Eixo::getMinimo()
{
    return minimo;
}

double Eixo::getMaximo()
{
    return maximo;
}


===============================================================================
===== EixoDinamico.cpp
===============================================================================

#include "EixoDinamico.h"
#include "Serie.h"
#include "iostream"

EixoDinamico::EixoDinamico(double minimoPadrao, double maximoPadrao,
                           Serie* base, bool orientacaoHorizontal):
                               Eixo (base->getNome(), minimoPadrao, maximoPadrao)
{
    this->orientaHorizontal = orientacaoHorizontal;

    if (this->orientaHorizontal == true)
    {
        this->titulo = base->getNomeDoCanalX();
        if (base->getQuantidade() >= 2 &&
            (base->Pontos[0]->getX() !=  base->Pontos[1]->getX()) )
        {
            this->maximo = base->getLimiteSuperior()->getX();
            this->minimo = base->getLimiteInferior()->getX();
        }
        else
        {
            this->maximo = maximoPadrao;
            this->minimo = minimoPadrao;
        }
    }
    else
    {
        this->titulo = base->getNomeDoCanalY();
        if (base->getQuantidade() >= 2 &&
            (base->Pontos[0]->getY() !=  base->Pontos[1]->getY()) )
        {
            this->maximo = base->getLimiteSuperior()->getY();
            this->minimo = base->getLimiteInferior()->getY();
        }
        else
        {
            this->maximo = maximoPadrao;
            this->minimo = minimoPadrao;
        }
    }
}

EixoDinamico::~EixoDinamico()
{
    cout << "EixoDinamico destruido" << endl;
}


===============================================================================
===== EixoDinamico.h
===============================================================================

#ifndef EIXODINAMICO_H
#define EIXODINAMICO_H
#include "Eixo.h"
#include "Serie.h"

class EixoDinamico : public Eixo
{
    public:
        EixoDinamico(double minimoPadrao, double maximoPadrao,
                     Serie* base, bool orientacaoHorizontal);
        virtual ~EixoDinamico();

    protected:
        double minimoPadrao;
        double maximoPadrao;
        bool orientaHorizontal;
        string Nome;
        string NomeDoCanalX;
        string NomeDoCanalY;
        Serie* base = new Serie(Nome, NomeDoCanalX, NomeDoCanalY);
    private:
};

#endif // EIXODINAMICO_H


===============================================================================
===== Eixo.h
===============================================================================

#ifndef EIXO_H
#define EIXO_H
#include "string"
using namespace std;

class Eixo
{
    public:
        Eixo( string titulo, double minimo, double maximo);
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

Grafico::Grafico(Eixo *x, Eixo* y, Serie* serie){
    this->x = new Eixo(x->getTitulo(), x->getMinimo(), x->getMaximo());
    this->y = new Eixo(y->getTitulo(), y->getMinimo(), y->getMaximo());
    this->serie = serie;
}

Grafico::~Grafico(){
    /*Destructor*/
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

    t = new Tela;
    t->setEixoX(x->getTitulo(), x->getMinimo(), x->getMaximo());
    t->setEixoY(y->getTitulo(), y->getMinimo(), y->getMaximo());

    for(i = 0; i < serie->Quantidade; i++){

        t->plotar(serie->getNome(), serie->Pontos[i]->getX() , serie->Pontos[i]->getY());

    }

    t->mostrar();
    delete t;
}


===============================================================================
===== Grafico.h
===============================================================================

#ifndef GRAFICO_H
#define GRAFICO_H

#include "Tela.h"
#include "Serie.h"
#include "Eixo.h"

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

    private:
        double maximo;
        double minimo;
        string titulo;
        string Nome;
        string NomeDoCanalX;
        string NomeDoCanalY;
        Eixo* x;
        Eixo* y;
        Serie* serie = new Serie(Nome, NomeDoCanalX, NomeDoCanalY);
        Tela* t;
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
#include <string>

#include "Serie.h"
#include "Ponto.h"
#include "InterfaceSerial.h"
#include "SerieTemporal.h"
#include "Eixo.h"
#include "EixoDinamico.h"
#include "Grafico.h"

#define COMM "\\\\.\\COM3"

using namespace std;

int main() {

    Grafico* graphic;
    Eixo *X;
    Eixo *Y;
    char choose;
    string title;
    double minimo, maximo;
    Serie *ser1;
    SerieTemporal *ser2;
    int i = 0;
    InterfaceSerial* is = new InterfaceSerial(COMM);
    is->inicializar();

    cout << "Aperte o botao reset da placa." << endl;
  // Obtem o nome e o atribui a serie ser
    string nomeSerie;
    cout << "Informe o nome da serie: ";
    cin >> nomeSerie;
    cout << endl;

  // Obtem  os canais escolhidos
    string canalX;
    string canalY;
    int quantidadeCanais = is->getQuantidadeDeCanais();
    int escolha;

  //Imprime a lista de canais para X
    cout << "Escolha o Canal X: " << endl;
    cout << "0) Tempo" << endl;
    for(i=0; i < quantidadeCanais; i++){
    cout << i + 1 << ") " << is->getNomeDosCanais()[i] << endl;
    }
    cin >> escolha;

    if(escolha == 0)
        canalX = "Tempo";
    else
        canalX = is->getNomeDosCanais()[escolha - 1];


  //Imprime a lista de canais para Y
    cout << "Escolha o Canal Y: " << endl;
    for(i=0; i < quantidadeCanais; i++){
        cout << i + 1 << ") " << is->getNomeDosCanais()[i] << endl;
    }
    cin >> escolha;
    canalY = is->getNomeDosCanais()[escolha - 1];

  // Obtem o numero de Pontos a adicionar
    int quantidade;
    cout << "Obter quantos pontos? ";
    cin >> quantidade;
    cout << endl;

    ser1 = new Serie(nomeSerie, canalX, canalY);
    ser2 = new SerieTemporal(nomeSerie, canalY);

    if(canalX != "Tempo"){
        cout << "Obtendo os pontos" << endl;
        delete ser2;
        for (i = 0; i < quantidade; i++){
            is->atualizar();
            ser1->adicionar(is->getValor(canalX), is->getValor(canalY));
      }
    }

    else{
        cout << "Obtendo os pontos" << endl;
        delete ser1;
        for (i = 0; i < quantidade; i++) {
            is->atualizar();
            ser2->adicionar(is->getValor(canalY));
        }
    }

    cout << "Gerando o grafico" << endl;
    cout << "O eixo X e estatico ou dinamico (e/d):";
    cin >>  choose;
    cout << endl;

    if(choose == 'e'){
        cout << "Informe o titulo:";
        cin >> title;
        cout << endl << "Valor minimo:";
        cin >> minimo;
        cout << endl << "Valor maximo:";
        cin >> maximo;
        cout << endl;

        X = new Eixo(title, minimo, maximo);
    }
    else{
        cout << "Valor minimo padrao:";
        cin >> minimo;
        cout << endl << "Valor maximo padrao:";
        cin >> maximo;
        cout << endl;

        if(canalX != "Tempo"){
            X = new EixoDinamico(minimo, maximo, ser1, true);
        }
        else{
            X = new EixoDinamico(minimo, maximo, ser2, true);
        }
    }

    cout << "O eixo Y e estatico ou dinamico (e/d):";
    cin >>  choose;
    cout << endl;

    if(choose == 'e'){
        cout << "Informe o titulo:";
        cin >> title;
        cout << endl << "Valor minimo:";
        cin >> minimo;
        cout << endl << "Valor maximo:";
        cin >> maximo;
        cout << endl;

        Y = new Eixo(title, minimo, maximo);
    }
    else{
        cout << "Valor minimo padrao:";
        cin >> minimo;
        cout << endl << "Valor maximo padrao:";
        cin >> maximo;
        cout << endl;

        if(canalX != "Tempo"){
            Y = new EixoDinamico(minimo, maximo, ser1, false);
        }
        else{
            Y = new EixoDinamico(minimo, maximo, ser2, false);
        }
    }

    if(canalX != "Tempo")
        graphic = new Grafico(X, Y, ser1);
    else
        graphic = new Grafico(X, Y, ser2);

    graphic->desenhar();



    return 0;

}


===============================================================================
===== Ponto.cpp
===============================================================================

#include "Ponto.h"
#include <iostream>
#define eps 1e-5
#include <math.h>

using namespace std;


Ponto::Ponto(double x, double y){

    this->X = x;
    this->Y = y;
}

Ponto::~Ponto(){
    /*Destructor*/
}

double Ponto :: getX(){
  return X;
}


double Ponto :: getY(){
  return Y;
}


void Ponto :: imprimir(){
  cout << "(" << X << ", " << Y << ")" << endl;
 }

bool Ponto :: eIgual(Ponto* outro){
    if (fabs(outro->X - this->X) <= eps && fabs(outro->Y - this->Y) <= eps)
      return true;
    else return false;
}



===============================================================================
===== Ponto.h
===============================================================================

#ifndef PONTO_H
#define PONTO_H
#define eps 1e-5
#include <math.h>

class Ponto {
public:
    double X;
    double Y;

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
#include <iostream>

using namespace std;

Serie::Serie(string Nome, string NomeDoCanalX, string NomeDoCanalY){

  this->Nome = Nome;
  this->NomeDoCanalX = NomeDoCanalX;
  this->NomeDoCanalY = NomeDoCanalY;

}

Serie::~Serie(){

  /*Destructor */

}

string Serie::getNome(){
  return Nome;
}

string Serie::getNomeDoCanalX(){
  return NomeDoCanalX;
}

string Serie::getNomeDoCanalY(){
  return NomeDoCanalY;
}

bool Serie::estaVazia(){
  if (Quantidade == 0)
    return true;
  else return false;
}

int Serie::getQuantidade(){
  return Quantidade;
}

void Serie::adicionar(double x, double y){
  if(Quantidade < NUMERO_MAXIMO_VALORES){

    Pontos[Quantidade] = new Ponto(x, y);
    Quantidade++;
  }
}

Ponto* Serie::getPosicao(int posicao){

  if(estaVazia())
    return NULL;

  else{
    if(posicao<0 || posicao > Quantidade)
      return NULL;
    return Pontos[posicao];
  }
}

Ponto* Serie::getLimiteSuperior(){

  if(Quantidade > 0){
      Ponto* PontoSuperior;
      int i;
      double maiorX, maiorY;
      maiorX = Pontos[0]->getX();
      maiorY = Pontos[0]->getY();

  //Encontra os maiores valores de X e Y entre os pontos da serie
      for(i = 1; i < Quantidade; i++){
          if(Pontos[i]->getX() > maiorX)
            maiorX = Pontos[i]->getX();
          if(Pontos[i]->getY() > maiorY)
            maiorY = Pontos[i]->getY();
      }
  //Atribui ao Atributo PontoSuperior os valores obtidos
      PontoSuperior = new Ponto(maiorX, maiorY);

    return PontoSuperior;
  }

  else
    return NULL;

}

Ponto* Serie::getLimiteInferior(){

  if(Quantidade > 0){
      Ponto* PontoInferior;
      int i;
      double menorX, menorY;
      menorX = Pontos[0]->getX();
      menorY = Pontos[0]->getY();


  //Encontra os maiores valores de X e Y entre os pontos da serie
      for(i = 1; i < Quantidade; i++){
          if(Pontos[i]->getX() < menorX)
            menorX = Pontos[i]->getX();
          if(Pontos[i]->getY() < menorY)
            menorY = Pontos[i]->getY();
      }
  //Atribui ao Atributo PontoSuperior os valores obtidos

  PontoInferior = new Ponto(menorX, menorY);

  return PontoInferior;
  }

  else
    return NULL;

}

void Serie::imprimir(){


  cout << "Serie: " << this->Nome << endl;
  int i;
  for (i = 0; i < Quantidade; i++){
    Pontos[i]->imprimir();
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
#include <iostream>

using namespace std;

class Serie {
public:

  //Atributos:
  Ponto* Pontos[NUMERO_MAXIMO_VALORES];
  string Nome;
  string NomeDoCanalX;
  string NomeDoCanalY;
  int Quantidade = 0;


  Serie(string Nome, string NomeDoCanalX, string NomeDoCanalY);
  virtual ~Serie();
  // Permite definir o nome, o nomeDoCanalX e o nomeDoCanalY.
  void setNome(string nome);
  void setNomeDoCanalX(string nomeDoCanalX);
  void setNomeDoCanalY(string nomeDoCanalY);

  // Permite obter o nome, o nomeDoCanalX e o nomeDoCanalY.
  string getNome();
  string getNomeDoCanalX();
  string getNomeDoCanalY();

  /**
   * Informa a quantidade de pontos que a Serie possui.
   */
  int getQuantidade();

  /**
   * Informa se a Serie esta vazia.
   */
  bool estaVazia();

  /**
   * Adiciona um novo ponto a Serie, informando sua coordenada x e y.
   * Ignora o valor passado caso o NUMERO_MAXIMO_VALORES tenha
   * sido ultrapassado.
   */
  void adicionar(double x, double y);

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
  Ponto* getLimiteSuperior();

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
  Ponto* getLimiteInferior();

  /**
   * Obtem o ponto que esta na posicao definida da Serie. A contagem de
   * posicoes comeca em 0.
   *
   * Em caso de posicoes invalidas, retorne NULL.
   *
   * Por exemplo, para a Serie {(2, 3), (5, 1), (1, 2)}, getPosicao(0)
   * deve retornar (2, 3) e getPosicao(2) deve retornar (1, 2).
   */
  Ponto* getPosicao(int posicao);

  /**
   * Imprime na saida padrao (cout) o nome da Serie e seus pontos
   * seguindo o formato definido.
   */
  void imprimir();

  // ADICIONE OS ATRIBUTOS NECESSARIOS
};

#endif // SERIE_H


===============================================================================
===== SerieTemporal.cpp
===============================================================================


#include <iostream>
#include <math.h>

#include "SerieTemporal.h"

#define eps 1e-5

using namespace std;

SerieTemporal::SerieTemporal(string nome, string nomeDoCanalY) : Serie(nome, "Tempo", nomeDoCanalY){
    tempo = 1.0;
}

SerieTemporal::~SerieTemporal(){
    /*Destructor*/
}

void SerieTemporal::adicionar(double valor){

  int i;
  bool check = false;

  if(Quantidade < NUMERO_MAXIMO_VALORES){

        for(i = 0; i < Quantidade && check == false; i++){
            if(fabs(Pontos[i]->getX() - tempo) < eps){
                delete Pontos[i];
                Pontos[i] = new Ponto(tempo, valor);
                check = true;
            }
        }


        if(check == false){
            Pontos[Quantidade] = new Ponto(tempo, valor);
            tempo = tempo + 1.0;
            Quantidade++;
        }
  }
}

void SerieTemporal::adicionar(double x, double y){
    int i;
    bool check = false;

    if(Quantidade < NUMERO_MAXIMO_VALORES && x > 1.0){

        for(i = 0; i < Quantidade && check == false; i++){


            if(fabs(Pontos[i]->getX() - x) < eps){
                delete Pontos[i];
                Pontos[i] = new Ponto(x, y);
                check = true;
            }
        }

        if(check == false){
            Pontos[Quantidade] = new Ponto(x, y);
            Quantidade++;
        }
    }

}



===============================================================================
===== SerieTemporal.h
===============================================================================

#ifndef SERIETEMPORAL_H
#define SERIETEMPORAL_H
#include "Serie.h"
#include "Ponto.h"

class SerieTemporal : public Serie
{
    public:

        SerieTemporal(string nome, string nomeDoCanalY);
        virtual ~SerieTemporal();

        virtual void adicionar (double valor);

        virtual void adicionar(double x, double y);

    protected:

    private:
        double tempo;
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


