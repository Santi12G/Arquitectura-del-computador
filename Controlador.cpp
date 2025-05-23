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
    bool esHit = false; // Variable para almacenar si hubo hit o no
    if (esEscritura)
    {
        cout << "Escribiendo en la memoria...\n";
        esHit = cache.escribir(direccion, dato);

        if (esHit)
        {
            // cout << "PREVIUS OF SET DATO\n";
            cache.setDato(direccion / 16, direccion % 16, dato); // Escribir dato en la caché
            cache.actualizarLRU(direccion);                      // Actualizar LRU si hubo hit
            // Leer el bloque correspondiente desde la memoria principal
            // vector<unsigned int> bloque = memoria.obtenerBloque(direccion / 16);

            // cache.subirBloque(bloque, direccion / 16); // Subir bloque a la caché

            //
        }
        else
        {
            cout << "LEYENDO RAM PARA ESCRITURA...\n";
            cout << "Leyendo de la memoria DRAM...\n";
            // Leer el bloque correspondiente desde la memoria principal
            vector<unsigned int> bloque = memoria.obtenerBloque(direccion / 16);
            cout << "Datos leidos de la memoria DRAM: " << endl;
            for (unsigned int dato : bloque)
            {
                cout << hex << static_cast<int>(dato) << " ";
            }
            cout << endl;
            string op = "WRITE"; // Operacion de escritura

            cache.subirBloque(bloque, direccion);                // Subir bloque a la caché
            cache.setDato(direccion / 16, direccion % 16, dato); // Escribir dato en la caché
        }
        memoria.modificarDato(direccion, dato); // Escribir dato en la memoria principal
    }
    else
    {
        cout << "Leyendo de la memoria cache...\n";
        cout << "direccion directa de DRAM: " << static_cast<int>(direccion) << endl;
        esHit = cache.leer(direccion);

        if (!esHit)
        {
            cout << "Leyendo de la memoria DRAM...\n";
            // Leer el bloque correspondiente desde la memoria principal
            vector<unsigned int> bloque = memoria.obtenerBloque(direccion / 16);
            cout << "Datos leidos de la memoria DRAM: " << endl;
            for (unsigned int dato : bloque)
            {
                cout << hex << static_cast<int>(dato) << " ";
            }
            cout << endl;

            /*
            cout << "En funcion controlador, bloque leido: " << endl;

            for (unsigned int dato : bloque)
            {
                cout << hex << static_cast<int>(dato) << " ";
            }
            cout << endl;

            // Subir el bloque a la caché
            cout << "Direccion a subir: " << direccion << endl;*/
            string op = "READ"; // Operacion de lectura
            cache.subirBloque(bloque, direccion);
        }
        else
        {
            cache.actualizarLRU(direccion); // Actualizar LRU si hubo hit
        }
        cout << "POST POLITICA LRU: " << cache.lruPolicy.front() << endl;
        cout << "TAMANIO POLITICA LRU: " << dec << static_cast<int>(cache.lruPolicy.size()) << endl;
    }
    return esHit; // Retornar si hubo hit
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
