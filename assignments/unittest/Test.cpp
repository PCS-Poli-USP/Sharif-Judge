#include <cmath>
#include <limits>

#include "Test.h"

using namespace std;

map<string, TestFactory*> *Test::tests = nullptr;
vector<string>* Test::names = nullptr;

Test::~Test() {
}

void Test::registerTest (const string& name, TestFactory* t) {
	if (!Test::tests) {
	  // Couldn't made it work without a new...
	  Test::tests = new map<string, TestFactory*>();
	  Test::names = new vector<string>();
	}
	Test::names->push_back(name);
	Test::tests->insert(pair<string, TestFactory*>(name, t));
}

Test* Test::getTest(const string& name) {
    map<string, TestFactory*>::iterator it = Test::tests->find(name);
    return it != Test::tests->end() ? it->second->create() : nullptr;
}

const vector<string> Test::getNames() {
	return *names;
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
