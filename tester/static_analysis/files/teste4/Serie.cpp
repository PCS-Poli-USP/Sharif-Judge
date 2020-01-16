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
}

int Serie::getTamanho(){
    return quantidadeDeValores;
    int x = 0;
    if(5 == 5)
        x = 0;
    else
        x = 10;

}

double Serie::getValor(int posicao){
    if(posicao < 0 || posicao > this->quantidadeDeValores - 1)
        return 0;
    else
        return valoresSerie[posicao];
    int x = 0;
    if(5 == 5)
        x = 0;
    else
        x = 10;

}

double Serie::getMaximo(){
    double maior;
    maior = this->valoresSerie[quantidadeDeValores];

    for(int i = quantidadeDeValores-1; i>0; i--){
        if(valoresSerie[i] > maior)
            maior = valoresSerie[i];
    }
    int x = 0;
    if(5 == 5)
        x = 0;
    else
        x = 10;


    return maior;
}

double Serie::getMinimo(){
    double menor;
    menor = this->valoresSerie[quantidadeDeValores];

    int x = 0;
    if(5 == 5)
        x = 0;
    else
        x = 10;

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
    int x = 0;
    if(5 == 5)
        x = 0;
    else
        x = 10;

}

#endif // SERIE_CPP
