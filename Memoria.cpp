#include "Memoria.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cctype>

using namespace std;

Memoria::Memoria()
{
    memoria.resize(128, vector<unsigned int>(16, 0)); // Inicializa 128 bloques de 16 bytes
}

void Memoria::inicializarDesdeArchivo(const string &archivo)
{
    ifstream entrada(archivo);
    if (!entrada.is_open())
    {
        cerr << "Error para abrir archivo " << archivo << "\n";
    }
    else
    {
        string linea;
        while (getline(entrada, linea))
        {
            cout << "LINEA LEIDA: " << linea << endl;
            if (!linea.empty())
            {
                istringstream stream(linea);
                string direccionStr;
                stream >> direccionStr;

                if (direccionStr.size() >= 2 && direccionStr.back() == ':')
                {
                    // cout << "TAMAÑO DIRECCION " << direccionStr.size() << endl;
                    direccionStr.pop_back(); // quitar ':'
                    bool direccionValida = true;
                    int sizeDirStr = direccionStr.size();
                    for (int i = 0; i < sizeDirStr; i++)
                    {
                        if (!isdigit(direccionStr[i])) // digito hex valido
                        {
                            cout << "NO DIGITO VALIDO " << direccionStr[i] << endl;
                            direccionValida = false;
                        }
                    }

                    /*

                    Aca se verifica que la direccion de memoria sea valido porque la estamos dando en decimal de 0 - 127

                    */

                    int direccion = 0;
                    if (direccionValida)
                    {
                        direccion = stoi(direccionStr, nullptr, 10); // convertir a decimal
                        if (direccion >= 0 && direccion < (int)memoria.size())
                        {
                            int bytesLeidos = 0;
                            while (bytesLeidos < 16)
                            {
                                string byteHex;
                                stream >> byteHex;

                                if (!byteHex.empty())
                                {
                                    bool byteValido = true;
                                    for (size_t j = 0; j < byteHex.size(); j++)
                                    {
                                        if (!isxdigit(byteHex[j]))
                                        {
                                            byteValido = false;
                                        }
                                    }

                                    if (byteValido)
                                    {
                                        int valor = stoi(byteHex, nullptr, 16);
                                        memoria[direccion][bytesLeidos] = static_cast<unsigned char>(valor);
                                        cout << "Escribiendo en direccion " << hex << (direccion + bytesLeidos) << ": "
                                             << hex << (int)memoria[direccion][bytesLeidos] << "\n";
                                    }
                                }

                                bytesLeidos++;
                            }
                        }
                        else
                        {
                            cerr << "Direccion fuera de rango: " << direccionStr << "\n";
                        }
                    }
                    else
                    {
                        cerr << "Dirección invalida: " << direccionStr << "\n";
                    }
                }
                else
                {
                    cerr << "Formato invalido en la linea: " << linea << "\n";
                }
            }
        }

        entrada.close();
        cout << "Memoria inicializada desde " << archivo << "\n";
    }
}
void Memoria::leerBloque(int direccionBase, ostream &salida) const
{
    if (direccionBase < 0 || direccionBase >= static_cast<int>(memoria.size()))
    {
        salida << "Error: Dirección " << direccionBase << " fuera de rango\n";
        return;
    }
    for (const auto &byte : memoria[direccionBase])
    {
        salida << setw(2) << setfill('0') << hex << static_cast<int>(byte) << "\n";
    }
}

void Memoria::escribirBloque(int direccionBase, const vector<unsigned int> &bloque)
{
    memoria[direccionBase / 16] = bloque;
}

int Memoria::getTamanio() const
{
    return memoria.size() * 16;
}

string Memoria::obtenerComoTexto() const
{
}

const vector<unsigned int> Memoria::obtenerBloque(unsigned int direccion) const
{
    cout << "Direccion: " << dec << direccion << endl;
    const vector<unsigned int> bloque(this->memoria[direccion]); // Obtener el bloque de memoria                                  // Mostrar el primer byte del bloque

    cout << "desde funcion obtenerBloque\n";
    cout << "Bloque " << dec << direccion << ": "; // Mostrar la dirección del bloque

    for (int i = 0; i < 16; i++)
    {
        cout << hex << setw(2) << setfill('0') << static_cast<int>(bloque[i]) << " "; // Mostrar en formato hexadecimal
    }
    cout << dec << endl;

    return bloque; // Retornar el bloque
}