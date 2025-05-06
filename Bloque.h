#ifndef BLOQUE_H
#define BLOQUE_H

#include <vector>

using namespace std;

class Bloque
{
private:
    int etiqueta;
    vector<unsigned int> datos;
    int contadorLRU;
    bool valido;

public:
    Bloque();

    void restartLRU();
    int getEtiqueta() const;
    void setEtiqueta(int);

    int getContadorLRU() const;
    void setContadorLRU(int);

    bool esValido() const;
    void setValido(bool);

    const vector<unsigned int> &getDatos() const; // sin modificar
    vector<unsigned int> &getDatos();             // con modificar
    void setDatos(const vector<unsigned int> &);  //  actualizar todos los datos del bloque de una sola vez, como al cargar un bloque completo desde la memoria principal.
    void setDato(int indice, unsigned int valor); // modificar un solo byte dentro del bloque, como al escribir un dato en la caché.
    unsigned char getDato(int) const;             // Obtener dato en una posicion especifica
};

#endif