#include <cmath>
#include <limits>

#include "Teste.h"

map<string, TesteFactory*> *Teste::testes;
vector<string>* Teste::nomes;

void Teste::registrar (const string& nome, TesteFactory* t) {
	if (!Teste::testes) {
			Teste::testes = new map<string, TesteFactory*>();
			Teste::nomes = new vector<string>();
	}
	Teste::testes->insert(pair<string, TesteFactory*>(nome, t));
	Teste::nomes->push_back(nome);
}

Teste* Teste::getTeste (const string& nome) {
    map<string, TesteFactory*>::iterator it;
    it = Teste::testes->find(nome);
    return it != Teste::testes->end() ? it->second->criar() : NULL;
}

const vector<string> Teste::getNomesEmOrdem() {
	return *nomes;
}

bool nearlyEqual (float a, float b, float epsilon) {
  const float absA = abs (a);
  const float absB = abs (b);
  const float diff = abs (a - b);

  if (a == b) { // shortcut, handles infinities
    return true;
  } else if (a == 0 || b == 0
             || diff < std::numeric_limits<float>::min() ) {
    // a or b is zero or both are extremely close to it
    // relative error is less meaningful here
    return diff < (epsilon *
                   std::numeric_limits<float>::min() );
  } else { // use relative error
    return diff / min ( (absA + absB),
                        std::numeric_limits<float>::max() ) < epsilon;
  }
}

bool nearlyEqual (float a, float b) {
  return nearlyEqual (a, b, EPSILON);
}
