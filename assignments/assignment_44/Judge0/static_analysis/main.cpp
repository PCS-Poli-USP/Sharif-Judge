#include <iostream>
#include <string>

#include "Serie.h"
#include "Ponto.h"
#include "InterfaceSerial.h"
#include "SerieTemporal.h"
#include "Eixo.h"
#include "EixoDinamico.h"
#include "Grafico.h"

#define COMM "\\\\.\\COM3"

using namespace std;

int main() {

    Grafico* graphic;
    Eixo *X;
    Eixo *Y;
    char choose;
    string title;
    double minimo, maximo;
    Serie *ser1;
    SerieTemporal *ser2;
    int i = 0;
    InterfaceSerial* is = new InterfaceSerial(COMM);
    is->inicializar();

    cout << "Aperte o botao reset da placa." << endl;
  // Obtem o nome e o atribui a serie ser
    string nomeSerie;
    cout << "Informe o nome da serie: ";
    cin >> nomeSerie;
    cout << endl;

  // Obtem  os canais escolhidos
    string canalX;
    string canalY;
    int quantidadeCanais = is->getQuantidadeDeCanais();
    int escolha;

  //Imprime a lista de canais para X
    cout << "Escolha o Canal X: " << endl;
    cout << "0) Tempo" << endl;
    for(i=0; i < quantidadeCanais; i++){
    cout << i + 1 << ") " << is->getNomeDosCanais()[i] << endl;
    }
    cin >> escolha;

    if(escolha == 0)
        canalX = "Tempo";
    else
        canalX = is->getNomeDosCanais()[escolha - 1];


  //Imprime a lista de canais para Y
    cout << "Escolha o Canal Y: " << endl;
    for(i=0; i < quantidadeCanais; i++){
        cout << i + 1 << ") " << is->getNomeDosCanais()[i] << endl;
    }
    cin >> escolha;
    canalY = is->getNomeDosCanais()[escolha - 1];

  // Obtem o numero de Pontos a adicionar
    int quantidade;
    cout << "Obter quantos pontos? ";
    cin >> quantidade;
    cout << endl;

    ser1 = new Serie(nomeSerie, canalX, canalY);
    ser2 = new SerieTemporal(nomeSerie, canalY);

    if(canalX != "Tempo"){
        cout << "Obtendo os pontos" << endl;
        delete ser2;
        for (i = 0; i < quantidade; i++){
            is->atualizar();
            ser1->adicionar(is->getValor(canalX), is->getValor(canalY));
      }
    }

    else{
        cout << "Obtendo os pontos" << endl;
        delete ser1;
        for (i = 0; i < quantidade; i++) {
            is->atualizar();
            ser2->adicionar(is->getValor(canalY));
        }
    }

    cout << "Gerando o grafico" << endl;
    cout << "O eixo X e estatico ou dinamico (e/d):";
    cin >>  choose;
    cout << endl;

    if(choose == 'e'){
        cout << "Informe o titulo:";
        cin >> title;
        cout << endl << "Valor minimo:";
        cin >> minimo;
        cout << endl << "Valor maximo:";
        cin >> maximo;
        cout << endl;

        X = new Eixo(title, minimo, maximo);
    }
    else{
        cout << "Valor minimo padrao:";
        cin >> minimo;
        cout << endl << "Valor maximo padrao:";
        cin >> maximo;
        cout << endl;

        if(canalX != "Tempo"){
            X = new EixoDinamico(minimo, maximo, ser1, true);
        }
        else{
            X = new EixoDinamico(minimo, maximo, ser2, true);
        }
    }

    cout << "O eixo Y e estatico ou dinamico (e/d):";
    cin >>  choose;
    cout << endl;

    if(choose == 'e'){
        cout << "Informe o titulo:";
        cin >> title;
        cout << endl << "Valor minimo:";
        cin >> minimo;
        cout << endl << "Valor maximo:";
        cin >> maximo;
        cout << endl;

        Y = new Eixo(title, minimo, maximo);
    }
    else{
        cout << "Valor minimo padrao:";
        cin >> minimo;
        cout << endl << "Valor maximo padrao:";
        cin >> maximo;
        cout << endl;

        if(canalX != "Tempo"){
            Y = new EixoDinamico(minimo, maximo, ser1, false);
        }
        else{
            Y = new EixoDinamico(minimo, maximo, ser2, false);
        }
    }

    if(canalX != "Tempo")
        graphic = new Grafico(X, Y, ser1);
    else
        graphic = new Grafico(X, Y, ser2);

    graphic->desenhar();



    return 0;

}
