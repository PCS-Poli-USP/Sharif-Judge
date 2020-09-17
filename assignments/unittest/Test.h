#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <map>
#include <vector>

#define MSG_OK "ACCEPTED"
#define MSG_NOK "WRONG"

#define TEST_CASE(testName) \
    class testName##Test : public Test { \
        public: \
            testName##Test() {} \
            inline bool test(); \
    }; \
    class testName##Factory : public TestFactory { \
    public: \
        testName##Factory() { \
            Test::registerTest(#testName, this); \
        } \
        virtual Test *create() { \
            return new testName##Test(); \
        } \
    }; \
    static testName##Factory global_##testName##Factory; \
    bool testName##Test::test()
using namespace std;

class Test;

class TestFactory {
public:
    virtual Test *create() = 0;
};

class Test {
public:
  virtual ~Test();
	virtual bool test() = 0;
	static void registerTest(const std::string& name, TestFactory* t);
	static Test* getTest(const std::string& name);
	/**
	 * Obtains the test names in order.
	 */
	static const std::vector<std::string> getNames();
private:
  // Oddly they must be pointers...
  static std::map<std::string, TestFactory*>* tests;
  static std::vector<std::string>* names;
};

#define EPSILON 0.00001
bool nearlyEqual (float a, float b, float epsilon);
bool nearlyEqual (float a, float b);

#endif // TEST_H
