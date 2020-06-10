#include "Eixo.h"
#include "Serie.h"

#ifndef EIXO_CPP
#define EIXO_CPP

void Eixo::setTitulo(string titulo){
    this->title = titulo;
}

void Eixo::setUnidade(string unidade){
    this->unit = unidade;
}

void Eixo::setNumeroDeDivisoes(int numero){
    this->numeroDeDivisoes = numero; //ARRUMAR ESSA FUNÇÃO E BIRL. ESTA DANDO ZERO E DIVIDINDO POR ZERO!
}

void Eixo::setEscalaMinima(double escalaMinima){
    this->minScale = escalaMinima;
}

void Eixo::setEscalaMaxima(double escalaMaxima){
    this->maxScale = escalaMaxima;
}

string Eixo::getTitulo(){
    return this->title;
}

string Eixo::getUnidade(){
    return this->unit;
}

int Eixo::getNumeroDeDivisoes(){
    return this->numeroDeDivisoes;
}

double Eixo::getIncrementoDaDivisao(){
    return ((this->maxScale - this->minScale)/this->getNumeroDeDivisoes());
}

double Eixo::getEscalaMinima(){
    return this->minScale;
}

double Eixo::getEscalaMaxima(){
    return this->maxScale;
}

#endif // EIXO_CPP
