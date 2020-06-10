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

