#include "Bloque.h"
#include <iostream>

Bloque::Bloque()
{

    etiqueta = -1;
    contadorLRU = 0;
    valido = false;
    datos.resize(16, -1); // Inicializa el bloque con 16 bytes de datos en 0
}

unsigned char Bloque::getDato(int i) const
{
    return datos[i]; // Obtener dato en una posicion especifica
}

int Bloque::getEtiqueta() const
{
    return etiqueta;
}

void Bloque::setEtiqueta(int e)
{
    int et = e / 16;
    etiqueta = et;
}

int Bloque::getContadorLRU() const
{
    return contadorLRU;
}

void Bloque::setContadorLRU(int c)
{
    contadorLRU = contadorLRU + 1; // Incrementa el contador LRU
}

void Bloque::restartLRU()
{
    contadorLRU = 1; // Reinicia el contador LRU
}

bool Bloque::esValido() const
{
    return valido;
}
void Bloque::setValido(bool v)
{
    valido = v;
}

const vector<unsigned int> &Bloque::getDatos() const
{
    return datos;
}
vector<unsigned int> &Bloque::getDatos()
{
    return datos;
}

void Bloque::setDatos(const vector<unsigned int> &d)
{
    for (int i = 0; i < static_cast<int>(datos.size()); i++)
    {
        datos[i] = d[i]; // Actualiza todos los datos del bloque
    }
}

void Bloque::setDato(int indice, unsigned int valor)
{
    cout << "indice: " << indice << endl;

    if (indice >= 0 && indice < static_cast<int>(datos.size()))
    {
        cout << "FIRST REVISION: " << endl;
        cout << "datos [indice] " << datos[indice] << endl;
        datos[indice] = valor; // Actualiza el dato en la posición especificada
    }
    else
    {
        std::cerr << "Error: Índice fuera de rango en setDato.\n";
    }
}