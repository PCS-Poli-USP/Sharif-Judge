#include <iostream>

using namespace std;

int quadranteDoPonto(double x, double y) {
    //*
    if (x > 0 && y > 0)
        return 1;
    if (x < 0 && y > 0)
        return 2;
    if (x < 0 && y < 0)
        return 3;
    if (x > 0 && y < 0)
        return 4;
    if (x == 0 || y == 0) //*/
        return 0;
}

