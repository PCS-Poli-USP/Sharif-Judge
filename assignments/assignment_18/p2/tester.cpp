#include "inject/Teste.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/*******************************************************

Este programa deve ser utilizado em conjunto com o caso
de teste quando a compara��o de sa�das n�o � utilizada.
Ele compara a mensagem de sa�da gerada pelo teste e retorna
0 quando o teste est� correto e 1 quando o teste est� errado.
Utilizando este programa n�o � necess�rio criar os arquivos
de sa�da "outputN.txt" no pacote de testes do Sharif-Judge.

*******************************************************/

int main(int argc, char const *argv[])
{
	string test_in(argv[1]);    /* This stream reads from test's input file   */
	string test_out(argv[2]);   /* This stream reads from test's output file  */
	ifstream user_out(argv[3]); /* This stream reads from user's output file  */

	string user_out_text((std::istreambuf_iterator<char>(user_out)), std::istreambuf_iterator<char>());

	return user_out_text.find(MSG_OK) != string::npos ? 0 : 1;
}
