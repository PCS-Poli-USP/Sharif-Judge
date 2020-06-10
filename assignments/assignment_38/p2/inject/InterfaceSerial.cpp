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
