#include "Controlador.h"
#include <iostream>
#include <fstream>

Controlador::Controlador() {}

void Controlador::manageMemory(string &archivo)
{
    memoria.inicializarDesdeArchivo(archivo);
}

void Controlador::guardarCacheEnArchivo(const string &nombreArchivo)
{
    cache.guardarEnArchivo(nombreArchivo);
}

/*bool Controlador::procesarOperacion(int direccion, bool esEscritura, unsigned char dato)
{

    if (esEscritura)
    {
        cout << "Escribiendo en la memoria...\n";
        bool res = cache.escribir(direccion, dato);
        return res; // true si hubo miss
    }
    else
    {
        cout << "Leyendo de la memoria cache...\n";
        bool res2 = cache.leer(direccion);
        cout << "Dato encontrado: " << res2 << endl;
        if (res2)
        {
            cout << "Cache hit!\n";
        }
        else
        {
            cout << "Leyendo de la memoria DRAM...\n";
            // Simular lectura de memoria
            vector<unsigned char> data = this->memoria.obtenerBloque(direccion / 16); // Leer bloque de memoria
            this->cache.subirBloque(data, direccion / 16);                            // Subir bloque a cache
            cout << "Cache miss!\n";
        }
        return res2; // true si hubo miss
    }
}*/

bool Controlador::procesarOperacion(int direccion, bool esEscritura, unsigned char dato)
{
    if (esEscritura)
    {
        cout << "Escribiendo en la memoria...\n";
        bool esHit = cache.escribir(direccion, dato);

        if (esHit)
        {
            // Leer el bloque correspondiente desde la memoria principal
            vector<unsigned int> bloque = memoria.obtenerBloque(direccion / 16);

            cache.subirBloque(bloque, direccion / 16); // Subir bloque a la caché

            //
        }

        return esHit; // Retornar si hubo miss
    }
    else
    {
        cout << "Leyendo de la memoria cache...\n";
        bool esHit = cache.leer(direccion);

        if (!esHit)
        {
            cout << "Leyendo de la memoria DRAM...\n";
            // Leer el bloque correspondiente desde la memoria principal
            vector<unsigned int> bloque = memoria.obtenerBloque(direccion / 16);

            cout << "En funcion controlador, bloque leido: " << endl;

            for (unsigned int dato : bloque)
            {
                cout << hex << static_cast<int>(dato) << " ";
            }
            cout << endl;
            // Subir el bloque a la caché
            cout << "Direccion a subir: " << direccion << endl;
            cache.subirBloque(bloque, direccion / 16);
        }

        return esHit; // Retornar si hubo hit
    }
}

void Controlador::mostrarResultados()
{
    cout << "Resultados:\n";
    cout << "Total accesos: " << cache.getConteoAccesos() << "\n";
    cout << "Total misses: " << cache.getConteoMisses() << "\n";
    cout << "Tasa misses: " << cache.calcularTasaMiss() << "%\n";
}

void Controlador::exportarResultados(const string &archivo)
{
    ofstream salida(archivo);
    if (salida.is_open())
    {
        salida << "Resultados:\n";
        salida << "Total accesos: " << cache.getConteoAccesos() << "\n";
        salida << "Total misses: " << cache.getConteoMisses() << "\n";
        salida << "Tasa misses: " << cache.calcularTasaMiss() << "%\n";
        salida.close();
        cout << "Resultados exportados a " << archivo << "\n";
    }
    else
    {
        cerr << "Error para abrir archivo " << archivo << " para exportar resultados.\n";
    }
}

void Controlador::imprimirMemoria() const
{
    ofstream archivo("memoria.txt");
    if (!archivo.is_open())
    {
        cerr << "No se pudo abrir memoria.txt\n";
        return;
    }

    archivo << "Contenido de la Memoria:\n";

    for (long long int i = 0; i < 2032; i++)
    {
        memoria.leerDato(i, archivo); // imprime en archivo
    }

    archivo.close();
}

int Controlador::getConteoAccesos() const
{
    return cache.getConteoAccesos();
}

int Controlador::getConteoMisses() const
{
    return cache.getConteoMisses();
}

double Controlador::calcularTasaMiss() const
{
    return cache.calcularTasaMiss();
}
