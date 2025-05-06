#include "Procesador.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

void Procesador::inicializar()
{
    srand((unsigned)time(0));
}

void Procesador::guardarCacheEnArchivo(const string &nombreArchivo)
{
    controlador.guardarCacheEnArchivo(nombreArchivo);
}

void Procesador::inicializarMemoria(string &archivo)
{
    controlador.manageMemory(archivo); // Delegar al controlador
}

// Función auxiliar simple para convertir string a número (hex/decimal)
unsigned int stringANumero(const string &str)
{
    unsigned int resultado = 0;
    int base = 10;
    int inicio = 0;

    // Detectar si es hexadecimal
    if (str.size() > 2 && str[0] == '0' && tolower(str[1]) == 'x')
    {
        base = 16;
        inicio = 2;
    }

    // Conversión manual
    for (size_t i = inicio; i < str.size(); i++)
    {
        char c = tolower(str[i]);
        if (c >= '0' && c <= '9')
        {
            resultado = resultado * base + (c - '0');
        }
        else if (base == 16 && c >= 'a' && c <= 'f')
        {
            resultado = resultado * base + (c - 'a' + 10);
        }
        else
        {
            return 0; // Retorno simple en caso de error
        }
    }
    return resultado;
}

void Procesador::leerArchivoOperaciones(const string &archivo)
{
    ifstream entrada(archivo);
    bool todoCorrecto = true;
    cout << "Leyendo archivo de operaciones: " << archivo << endl;
    cout << "----------------------------------------\n";
    // cout << "BOOL IS OPEN: " << entrada.is_open() << endl;
    if (entrada.is_open())
    {
        string linea_limpia;
        while (getline(entrada, linea_limpia))
        {
            // cout << "LINEA LEIDA: " << linea_limpia << endl;
            // Limpieza manual de espacios

            if (!linea_limpia.empty())
            {
                istringstream stream(linea_limpia);
                string tipoOperacion, dirStr, datoStr;
                unsigned int direccion = 0, dato = 0;

                stream >> tipoOperacion >> dirStr;
                direccion = stringANumero(dirStr);
                // cout << "TIPO OPERACION: " << tipoOperacion << endl;
                // cout << "DIRECCION: " << dirStr << endl;
                if (tipoOperacion == "WRITE")
                {
                    if (stream >> datoStr)
                    {
                        dato = stringANumero(datoStr);
                        // cout << "DATO: " << datoStr << endl;
                    }
                    else
                    {
                        todoCorrecto = false;
                    }
                }
                else if (tipoOperacion != "READ")
                {
                    todoCorrecto = false;
                }

                if (direccion >= 0 || dirStr == "0")
                {
                    cout << "Direccion: " << dec << direccion << endl;
                    bool esHit = controlador.procesarOperacion(
                        direccion,
                        tipoOperacion == "WRITE",
                        static_cast<unsigned char>(dato));

                    cout << "OP: " << tipoOperacion << "; DIR: 0x" << hex << direccion
                         << ", Hit: " << (esHit ? "Si" : "No")
                         << ", Direccion bloque: " << dec << (direccion / 16) << "\n";
                }
                else
                {
                    todoCorrecto = false;
                }
            }
        }
        entrada.close();
    }
    else
    {
        todoCorrecto = false;
    }

    if (!todoCorrecto)
    {
        cerr << "Hubo errores al procesar el archivo\n";
    }
}

void Procesador::mostrarResultados()
{
    controlador.mostrarResultados();
}

void Procesador::exportarResultados(const string &archivo)
{
    ofstream salida(archivo);
    if (salida.is_open())
    {
        salida << "Resultados:\n";
        salida << "Total accesos: " << controlador.getConteoAccesos() << "\n";
        salida << "Total misses: " << controlador.getConteoMisses() << "\n";
        salida << "Tasa misses: " << controlador.calcularTasaMiss() << "%\n";
        salida.close();
    }
    else
    {
        cerr << "No se pudo abrir el archivo " << archivo << " para exportar resultados.\n";
    }
}

void Procesador::imprimirMemoria() const
{
    controlador.imprimirMemoria();
}