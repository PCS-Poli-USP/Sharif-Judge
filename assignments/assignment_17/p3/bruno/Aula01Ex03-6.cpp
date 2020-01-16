#include <cmath>

double distanciaDoPontoDeMenorOrdenadaAOrigem(double abscissas[], double ordenadas[], int quantidadeDePontos) {
    //*
    int index = 0;
    double menor = ordenadas[0];

    for (int i = 1; i < quantidadeDePontos; i++) {
        if (ordenadas[i] < menor) {
            menor = ordenadas[i];
            index = i;
        }
    }

    return sqrt(abscissas[index]*abscissas[index] + ordenadas[index]*ordenadas[index]);
    //*
}
