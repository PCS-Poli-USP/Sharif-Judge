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
