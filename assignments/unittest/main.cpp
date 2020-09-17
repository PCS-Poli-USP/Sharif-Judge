/**
 * Main file to be used when developing
 * the PCS Judge Test Case.
 */
#include <iostream>

#include <sstream>
#include <iterator>
#include <fstream>
#include <algorithm>
#include "Test.h"

using namespace std;

bool test(string test) {
  string printedName = test;
  replace(printedName.begin(), printedName.end(), '$', ':');
  replace(printedName.begin(), printedName.end(), '_', ' ');
  cout << "Test case: " << printedName << endl;

  stringstream buffer;
  streambuf* old = cout.rdbuf (buffer.rdbuf() );

  Test* isolated = Test::getTest(test);
  bool correct = false;

  try {
     correct = isolated->test();
     delete isolated;
  } catch (...) {
    cout << MSG_NOK;
    delete isolated;
  }

  std::cout.rdbuf (old);

  if (!correct) {
    cout << MSG_NOK;
  } else {
    cout << MSG_OK;
  }

  cout << endl;
  return correct;
}

void executeTests() {
  unsigned int i;
  int corrects = 0;

  vector<string> names = Test::getNames();
  for (i = 0; i < names.size(); i++) {
    if (test(names[i])) corrects++;
  }

  cout << "RESULT: " << (100 * ((double) corrects / i));
}

int main() {
  executeTests();
}
