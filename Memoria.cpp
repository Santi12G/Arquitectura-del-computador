#include "Memoria.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cctype>

using namespace std;

Memoria::Memoria()
{
    memoria.resize(2032); // Inicializa 2032 datos de 1 byte (8 bits) cada uno
}

/*void Memoria::inicializarDesdeArchivo(const string &archivo)
{
    ifstream entrada(archivo);
    if (!entrada.is_open())
    {
        cerr << "Error para abrir archivo " << archivo << "\n";
    }
    else
    {
        string dato;
        while (getline(entrada, dato))
        {
            cout << "dato LEIDA: " << dato << endl;
            if (!dato.empty())
            {
                istringstream stream(dato);
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



                    Aca se verifica que la direccion de memoria sea valido porque la estamos dando en decimal de 0 - 127



                    int direccion = 0;
                    if (direccionValida)
                    {
                        direccion = stoi(direccionStr, nullptr, 10); // convertir a decimal
                        if (direccion >= 0 && direccion < (int)memoria.size())
                        {
                            int bytesLeidos = 0;
                            while (bytesLeidos < 16)
                            {
                                string dato;
                                stream >> dato;

                                if (!dato.empty())
                                {
                                    bool byteValido = true;
                                    for (size_t j = 0; j < dato.size(); j++)
                                    {
                                        if (!isxdigit(dato[j]))
                                        {
                                            byteValido = false;
                                        }
                                    }

                                    if (byteValido)
                                    {
                                        int valor = stoi(dato, nullptr, 16);
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
                    cerr << "Formato invalido en la dato: " << dato << "\n";
                }
            }
        }

        entrada.close();
        cout << "Memoria inicializada desde " << archivo << "\n";
    }
}*/

void Memoria::inicializarDesdeArchivo(const string &archivo)
{
    ifstream entrada(archivo);
    if (!entrada.is_open())
    {
        cerr << "Error al abrir archivo " << archivo << "\n";
        return;
    }

    string linea;
    int direccionBase = 0;

    while (getline(entrada, linea))
    {
        // cout << "Linea leida: " << linea << endl; activar linea para debug

        if (linea.empty())
        {
            cerr << "Linea vacia encontrada, se ignora\n";
        }
        else
        {
            istringstream stream(linea);
            string token;
            stream >> token;

            // Eliminar prefijo "0x" si existe
            if (token.size() >= 2 && token[0] == '0' && (token[1] == 'x' || token[1] == 'X'))
            {
                token = token.substr(2);
            }

            bool byteValido = true;
            for (char caracter : token)
            {
                if (!isxdigit(caracter))
                {
                    byteValido = false;
                }
            }

            if (byteValido)
            {
                int valor = stoi(token, nullptr, 16);
                if (direccionBase >= static_cast<int>(memoria.size()))
                {
                    cerr << "Direccion fuera de rango: " << direccionBase << "\n";
                }
                else
                {
                    memoria[direccionBase] = static_cast<unsigned int>(valor);
                    // cout << "Escribiendo en direccion " << hex << direccionBase << ": "
                    //      << hex << memoria[direccionBase] << "\n"; activar linea para debug
                }
                direccionBase++;
            }
            else
            {
                cerr << "Dato inválido (no hexadecimal): " << token << "\n";
            }
        }
    }

    entrada.close();
    cout << "Memoria inicializada desde " << archivo << "\n";
}

void Memoria::leerDato(long long int direccionBase, ostream &salida) const
{
    if (direccionBase < 0 || direccionBase >= static_cast<int>(memoria.size()))
    {
        salida << "Error: Dirección " << direccionBase << " fuera de rango\n";
        return;
    }
    // cout << "IMPRIMIENDO DATO EN DIRECCION " << direccionBase << endl;
    // cout << "DATO EN DIRECCION " << *(memoria.begin() + direccionBase) << ": ";
    auto it = memoria.begin() + direccionBase;
    salida << setw(2) << setfill('0') << hex << static_cast<int>(*it) << "\n";
}

void Memoria::escribirBloque(int direccionBase, const vector<unsigned int> &bloque)
{
    // memoria[direccionBase / 16] = bloque;
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
    cout << "OBTENIENDO BLOQUE FUNCION " << dec << direccion << endl;
    vector<unsigned int> res; // Crear un bloque de 16 bytes
    int direccionBloque = direccion;
    int direccionInicio = direccionBloque * 16; // Calcular la dirección de inicio del bloque
    cout << "Direccion bloque: " << dec << direccionBloque << endl;
    cout << "Direccion inicio: " << dec << direccionInicio << endl;
    cout << "Direccion fin: " << dec << direccionInicio + 16 << endl;
    for (auto it = memoria.begin() + direccionInicio; it != memoria.begin() + direccionInicio + 16; it++)
    {
        res.push_back(*it);                          // Copiar el bloque de memoria
        cout << hex << static_cast<int>(*it) << " "; // Mostrar en formato hexadecimal
    }

    cout << "\ndesde funcion obtenerBloque\n";
    cout << "Bloque " << dec << direccion << ": "; // Mostrar la dirección del bloque
    /*
    for (int i = 0; i < 16; i++)
    {
        cout << hex << setw(2) << setfill('0') << static_cast<int>(bloque[i]) << " "; // Mostrar en formato hexadecimal
    }
    cout << dec << endl;*/

    return res; // Retornar el bloque
}