#include "Cache.h"
#include <algorithm>

Cache::Cache()
{
    bloques.resize(32); // Inicializa la cache con 32 bloques
    conteoAccesos = 0;
    conteoMisses = 0;
    estadoActual = VACIA;
    bloqueLibre = 0; // Inicializa el bloque libre
}

void Cache::setEstado(EstadoCache estado)
{
    estadoActual = estado;
}

int Cache::manageBloqueLibre()
{
    if (bloqueLibre <= 32)
    {
        bloqueLibre++;
    }
    if (bloqueLibre == 33)
    {
        setEstado(REEMPLAZO);
    }
    if (bloqueLibre == 33 && estadoActual == REEMPLAZO)
    {
        setEstado(REEMPLAZO);
        cout << "Cache llena, reemplazando bloque..." << endl;
        bloqueLibre = encontrarReemplazo();   // Reemplazar el bloque menos usado
        bloques[bloqueLibre].setValido(true); // Marcar como no valido
        bloques[bloqueLibre].restartLRU();    // Reiniciar el contador LRU

        cout << "Reemplazando bloque: " << dec << bloqueLibre << endl;
    }
    else
    {
        cout << "Bloque libre: " << dec << bloqueLibre << endl;
    }
    cout << "\nLRU: " << bloques[bloqueLibre].getContadorLRU() << endl;
    return estadoActual; // Retorna el bloque libre
}

bool Cache::leer(unsigned int direccion)
{
    bool ans = false;
    conteoAccesos++;
    int blockAddress = direccion >> 4; //
    int offset = direccion % 16;       //
    bool validBloque = false;
    int bloqueCache;
    cout << "Offset: " << offset << endl;
    cout << "Direccion bloque: " << hex << static_cast<int>(blockAddress) << endl;
    for (int i = 0; i < 32 && !validBloque; i++)
    {
        if (bloques[i].getEtiqueta() == blockAddress)
        {
            bloqueCache = i;
            validBloque = true;
            cout << "Se encontro el bloque " << bloques[i].getEtiqueta() << " en la memoria cache" << endl;
        }
    }
    if (validBloque)
    {
        // cout << "dato: " << hex << static_cast<int>((bloques[blockAddress]).getDato(offset)) << endl;
        ans = true; // Se encuentra el dato
        setEstado(HIT);
        actualizarLRU(bloqueCache); // Actualiza el contador LRU   --> recordar que hay que implementarlo con un deque para ir metiendo los recientemente usados en la parte de atras
    }
    else
    {
        cout << "No se encuentra el bloque en la cache" << endl;
        setEstado(MISS);
        conteoMisses++;
    }
    // Falta subir el dato a la cache si no se encuentra pero desde la clase controlador

    return ans;
}

void Cache::subirBloque(vector<unsigned int> &bloque, int direccionBloque)
{
    int bloqueReemplazo = 0; // Inicializa el bloque a reemplazar

    for (int i = 0; i < 16; i++)
    {
        cout << hex << static_cast<int>(bloque[i]) << " ";
    }
    int estado = -1;
    if (estadoActual == MISS && bloqueLibre <= 32)
    {
        this->bloques[bloqueLibre].setDatos(bloque);             // Subir bloque a cache
        this->bloques[bloqueLibre].setEtiqueta(direccionBloque); // Etiqueta del bloque
        this->bloques[bloqueLibre].setContadorLRU(0);            // Contador LRU a 0
        this->bloques[bloqueLibre].setValido(true);              // Bloque valido
        estado = manageBloqueLibre();
        cout << "Subiendo bloque a cache: " << direccionBloque << endl;
    }
    if (estado == REEMPLAZO)
    {
        cout << "Cache llena, reemplazando bloque..." << endl;
        bloqueReemplazo = encontrarReemplazo();                // Reemplazar el bloque menos usado
        bloques[bloqueReemplazo].setDatos(bloque);             // Subir bloque a cache
        bloques[bloqueReemplazo].setEtiqueta(direccionBloque); // Etiqueta del bloque
        bloques[bloqueReemplazo].setContadorLRU(0);            // Contador LRU a 0
        bloques[bloqueReemplazo].setValido(true);              // Bloque valido
        bloques[bloqueReemplazo].restartLRU();                 // Reiniciar el contador LRU
        cout << "Subiendo bloque a cache: " << bloqueReemplazo << endl;
        cout << "Etiqueta bloque reemplazo: " << bloques[bloqueReemplazo].getEtiqueta() << endl;
    }
}

