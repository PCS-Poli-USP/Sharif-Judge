#include "Teste.h"
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char const *argv[]) {

    cout << "Metodo de teste: ";

    stringstream buffer;
    streambuf* old = cout.rdbuf (buffer.rdbuf() );

	string testeSelecionado;
	cin >> testeSelecionado;

	Teste* isolado = Teste::getTeste(testeSelecionado);

	bool resultadoTeste = isolado->testar();

    std::cout.rdbuf (old);

	if (!resultadoTeste) {
		cout << MSG_NOK;
	}
	else {
        cout << MSG_OK;
	}

	return 0;
}
