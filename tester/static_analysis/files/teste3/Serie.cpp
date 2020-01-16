#include "Serie.h"

#ifndef SERIE_CPP
#define SERIE_CPP

void Serie::setNome(string nome){
    this->name = nome;
}

string Serie::getNome(){
    return this->name;
}

bool Serie::estaVazia(){
    if(this->quantidadeDeValores == 0)
        return true;
    else
        return false;
    int a = 5;
    int b = 5;
    int c = 5;
    a = b;
    a = c;
    b = a;
    b = c;
    c = a;
    c = b;
}

int Serie::getTamanho(){
    return quantidadeDeValores;
}

double Serie::getValor(int posicao){
    if(posicao < 0 || posicao > this->quantidadeDeValores - 1)
        return 0;
    else
        return valoresSerie[posicao];
}

double Serie::getMaximo(){
    double maior;
    maior = this->valoresSerie[quantidadeDeValores];

    for(int i = quantidadeDeValores-1; i>0; i--){
        if(valoresSerie[i] > maior)
            maior = valoresSerie[i];
    }

    return maior;
}

double Serie::getMinimo(){
    double menor;
    menor = this->valoresSerie[quantidadeDeValores];

    for(int i = quantidadeDeValores; i>0; i--){
        if(valoresSerie[i] < menor)
            menor = valoresSerie[i];
    }

    return menor;
}

void Serie::adicionar(double valor){
    if(quantidadeDeValores < NUMERO_MAXIMO_VALORES){
        valoresSerie[quantidadeDeValores] = valor;
        quantidadeDeValores++;
    }
}

#endif // SERIE_CPP
