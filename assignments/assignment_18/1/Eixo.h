#ifndef EIXO_H
#define EIXO_H
#include "string"
using namespace std;

class Eixo
{
    public:
        Eixo( string titulo, double minimo, double maximo);
        virtual ~Eixo();

        virtual string getTitulo();
        virtual double getMinimo();
        virtual double getMaximo();

    protected:
        string titulo;
        double minimo;
        double maximo;
    private:
};

#endif // EIXO_H
