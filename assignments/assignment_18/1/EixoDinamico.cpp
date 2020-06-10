#include "EixoDinamico.h"
#include "Serie.h"
#include "iostream"

EixoDinamico::EixoDinamico(double minimoPadrao, double maximoPadrao,
                           Serie* base, bool orientacaoHorizontal):
                               Eixo (base->getNome(), minimoPadrao, maximoPadrao)
{
    this->orientaHorizontal = orientacaoHorizontal;

    if (this->orientaHorizontal == true)
    {
        this->titulo = base->getNomeDoCanalX();
        if (base->getQuantidade() >= 2 &&
            (base->Pontos[0]->getX() !=  base->Pontos[1]->getX()) )
        {
            this->maximo = base->getLimiteSuperior()->getX();
            this->minimo = base->getLimiteInferior()->getX();
        }
        else
        {
            this->maximo = maximoPadrao;
            this->minimo = minimoPadrao;
        }
    }
    else
    {
        this->titulo = base->getNomeDoCanalY();
        if (base->getQuantidade() >= 2 &&
            (base->Pontos[0]->getY() !=  base->Pontos[1]->getY()) )
        {
            this->maximo = base->getLimiteSuperior()->getY();
            this->minimo = base->getLimiteInferior()->getY();
        }
        else
        {
            this->maximo = maximoPadrao;
            this->minimo = minimoPadrao;
        }
    }
}

EixoDinamico::~EixoDinamico()
{
    cout << "EixoDinamico destruido" << endl;
}
