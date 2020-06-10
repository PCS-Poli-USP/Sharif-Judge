#include "Grafico.h"

Grafico::Grafico(Eixo *x, Eixo* y, Serie* serie){
    this->x = new Eixo(x->getTitulo(), x->getMinimo(), x->getMaximo());
    this->y = new Eixo(y->getTitulo(), y->getMinimo(), y->getMaximo());
    this->serie = serie;
}

Grafico::~Grafico(){
    /*Destructor*/
}

Eixo* Grafico::getEixoX(){
    return x;
}

Eixo* Grafico::getEixoY(){
    return y;
}

Serie* Grafico::getSerie(){
    return serie;
}

void Grafico::desenhar(){
    int i;

    t = new Tela;
    t->setEixoX(x->getTitulo(), x->getMinimo(), x->getMaximo());
    t->setEixoY(y->getTitulo(), y->getMinimo(), y->getMaximo());

    for(i = 0; i < serie->Quantidade; i++){

        t->plotar(serie->getNome(), serie->Pontos[i]->getX() , serie->Pontos[i]->getY());

    }

    t->mostrar();
    delete t;
}
