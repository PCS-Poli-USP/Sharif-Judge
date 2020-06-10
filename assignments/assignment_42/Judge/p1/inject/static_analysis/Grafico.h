#ifndef GRAFICO_H
#define GRAFICO_H

#include "Tela.h"
#include "Serie.h"
#include "Eixo.h"

class Grafico
{
    public:
        Grafico(Eixo* x, Eixo* y, Serie* serie);
        virtual ~Grafico();
        Eixo* getEixoX();
        Eixo* getEixoY();
        Serie* getSerie();

        void desenhar();

    protected:

    private:
        double maximo;
        double minimo;
        string titulo;
        string Nome;
        string NomeDoCanalX;
        string NomeDoCanalY;
        Eixo* x;
        Eixo* y;
        Serie* serie = new Serie(Nome, NomeDoCanalX, NomeDoCanalY);
        Tela* t;
};

#endif // GRAFICO_H
