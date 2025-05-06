#ifndef MEMORIA_H
#define MEMORIA_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace std;

class Memoria
{
private:
    vector<vector<unsigned int>> memoria;

public:
    Memoria();
    void inicializarDesdeArchivo(const string &);

    void leerBloque(int, ostream &) const;
    void escribirBloque(int, const vector<unsigned int> &);

    int getTamanio() const;
    Memoria &getMemoria();

    string obtenerComoTexto() const;
    const vector<unsigned int> obtenerBloque(unsigned int) const;
};

#endif