#include <sstream>
#include <typeinfo>
#include <regex>

#include "Teste.h"

#include "Ponto.h"
#include "Eixo.h"
#include "EixoDinamico.h"
#include "Grafico.h"
#include "Serie.h"
#include "SerieTemporal.h"
#include "Tela.h"
#include "InterfaceSerial.h"

using namespace std;

CASO_DE_TESTE (Ponto_nao_tem_superclasse) {
  return !is_base_of<Eixo, Ponto>::value &&
         !is_base_of<EixoDinamico, Ponto>::value &&
         !is_base_of<Serie, Ponto>::value &&
         !is_base_of<SerieTemporal, Ponto>::value &&
         !is_base_of<Grafico, Ponto>::value &&
         !is_base_of<Tela, Ponto>::value &&
         !is_base_of<InterfaceSerial, Ponto>::value;
}

CASO_DE_TESTE (Grafico_nao_tem_superclasse) {
  return !is_base_of<Eixo, Grafico>::value &&
         !is_base_of<EixoDinamico, Grafico>::value &&
         !is_base_of<Serie, Grafico>::value &&
         !is_base_of<SerieTemporal, Grafico>::value &&
         !is_base_of<Ponto, Grafico>::value &&
         !is_base_of<Tela, Grafico>::value &&
         !is_base_of<InterfaceSerial, Grafico>::value;

}

CASO_DE_TESTE (Eixo_nao_tem_superclasse) {
  return !is_base_of<Ponto, Eixo>::value &&
         !is_base_of<EixoDinamico, Eixo>::value &&
         !is_base_of<Serie, Eixo>::value &&
         !is_base_of<SerieTemporal, Eixo>::value &&
         !is_base_of<Grafico, Eixo>::value &&
         !is_base_of<Tela, Eixo>::value &&
         !is_base_of<InterfaceSerial, Eixo>::value;
}

CASO_DE_TESTE (Serie_nao_tem_superclasse) {
  return !is_base_of<Eixo, Serie>::value &&
         !is_base_of<EixoDinamico, Serie>::value &&
         !is_base_of<Ponto, Serie>::value &&
         !is_base_of<SerieTemporal, Serie>::value &&
         !is_base_of<Grafico, Serie>::value &&
         !is_base_of<Tela, Serie>::value &&
         !is_base_of<InterfaceSerial, Serie>::value;
}

CASO_DE_TESTE (SerieTemporal_eh_filha_de_Serie_e_somente_dela) {
  return is_base_of<Serie, SerieTemporal>::value &&
         !is_base_of<Eixo, SerieTemporal>::value &&
         !is_base_of<EixoDinamico, SerieTemporal>::value &&
         !is_base_of<Ponto, SerieTemporal>::value &&
         !is_base_of<Grafico, SerieTemporal>::value &&
         !is_base_of<Tela, SerieTemporal>::value &&
         !is_base_of<InterfaceSerial, SerieTemporal>::value;
}

CASO_DE_TESTE (EixoDinamico_eh_filha_de_Eixo_e_somente_dela) {
  return is_base_of<Eixo, EixoDinamico>::value &&
         !is_base_of<Serie, EixoDinamico>::value &&
         !is_base_of<SerieTemporal, EixoDinamico>::value &&
         !is_base_of<Ponto, EixoDinamico>::value &&
         !is_base_of<Grafico, EixoDinamico>::value &&
         !is_base_of<Tela, EixoDinamico>::value &&
         !is_base_of<InterfaceSerial, EixoDinamico>::value;
}

CASO_DE_TESTE (SerieTemporal$_metodo_adicionar_foi_redefinido) {
    // https://stackoverflow.com/questions/40210333/c-typeid-on-class-member-operator-overloads
  return int(typeid(&Serie::adicionar) != typeid(static_cast<void(SerieTemporal::*)(double, double)>(&SerieTemporal::adicionar)));
}

/*CASO_DE_TESTE (SerieTemporal$_metodos_getNome_getNomeDoCanalX_getNomeDoCanalY_getQuantidade_estaVazia_nao_redefinidos) {
  // O posicao tambem poderia...
  return int(typeid(&Serie::getNome) == typeid(&SerieTemporal::getNome)) &&
         int(typeid(&Serie::getNomeDoCanalX) == typeid(&SerieTemporal::getNomeDoCanalX))  &&
         int(typeid(&Serie::getNomeDoCanalY) == typeid(&SerieTemporal::getNomeDoCanalY))  &&
         int(typeid(&Serie::getQuantidade) == typeid(&SerieTemporal::getQuantidade))  &&
         int(typeid(&Serie::estaVazia) == typeid(&SerieTemporal::estaVazia));
}

CASO_DE_TESTE (EixoDinamico$_metodos_getMinimo_e_getMaximo_redefinidos) {
  return int(typeid(&Eixo::getMinimo) != typeid(&EixoDinamico::getMinimo)) &&
         int(typeid(&Eixo::getMinimo) != typeid(&EixoDinamico::getMaximo));
}


CASO_DE_TESTE (EixoDinamico$_metodo_getTitulo_nao_redefinido) {
  return int(typeid(&Eixo::getTitulo) == typeid(&EixoDinamico::getTitulo));
}*/
