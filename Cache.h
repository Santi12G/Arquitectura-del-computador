#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include "Bloque.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <deque>

/**
 * La cache tendra un direction bus de 11 bits, lo que significa que puede direccionar
 *  2048 direcciones diferentes.
 *
 */

enum EstadoCache
{
    VACIA, // Ianctiva
    READ,
    WRITE,
    MISS,
    HIT,
    REEMPLAZO // Reemplazo de bloque
};

class Cache
{
private:
    vector<Bloque> bloques;
    int conteoAccesos;
    int conteoMisses;
    EstadoCache estadoActual;
    int bloqueLibre;

public:
    Cache();
    deque<unsigned int> lruPolicy; // Vector para almacenar la politica LRU
    void leerLruPolicyQueue();
    bool leer(unsigned int);          // Lee un dato de la caché
    bool escribir(int, unsigned int); // Escribe un dato en la caché
    void guardarEnArchivo(const string &);
    const Bloque &getBloque(int) const; // solo lectura
    int getBloqueLibre() const;         // solo lectura
    int manageBloqueLibre();            // Maneja el bloque libre
    float calcularTasaMiss() const;
    int getConteoAccesos() const;
    int getConteoMisses() const;

    int buscarBloque(int etiqueta);                                        // Busca bloque en cache por etiqueta
    int encontrarReemplazo();                                              // Indice del bloque a reemplazar
    void actualizarLRU(int indice);                                        // Actualiza contador LRU
    void subirBloque(vector<unsigned int> &bloque, int etiqueta);          // Subir bloque a cache
    void setDato(int direccionBloque, int offset, unsigned int nuevoDato); // Modificar un dato en la cache
    void setEstado(EstadoCache estado);
};

#endif