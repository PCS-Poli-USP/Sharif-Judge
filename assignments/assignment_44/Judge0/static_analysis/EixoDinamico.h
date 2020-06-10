#ifndef EIXODINAMICO_H
#define EIXODINAMICO_H
#include "Eixo.h"
#include "Serie.h"

class EixoDinamico : public Eixo
{
    public:
        EixoDinamico(double minimoPadrao, double maximoPadrao,
                     Serie* base, bool orientacaoHorizontal);
        virtual ~EixoDinamico();

    protected:
        double minimoPadrao;
        double maximoPadrao;
        bool orientaHorizontal;
        string Nome;
        string NomeDoCanalX;
        string NomeDoCanalY;
        Serie* base = new Serie(Nome, NomeDoCanalX, NomeDoCanalY);
    private:
};

#endif // EIXODINAMICO_H
