#ifndef CONTROLADOR_H
#define CONTROLADOR_H

#include "Cache.h"
#include "Memoria.h"

class Controlador
{
private:
    Cache cache;
    Memoria memoria;

public:
    Controlador();
    bool procesarOperacion(int, bool, unsigned char dato = 0); // true si hubo miss
    void manageMemory(string &);                               // inicializar
    void guardarCacheEnArchivo(const string &);
    void mostrarResultados();
    void exportarResultados(const string &);
    void imprimirMemoria() const;

    int getConteoAccesos() const;
    int getConteoMisses() const;
    double calcularTasaMiss() const;
};

#endif