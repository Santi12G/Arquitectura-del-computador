#ifndef PROCESADOR_H
#define PROCESADOR_H

#include "Controlador.h"
#include <string>

class Procesador
{
private:
    Controlador controlador;

public:
    void guardarCacheEnArchivo(const string &);
    void inicializar();
    void inicializarMemoria(string &);
    void leerArchivoOperaciones(const string &);

    void mostrarResultados();
    void exportarResultados(const string &);

    void imprimirCache() const;
    void imprimirMemoria() const;
};

#endif