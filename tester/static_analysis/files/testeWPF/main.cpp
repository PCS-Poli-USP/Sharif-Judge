#include <iostream>
#include "MyClass.h"

using namespace std;

int main()
{
    MyClass* myc = new MyClass();
    myc->usedMethod();

    cout << "do stuff" << endl;

    delete myc;
    return 1;
}
