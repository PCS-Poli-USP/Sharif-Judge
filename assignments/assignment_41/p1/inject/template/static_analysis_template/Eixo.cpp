#include "Eixo.h"
#include "iostream"
using namespace std;

Eixo::Eixo(string titulo, double minimo, double maximo) :
titulo(titulo), minimo(minimo), maximo(maximo)
{

}

Eixo::~Eixo()
{
   cout << "Eixo destruido" << endl;
}

string Eixo::getTitulo()
{
    return titulo;
}

double Eixo::getMinimo()
{
    return minimo;
}

double Eixo::getMaximo()
{
    return maximo;
}
