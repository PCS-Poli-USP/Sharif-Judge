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