bool Cache::escribir(int direccion, unsigned int nuevoD)
{
    bool ans = false;
    conteoAccesos++;
    int blockAddress = direccion >> 4;
    int offset = direccion % 16;
    bool validBloque = false;
    int bloqueCache;
    cout << "Offset: " << offset << endl;
    cout << "Direccion bloque: " << hex << static_cast<int>(blockAddress) << endl;
    for (int i = 0; i < 32 && !validBloque; i++)
    {
        if (bloques[i].getEtiqueta() == blockAddress)
        {
            bloqueCache = i;
            validBloque = true;
            cout << "Se encontro el bloque " << bloques[i].getEtiqueta() << " en la memoria cache" << endl;
        }
    }
    if (validBloque)
    {
        // cout << "Bloque valido: " << validBloque << endl;
        setDato(bloqueCache, offset, nuevoD); // Escribir dato en la cache
        cout << "Escribiendo en la cache: " << hex << static_cast<int>(nuevoD) << endl;
        ans = true; // Se encuentra el dato
        setEstado(HIT);
        // actualizarLRU(bloqueCache); // Actualiza el contador LRU revisar el actualizar LRU
        conteoAccesos++;
    }
    else
    {
        cout << "No se encuentra el bloque en la cache" << endl;
        setEstado(MISS);
        subirBloque(bloques[bloqueCache].getDatos(), blockAddress); // Subir bloque a cache
        setDato(bloqueCache, offset, nuevoD);                       // Escribir dato en la cache
        // actualizarLRU(bloqueCache);                                 // Actualiza el contador LRU no funciona aun
        conteoMisses++;
        ans = false; // No se encuentra el dato
    }

    return ans;
}

// bool Cache::escribir(int direccion, unsigned char nuevoDato) {}

void Cache::guardarEnArchivo(const string &nombreArchivo)
{
    ofstream archivo(nombreArchivo);
    if (archivo.is_open())
    {
        archivo << "Contenido de la Caché:\n";
        for (const auto &bloque : this->bloques)
        {
            archivo << "Etiqueta: " << hex << bloque.getEtiqueta();
            archivo << " | LRU: " << dec << bloque.getContadorLRU() << "\n";

            for (const auto &dato : bloque.getDatos())
            {
                archivo << hex << setw(2) << setfill('0') << static_cast<int>(dato) << " ";
            }
            archivo << "\n";
        }
        archivo.close();
    }
    else
    {
        std::cerr << "No se pudo abrir el archivo: " << nombreArchivo << std::endl;
    }
}

float Cache::calcularTasaMiss() const
{
    if (conteoAccesos == 0)
        return 0.0f;
    return (static_cast<float>(conteoMisses) / conteoAccesos) * 100.0f; // convierte enteros a float
}

int Cache::buscarBloque(int etiqueta)
{
    for (int i = 0; i < 32; i++)
    {
        if (bloques[i].esValido() && bloques[i].getEtiqueta() == etiqueta)
        {
            return i; // Si se encuentra
        }
    }
    return -1; // No se encuentra el bloque
}

int Cache::encontrarReemplazo()
{
    int max = bloques[0].getContadorLRU(); // Inicializa el maximo
    int bloqueReemplazo = 0;               // Inicializa el bloque a reemplazar
    for (int i = 1; i < 32; i++)
    {

        if (bloques[i].getContadorLRU() < max)
        {
            bloqueReemplazo = i;
            max = bloques[i].getContadorLRU(); // Actualiza el maximo
            bloques[i].restartLRU();           // Reinicia el contador LRU
        }
        cout << "Contador LRU: " << bloques[i].getContadorLRU() << endl;
        cout << "Maximo: " << max << endl;
        cout << "Bloque: " << dec << i << endl;
        cout << "Bloque reemplazo: " << dec << bloqueReemplazo << endl;
    }

    return bloqueReemplazo; // Indice del bloque a reemplazar
}

const Bloque &Cache::getBloque(int indice) const
{

    return bloques[indice];
}

void Cache::actualizarLRU(int indice)
{
    /*int erasePosition = -1;
    bool found = false;
    if (lruPolicy.size() != 0)
    {
        for (int i = 0; i < this->bloques.size() && !found; i++)
        {
            if (this->bloques[i].getEtiqueta() == indice)
            {
                erasePosition = i; // Encuentra la posicion del bloque a eliminar
                found = true;
            }
        }
        erase
    }*/
    if (estadoActual == HIT)
        bloques[indice].setContadorLRU(1);
    // Reinicia el contador LRU
    else if (estadoActual == MISS)
        bloques[indice].restartLRU();
}
int Cache::getConteoAccesos() const
{
    return conteoAccesos;
}

int Cache::getConteoMisses() const
{
    return conteoMisses;
}

void Cache::setDato(int direccionBloque, int offset, unsigned int nuevoDato)
{
    for (int i = 0; i < 32; i++)
    {
        if (bloques[i].getEtiqueta() == direccionBloque)
        {
            bloques[i].setDato(offset, nuevoDato); // Escribir dato en la cache
        }
    }
}