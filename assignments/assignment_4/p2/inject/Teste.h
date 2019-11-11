#ifndef TESTE_H
#define TESTE_H

#include <iostream>
#include <map>
#include <vector>

#define MSG_OK "ACCEPTED"
#define MSG_NOK "WRONG"

#define REGISTRAR_TESTE(regTeste) \
    class regTeste##Factory : public TesteFactory { \
    public: \
        regTeste##Factory() { \
            Teste::registrar(#regTeste, this); \
        } \
        virtual Teste *criar() { \
            return new regTeste(); \
        } \
    }; \
    static regTeste##Factory global_##regTeste##Factory;

#define CASO_DE_TESTE(nomeDoTeste) \
    class nomeDoTeste##Teste : public Teste { \
        public: \
            nomeDoTeste##Teste() {} \
            inline bool testar(); \
    }; \
    class nomeDoTeste##Factory : public TesteFactory { \
    public: \
        nomeDoTeste##Factory() { \
            Teste::registrar(#nomeDoTeste, this); \
        } \
        virtual Teste *criar() { \
            return new nomeDoTeste##Teste(); \
        } \
    }; \
    static nomeDoTeste##Factory global_##nomeDoTeste##Factory; \
    bool nomeDoTeste##Teste::testar()

using namespace std;

class Teste;

class TesteFactory
{
public:
    virtual Teste *criar() = 0;
};

class Teste {
public:
    // Colocar outros parametros se necessario
	virtual bool testar() = 0;
	static void registrar (const string& nome, TesteFactory* t);
	static Teste* getTeste(const string& nome);
	static const vector<string> getNomesEmOrdem();
private:
  static map<string, TesteFactory*> *testes;
  static vector<string>* nomes;
};

#define EPSILON 0.00001
bool nearlyEqual (float a, float b, float epsilon);
bool nearlyEqual (float a, float b);

#endif // TESTE_H
