#include "Test.h"
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char const *argv[]) {
    stringstream buffer;
    streambuf* old = cout.rdbuf (buffer.rdbuf() );

	string test;
	cin >> test;

	Test* isolated = Test::getTest(test);

	bool correct = isolated->test();

    std::cout.rdbuf (old);

	if (!correct) {
		cout << MSG_NOK;
	}
	else {
        cout << MSG_OK;
	}

	return 0;
}