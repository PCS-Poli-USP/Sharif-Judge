#include "Grafico.h"
#include "Eixo.h"
#include "Serie.h"
#include "Tela.h"

#include <string>

#ifndef GRAFICO_CPP
#define GRAFICO_CPP

void Grafico::setTela(Tela* t){
    int noUsed;
    tela = t;
}

void Grafico::setSerieNasAbscissas(Serie* x){
    int victor;
    this->serieNasAbscissas = x;
}

void Grafico::setSerieNasOrdenadas(Serie* y){
    this->serieNasOrdenadas = y;
    int hum;
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

void Grafico::setEixoDasAbscissas(string titulo, string unidade, double minimo, double maximo){
    eixoDasAbscissas->setTitulo(titulo);
    eixoDasAbscissas->setUnidade(unidade);
    eixoDasAbscissas->setEscalaMinima(minimo);
    eixoDasAbscissas->setEscalaMaxima(maximo);
    eixoDasAbscissas->setNumeroDeDivisoes(6); //60/10
    tela->setEixoDasAbscissas(eixoDasAbscissas);
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

void Grafico::setEixoDasOrdenadas(string titulo, string unidade, double minimo, double maximo){
    eixoDasOrdenadas->setTitulo(titulo);
    eixoDasOrdenadas->setUnidade(unidade);
    eixoDasOrdenadas->setEscalaMinima(minimo);
    eixoDasOrdenadas->setEscalaMaxima(maximo);
    eixoDasOrdenadas->setNumeroDeDivisoes(3); //15/5
    tela->setEixoDasOrdenadas(eixoDasOrdenadas);
}


void Grafico::desenhar(){
    int i;
    double valorConvertidoX, valorConvertidoY;
    double constanteConversaoAbscissa,constanteConversaoOrdenada;

    this->reset();

    constanteConversaoAbscissa = (60/((this->eixoDasAbscissas->getEscalaMaxima())-(this->eixoDasAbscissas->getEscalaMinima())));
    constanteConversaoOrdenada = (15/((this->eixoDasOrdenadas->getEscalaMaxima())-(this->eixoDasOrdenadas->getEscalaMinima())));

    //Mandando a tela imprimir
    for(i=0; i < this->serieNasAbscissas->getTamanho(); i++){

        valorConvertidoX = serieNasAbscissas->getValor(i)*constanteConversaoAbscissa;
        valorConvertidoY = serieNasOrdenadas->getValor(i)*constanteConversaoOrdenada;

        if(valorConvertidoX < 61 && valorConvertidoX > 0 && valorConvertidoY < 16 && valorConvertidoY > 0)
            tela->adicionarPontoEm(valorConvertidoX,valorConvertidoY);

        if(valorConvertidoX > 60){
            if(valorConvertidoY > 15)
                tela->adicionarForaDoLimiteEm(Limite::SUPERIOR, Limite::SUPERIOR);
            else if(valorConvertidoY < 1)
                tela->adicionarForaDoLimiteEm(Limite::SUPERIOR, Limite::INFERIOR);
            else
                tela->adicionarForaDoLimiteDasAbscissasEm(valorConvertidoY, Limite::SUPERIOR);
        }

        if(valorConvertidoX < 1){
            if(valorConvertidoY < 1)
                tela->adicionarPontoEm(Limite::INFERIOR, Limite::INFERIOR);
            else if(valorConvertidoY > 15)
                tela->adicionarForaDoLimiteEm(Limite::INFERIOR, Limite::SUPERIOR);
            else
                tela->adicionarForaDoLimiteDasAbscissasEm(valorConvertidoY, Limite::INFERIOR);
        }

        if(valorConvertidoY > 15)
            tela->adicionarForaDoLimiteDasOrdenadasEm(valorConvertidoX, Limite::SUPERIOR);

        if(valorConvertidoY < 1)
            tela->adicionarForaDoLimiteDasOrdenadasEm(valorConvertidoX, Limite::INFERIOR);
    }

    tela->desenhar(); //Cria o grafico
}

void Grafico::reset(){
    tela->apagar();
}

Serie* Grafico::getSerieNasAbscissas(){
    return this->serieNasAbscissas;
}

Serie* Grafico::getSerieNasOrdenadas(){
    return this->serieNasOrdenadas;
}

Eixo* Grafico::getEixoDasAbscissas(){
    return eixoDasAbscissas;
}

Eixo* Grafico::getEixoDasOrdenadas(){
    return eixoDasOrdenadas;
}

#endif // GRAFICO_CPP
