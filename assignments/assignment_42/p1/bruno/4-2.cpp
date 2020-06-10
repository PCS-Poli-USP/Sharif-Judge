===============================================================================
===== 4
===============================================================================



===============================================================================
===== Eixo.cpp
===============================================================================

#include "Eixo.h"

using namespace std;

Eixo::Eixo(string titulo, double minimo, double maximo){
    this->titulo = titulo;
    this->minimo = minimo;
    this->maximo = maximo;
}

Eixo::~Eixo(){
    // cout << "Eixo destruido" << endl;
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

using namespace std;

EixoDinamico::EixoDinamico(double minimoPadrao, double maximoPadrao, Serie* base, bool orientacaoHorizontal)
			: Eixo("Default EixoDinamico", minimoPadrao, maximoPadrao)
{
	
	// Configurar titulo, minimo e maximo do pai "eixo"
	if (orientacaoHorizontal) {
		titulo = base->getNomeDoCanalX();
		minimo = base->getLimiteInferior()->getX();
		maximo = base->getLimiteSuperior()->getX();
	}
	else {
		titulo = base->getNomeDoCanalY();
		minimo = base->getLimiteInferior()->getY();
		maximo = base->getLimiteSuperior()->getY();
	}

	// Validar as configuracoes acima conforme o enunciado
		// Se serie tiver menos de 2 pontos ou min e max forem proximos usar o padrao
	if ((base->getQuantidade() < 2) || (std::abs(maximo - minimo) <= Epsilon)) {
		minimo = minimoPadrao;
		maximo = maximoPadrao;
	}
}

EixoDinamico::~EixoDinamico(){
    // cout << "EixoDinamico destruido" << endl;
}


===============================================================================
===== EixoDinamico.h
===============================================================================

#ifndef EIXODINAMICO_H
#define EIXODINAMICO_H

#include "Eixo.h"
#include "Serie.h"
#include <iostream>
#include <cmath> // para ter std::abs


using namespace std;

class EixoDinamico : public Eixo {
    public:
        EixoDinamico(double minimoPadrao, double maximoPadrao, Serie* base, bool orientacaoHorizontal);
        virtual ~EixoDinamico();

    protected:
		// Nao eh necessario redefinir titulo, minimo, maximo pois ja existem no pai
		const double Epsilon = 1E-5;
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
};

#endif // EIXO_H


===============================================================================
===== Grafico.cpp
===============================================================================

#include "Grafico.h"

using namespace std;

Grafico::Grafico(Eixo* x, Eixo* y, Serie* serie){
	myX = x;
	myY = y;
	mySerie = serie;

	if (x == nullptr || y == nullptr || serie == nullptr)
	{
		std::cout << "Classe grafico inicializada com Eixo X, Eixo Y ou Serie == nullptr ):\n";
	}
}

Grafico::~Grafico(){
	// cout << "Grafico destruido" << endl;
}

Eixo * Grafico::getEixoX()
{
	return myX;
}

Eixo * Grafico::getEixoY()
{
	return myY;
}

Serie * Grafico::getSerie()
{
	return mySerie;
}

void Grafico::desenhar(){

    Tela* t = new Tela;
	// Configurar eixos
    t->setEixoX(myX->getTitulo(), myX->getMinimo(), myX->getMaximo());
    t->setEixoY(myY->getTitulo(), myY->getMinimo(), myY->getMaximo());

	// for each ponto in mySerie, adicionar ao plotar da tela
	for (int ponto = 0; ponto < mySerie->getQuantidade(); ponto++) {
		t->plotar(mySerie->getNome(),
				  mySerie->getPosicao(ponto)->getX(),
			      mySerie->getPosicao(ponto)->getY());
	}

	// Mostrar a tela
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
#include "Tela.h"
#include <string>
#include <iostream>

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
		Eixo *myX, *myY;
		Serie *mySerie;
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

#include "main.h"

void main() {
	using std::cout; using std::endl;

	/**
	// Se algum dos testes da classe de Teste falhar, sera lancada uma exception e return false;
	Teste *Testador = new Teste();
	if (Testador->Testar_Todas_as_Classes()) { cout << "Sucesso"; }
	else { cout << "Erro"; }
	/**/

	// Inicializar o Interface Serial
		// Ele vai printar "Aperte o botao reset da placa"
		InterfaceSerial *is = new InterfaceSerial("COM1");

	// Inicializar a Serie
		Serie *S = nullptr;
		SerieTemporal *ST = nullptr;
		std::string Serie_Nome = getInput("Informe o nome da serie: ");
		std::string Serie_CanalX = getCanal("Escolha o canal X:", *is, true);
		std::string Serie_CanalY = getCanal("Escolha o canal Y:", *is);

		Serie_CanalX == "Tempo" ? ST = new SerieTemporal(Serie_Nome, Serie_CanalY)
								: S = new Serie(Serie_Nome, Serie_CanalX, Serie_CanalY);

	// Quantos pontos devemos obter? Obter pontos
		if (S != 0 && S != nullptr) { amostrarPontos(getNumeroPontos(*is, S), *is, S); }
		else if (ST != 0 && ST != nullptr) { amostrarPontos(getNumeroPontos(*is, S), *is, ST); }
		else { cout << "Nenhum objeto serie inicializado\n"; }

		cout << "Gerando o grafico\n";
	
	// Construir os Eixos
		Eixo *Eixo_X = nullptr;
		Eixo *Eixo_Y = nullptr;
		if (S != 0 && S != nullptr) { Eixo_X = construirEixo("X", S); Eixo_Y = construirEixo("Y", S); }
		else if (ST != 0 && ST != nullptr) { Eixo_X = construirEixo("X", ST); Eixo_Y = construirEixo("Y", ST); }
		else { cout << "Nenhum objeto serie inicializado\n"; }
	
	// Criar grafico
		Grafico *Graf = nullptr;
		if (S != 0 && S != nullptr) { Graf = new Grafico(Eixo_X, Eixo_Y, S); }
		else if (ST != 0 && ST != nullptr) { Graf = new Grafico(Eixo_X, Eixo_Y, ST); }
		else { cout << "Nenhum objeto serie inicializado\n"; }
	
	// Mostrar grafico
		Graf->desenhar();
	return;
}

void Imprimir_Limites(Serie &S)
{
	using std::cout;
	Ponto *P = nullptr;

	P = S.getLimiteSuperior();
	cout << "Limite Superior: ";
	if (P == nullptr) { cout << "nullptr\n"; } else { P->imprimir(); }

	P = S.getLimiteInferior();
	cout << "Limite Inferior: ";
	if (P == nullptr) { cout << "nullptr\n"; } else { P->imprimir(); }

	return;
}

std::string getInput(std::string Mensagem)
{
	using std::cout; using std::cin; using std::endl;

	std::string Input;
	
	cout << Mensagem;
	cin >> Input;
	//cout << endl;
	return Input;
}

std::string getCanal(std::string Mensagem, InterfaceSerial &is, bool MostrarCanalTempo)
{
	using std::cout; using std::cin; using std::endl;

	std::string CanalEscolhido = "";
	bool InputValido = 0;
	
	while (!InputValido)
	{
	// Imprimir mensagem pedindo o canal
		cout << Mensagem << endl;
	// Fazer a impressao dos canais disponiveis
		// Canal Tempo
		if (MostrarCanalTempo) { cout << "0) Tempo" << endl;	}
		// Canais do Interface Serial
		for (int i = 0; i < is.getQuantidadeDeCanais(); i++)
		{
			cout << (i + 1) << ") " << is.getNomeDosCanais()[i] << endl;
		}
	// Pegar input do user
		cin >> CanalEscolhido;
	// Validar input do user
		try
		{
			if ((stoi(CanalEscolhido) > 0 && stoi(CanalEscolhido) <= is.getQuantidadeDeCanais()) || (stoi(CanalEscolhido)==0 && MostrarCanalTempo))
			{
				InputValido = 1;
				if (stoi(CanalEscolhido) == 0) {
					CanalEscolhido = "Tempo";
				}
				else {
					CanalEscolhido = is.getNomeDosCanais()[stoi(CanalEscolhido) - 1]; // -1 pq o indice do user comeca em 1 e nao em zero.
				}
			}
			else
			{
				cout << "Canal Invalido. Digite um numero de " << (MostrarCanalTempo ? 0 : 1) << " a " << is.getQuantidadeDeCanais() << endl;
			}
		}
		catch (const std::exception& e)
		{
			cout << "EXCEPTION: " << e.what() << endl;
			cout << "Digite um numero de " << (MostrarCanalTempo ? 0 : 1) << " a " << is.getQuantidadeDeCanais() << endl;
		}
		//cout << endl;
	}

	return CanalEscolhido;
}

int getNumeroPontos(InterfaceSerial & is, Serie *S)
{
	using std::cout; using std::cin; using std::endl;

	std::string QtdPontos = "";
	bool InputValido = 0;

	// Pedir numero de pontos ate vir input valido
	while (!InputValido)
	{
		// Imprimir mensagem pedindo o canal
		cout << "Obter quantos pontos? ";

		// Pegar input do user e validar
		cin >> QtdPontos;
		try
		{
			if (stoi(QtdPontos) >= 0) { InputValido = 1; }
		}
		catch (const std::exception& e)
		{
			cout << "EXCEPTION: " << e.what() << endl;
			cout << "Digite um numero." << endl;
		}
		//cout << endl;
	}

	cout << "Obtendo os pontos...\n";

	return stoi(QtdPontos);
}

void amostrarPontos(int QtdPontos, InterfaceSerial & is, Serie * S)
{
	for (int i = 0; i < QtdPontos; i++)
	{
		if (!is.atualizar()) { std::cout << "Atualizar falhou no ponto " << i << endl; }
		S->adicionar(is.getValor(S->getNomeDoCanalX()), is.getValor(S->getNomeDoCanalY()));
	}

	//std::cout << endl;
}

void amostrarPontos(int QtdPontos, InterfaceSerial & is, SerieTemporal * ST)
{
	for (int i = 0; i < QtdPontos; i++)
	{
		if (!is.atualizar()) { std::cout << "Atualizar falhou no ponto " << i << endl; }
		ST->adicionar(is.getValor(ST->getNomeDoCanalY()));
	}

	//std::cout << endl;
}

Eixo* construirEixo(std::string Nome_do_Eixo, Serie *serieBase_EixoDinamico)
{
	using std::string;

	string Eixo_Tipo;
	string Tem_Padrao = "";
	double Eixo_valMinimo = 0.0;
	double Eixo_valMaximo = 0.0;

	// Perguntar se eh estatico ou dinamico
	bool Input_Valido = false;
	while (!Input_Valido) {
		Eixo_Tipo = getInput("O eixo " + Nome_do_Eixo + " e estatico ou dinamico (e/d): ");
		if (Eixo_Tipo == "e" || Eixo_Tipo == "d") { Input_Valido = true; }
	}
	
	// Se for dinamico, perguntar se eh horizontal
	// NO ENUNCIADO ELE NAO PEDE PARA PERGUNTAR ISSO, ENTAO FOI TIRADO FORA
	/// if (Eixo_Tipo == "d") {
	/// 	bool Input_Valido = false;
	/// 	while (!Input_Valido) {
	/// 		Eixo_Tipo = getInput("O eixo dinamico deve ser horizontal (s/n): ");
	/// 		if (Eixo_Tipo == "s" || Eixo_Tipo == "n") { Input_Valido = true; }
	/// 	}
	/// }

	if (Eixo_Tipo == "e") { Nome_do_Eixo = getInput("Informe o titulo: "); }
	else { Tem_Padrao = " padrao"; }

	Input_Valido = false;
	while (!Input_Valido) {
		try { Eixo_valMinimo = stod(getInput("Valor minimo" + Tem_Padrao + ": ")); Input_Valido = true; }
		catch (const std::exception& e)	{ cout << "EXCEPTION: " << e.what() << endl; }
	}

	Input_Valido = false;
	while (!Input_Valido) {
		try { Eixo_valMaximo = stod(getInput("Valor maximo" + Tem_Padrao + ": ")); Input_Valido = true; }
		catch (const std::exception& e) { cout << "EXCEPTION: " << e.what() << endl; }
	}

	if (Eixo_Tipo == "e") { return new Eixo(Nome_do_Eixo, Eixo_valMinimo, Eixo_valMaximo); }
	// Eixo Dinamico Horizontal
	// No enunciado ele nao pede para perguntar se eh horizontal ou nao, entao assume-se que eh horizontal...
	if (Eixo_Tipo == "s" || Eixo_Tipo == "d") { return new EixoDinamico(Eixo_valMinimo, Eixo_valMaximo,serieBase_EixoDinamico, true); }
	// Eixo Dinamico Nao Horizontal
	if (Eixo_Tipo == "n") { return new EixoDinamico(Eixo_valMinimo, Eixo_valMaximo, serieBase_EixoDinamico, false); }

	// Chegou aqui e nao retornou nada? deu pau!
	return nullptr;
}

===============================================================================
===== main.h
===============================================================================

#pragma once
#ifndef H_MAIN
#define H_MAIN

#include <iostream>
#include <cmath> // para usar pow na funcao Teste_Serie() 100 elementos
#include <string>

// Include das nossas classes
#include "Eixo.h"
#include "EixoDinamico.h"
#include "Grafico.h"
#include "InterfaceSerial.h"
#include "Ponto.h"
#include "Serie.h"
#include "SerieTemporal.h"
#include "Tela.h"
// #include "Teste.h" // Nossa classe de testes das outras classes


void main();

// Funcoes Utilizadas no main
void Imprimir_Limites(Serie &S);
std::string getInput(std::string Mensagem);
std::string getCanal(std::string Mensagem, InterfaceSerial &is, bool MostrarCanalTempo = false);
int getNumeroPontos(InterfaceSerial & is, Serie *S);

void amostrarPontos(int QtdPontos, InterfaceSerial & is, Serie * S);
void amostrarPontos(int QtdPontos, InterfaceSerial & is, SerieTemporal * ST);

Eixo* construirEixo(std::string Nome_do_Eixo, Serie *serieBase_EixoDinamico);


#endif

===============================================================================
===== Ponto.cpp
===============================================================================

#include "Ponto.h"

using namespace std;

Ponto::Ponto(double x, double y){
    Coordenada[0] = x;
    Coordenada[1] = y;
}

Ponto::~Ponto(){
	// cout << "Ponto destruido" << endl;
}

double Ponto::getX() const
{
	return Coordenada[0];
}

double Ponto::getY() const
{
	return Coordenada[1];
}


void Ponto::imprimir() const {
	std::cout << "(" << Coordenada[0] << ", " << Coordenada[1] << ")" << std::endl;
	return;
}

bool Ponto::eIgual(const Ponto * outro) const
{
	double DistX = std::abs(outro->getX() - Coordenada[0]);
	double DistY = std::abs(outro->getY() - Coordenada[1]);

	if (DistX <= Epsilon && DistY <= Epsilon) return true;

	return false;
}


===============================================================================
===== Ponto.h
===============================================================================

#pragma once
#ifndef H_PONTO
#define H_PONTO

#include <cmath>
#include <iostream>

class Ponto
{
public:

        Ponto(double x, double y);
        virtual ~Ponto();

		double getX() const;
		double getY() const;

		void imprimir() const;

		// Retorna 1 quando esse ponto e "outro" forem iguais. outro eh const pois a funcao nao mexe nele.
		bool eIgual(const Ponto* outro) const;

protected:
        // Coordenadas x e y, respectivamente.
        double Coordenada[2] = {0.0, 0.0};

		// Usado no metodo eIgual
		const double Epsilon = 1E-5;
};

#endif // !H_PONTO


===============================================================================
===== Serie.cpp
===============================================================================

#include "Serie.h"

using namespace std;

Serie::Serie(string nome, string nomeDoCanalX, string nomeDoCanalY){
    this->nome = nome;
    this->nomeDoCanalX = nomeDoCanalX;
    this->nomeDoCanalY = nomeDoCanalY;
}

Serie::~Serie(){
	// cout << "Serie destruida" << endl;
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

int Serie::getQuantidade() {
	return myPointsSize;
}

bool Serie::estaVazia() {
	return (myPointsSize==0);
}

void Serie::adicionar(double x, double y){
	// Nao deixa por mais pontos do que "numero maximo de valores"
	if (this->getQuantidade() >= NUMERO_MAXIMO_VALORES) { return; }

	// Se o vetor myPoints nao tiver cheio, adicionar ponto...
		myPoints[myPointsSize] = new Ponto(x, y);

		myPointsSize++;
		return;
}

/**
* Obtém um ponto representando o limite superior da Serie.
* A coordenada x desse ponto deve ser o máximo valor horizontal
* existente na Serie e a coordenada y deve ser o máximo valor
* vertical existente na Serie.
*
* Caso a Serie não tenha valores, deve-se retornar nullptr.
*/
Ponto * Serie::getLimiteSuperior() {
	if (this->estaVazia()) { return nullptr; }

	double XMax = myPoints[0]->getX(); // inicializando com primeiros valores
	double YMax = myPoints[0]->getY();

	// Iterar pelos pontos em busca do maximo X e Y
	for (int i = 0; i < this->getQuantidade(); i++) {
		if (myPoints[i]->getX() > XMax) { XMax = myPoints[i]->getX(); }
		if (myPoints[i]->getY() > YMax) { YMax = myPoints[i]->getY(); }
	}

	return new Ponto(XMax, YMax);
}

/**
* Obtêm um ponto representando o limite inferior da Serie.
* A coordenada x desse ponto deve ser o mínimo valor horizontal
* existente na Serie e a coordenada y deve ser o mínimo valor
* vertical existente na Serie.
*
* Caso a serie nao tenha valores, deve-se retornar nullptr.
*/
Ponto * Serie::getLimiteInferior()
{
	if (this->estaVazia()) { return nullptr; }

	

	double XMin = myPoints[0]->getX(); // inicializando com primeiros valores
	double YMin = myPoints[0]->getY();

	// Iterar pelos pontos em busca do minimo X e Y
	for (int i = 0; i < this->getQuantidade(); i++){
		if (myPoints[i]->getX() < XMin) XMin = myPoints[i]->getX();
		if (myPoints[i]->getY() < YMin) YMin = myPoints[i]->getY();
	}

	return new Ponto(XMin, YMin);
}

// Retorna o ponto na "posicao" da serie, nullptr se nao existir
Ponto * Serie::getPosicao(int posicao)
{
	// posicao >= getQtd já cobre a possibilidade de estar vazio 0 >= 0
	if ((posicao >= this->getQuantidade()) || (posicao < 0)) { return nullptr; }

	return new Ponto(myPoints[posicao]->getX(), myPoints[posicao]->getY());
}

void Serie::imprimir()
{
	std::cout << "Serie " << this->getNome() << std::endl;
	for (int i = 0; i < this->getQuantidade(); i++){
		myPoints[i]->imprimir();
	}
	std::cout << std::endl;
	return;
}


===============================================================================
===== Serie.h
===============================================================================

#pragma once
#ifndef H_SERIE
#define H_SERIE

#define NUMERO_MAXIMO_VALORES 10

#include "Ponto.h"
#include <string>

using namespace std;

class Serie
{
public:

    Serie(string nome, string nomeDoCanalX, string nomeDoCanalY);
    virtual ~Serie();

	// Funcoes Getters e Funcoes que poderiam ser const
		// "const" foi removido das funcoes para evitar conflitos com o Judge
		// caso ele fizesse uma ligação dinamica para uma funcao nao const
    virtual string getNome();
    virtual string getNomeDoCanalX();
    virtual string getNomeDoCanalY();

    virtual Ponto* getLimiteSuperior();
    virtual Ponto* getLimiteInferior();
    virtual Ponto* getPosicao(int posicao);

    virtual int getQuantidade();
    virtual bool estaVazia();
    virtual void imprimir();

	// Adiciona novo ponto a serie
	virtual void adicionar(double x, double y);

    protected:
        string nome;
        string nomeDoCanalX;
        string nomeDoCanalY;
        Ponto *myPoints[NUMERO_MAXIMO_VALORES];
        int myPointsSize = 0;

};
#endif



===============================================================================
===== SerieTemporal.cpp
===============================================================================

#include "SerieTemporal.h"

SerieTemporal::SerieTemporal(string nome, string nomeDoCanalY) : Serie(nome, "Tempo", nomeDoCanalY)
{
}

SerieTemporal::~SerieTemporal()
{
}

void SerieTemporal::adicionar(double valor)
{
	// Se encontrar um indice cujo |Tempo - VarAuxiliar| <= Epsilon, colocar valor nesse indice
	int Indice = encontraPontoComTempoIgual(Tempo_Adicionar_DeUmParam);
	if (Indice != -1)
	{
		//TODO: Qual deve ser o X usado nesse caso? Esperando resposta do forum: https://edisciplinas.usp.br/mod/forum/discuss.php?d=388273#p618998
			//myPoints[Indice] = new Ponto(myPoints[Indice]->getX(), valor);
		myPoints[Indice] = new Ponto(Tempo_Adicionar_DeUmParam, valor);
		return;
	}

	// Codigo abaixo so sera executado se Indice == -1, ou seja, nao achou valor de tempo proximo
	// Nesse caso, basta adicionar um ponto normalmente pelo metodo da classe Pai (Serie::adicionar)
	// no tempo da Var Auxiliar e Y = valor, por fim, incrementar a Var Auxiliar
	Serie::adicionar(Tempo_Adicionar_DeUmParam, valor);
	Tempo_Adicionar_DeUmParam += 1.0;
	return;
}

void SerieTemporal::adicionar(double x, double y)
{
	if (x < 1.0) { return; } // Nao adicionar se tempo < 1

	// Se encontrar um indice cujo |Tempo - x| <= Epsilon, colocar y nesse indice
	int Indice = encontraPontoComTempoIgual(x);
	if (Indice != -1)
	{
		//TODO: Qual deve ser o X usado nesse caso? Esperando resposta do forum: https://edisciplinas.usp.br/mod/forum/discuss.php?d=388273#p618998
			//myPoints[Indice] = new Ponto(myPoints[Indice]->getX(), y);
		myPoints[Indice] = new Ponto(x, y);
		return;
	}

	// Codigo abaixo so sera executado se Indice == -1, ou seja, nao achou valor de tempo proximo
	// Nesse caso, basta adicionar um ponto normalmente pelo metodo da classe Pai (Serie::adicionar)
	Serie::adicionar(x, y);
}

int SerieTemporal::encontraPontoComTempoIgual(double Tempo)
{
	double DistT = 0;

	// Retorna o indice do ponto presente na serie que satisfaz Tempo <= Epsilon
	for (int i = 0; i < getQuantidade(); i++)
	{
		DistT = std::abs(Tempo - myPoints[i]->getX());
		if (DistT <= Epsilon) return i; 
	}

	// Se nao encontrou nenhum ponto na serie que satisfaca, retorna -1.
	return -1;
}


===============================================================================
===== SerieTemporal.h
===============================================================================

#pragma once
#ifndef H_SERIETEMPORAL
#define H_SERIETEMPORAL

#include "Serie.h"
#include <cmath> // para usar std::abs

class SerieTemporal : public Serie
{
public:
	// Cria uma nova serie. O nome do canal X deve ser "Tempo"
	SerieTemporal(string nome, string nomeDoCanalY);
	virtual ~SerieTemporal();

	/**
	Adiciona um novo Ponto a Serie, no instante seguinte ao do
	ponto anterior adicionado por este método. O primeiro ponto
	deve ser adicionado no instante 1.
	*/
	virtual void adicionar(double valor);

	/**
	* Adiciona um Ponto a Serie informando a coordenada x e y.
	* Caso já exista um ponto cuja coordenada x seja suficientemente
	* próxima ao do valor x informado, ao invés de adicionar o ponto,
	* altere o valor anterior (mantendo sua posição no arranjo)
	* ao considerar a nova coordenada y.
	* Caso a coordenada x do ponto for < 1, o ponto não deve ser adicionado.
	*/
	virtual void adicionar(double x, double y);

protected:
	// Retorna o indice do ponto com tempo proximo ao Tempo passado no parametro
	// retorna -1 se nao houver ponto proximo (Proximo eh <= Epsilon)
	// (esse metodo nao pode ser const pois getQuantidade() nao eh const)
	int encontraPontoComTempoIgual(double Tempo);
	const double Epsilon = 1E-5;

	// Variavel auxiliar para o metodo adicionar(valor)
	double Tempo_Adicionar_DeUmParam = 1.0;
};

#endif //H_SERIETEMPORAL


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


