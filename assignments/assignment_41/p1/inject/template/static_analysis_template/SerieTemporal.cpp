
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

