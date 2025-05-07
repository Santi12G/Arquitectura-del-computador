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
    int blockAddress = direccion / 16; //
    int offset = direccion % 16;       //
    bool validBloque = false;
    int bloqueCache;
    cout << "Offset: " << offset << endl;
    cout << "Direccion bloque: " << dec << static_cast<int>(blockAddress) << endl;
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
        // actualizarLRU(bloqueCache); // Actualiza el contador LRU   --> recordar que hay que implementarlo con un deque para ir metiendo los recientemente usados en la parte de atras
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

void Cache::subirBloque(vector<unsigned int> &bloque, int direccionBase)
{
    // cout << "DIRECCION ACTUALIZAR LRU EN SUBIR BLOQUE: " << direccionBase / 16 << endl;
    // cout << "Politica LRU: " << lruPolicy.front() << endl;
    int lruSize = static_cast<int>(lruPolicy.size());
    cout << "Politica lru size: " << lruSize << endl;
    cout << "lru < 32: " << (lruSize < 32) << endl;
    int direccionBloque = direccionBase / 16; // Direccion del bloque
    int etiquetaBloqueReemplazar;
    int indiceReemplazar = 0; // Indice del bloque a reemplazar
    bool llena = false;       // Variable para verificar si la cache no esta llena
    if (lruSize == 32)
    {
        // cout << "FIRST REVISION" << endl;
        llena = true;
        etiquetaBloqueReemplazar = lruPolicy.front(); // Obtener el bloque menos usado
        lruPolicy.pop_front();                        // Eliminar el bloque menos usado
        lruPolicy.push_back(direccionBloque);         // Agregar el bloque a la politica LRU
    }
    else if (lruSize < 32 && !llena && lruSize != 0)
    {
        cout << "SECOND REVISION" << endl;
        // cout << "cache no llena" << endl;
        lruPolicy.push_back(direccionBloque); // Agregar el bloque a la politica LRU
        // cout << "Politica LRU: " << lruPolicy.front() << endl;
        etiquetaBloqueReemplazar = -1; // Obtener el bloque menos usado
    }

    if (lruSize <= 32 && lruSize != 0)
    {
        // cout << "THIRD REVISION" << endl;
        bool terminado = false;
        for (int i = 0; i < 32 && !terminado; i++)
        {
            if (bloques[i].getEtiqueta() == etiquetaBloqueReemplazar)
            {
                cout << "Etiqueta bloque reemplazar: " << etiquetaBloqueReemplazar << endl;
                indiceReemplazar = i; // Obtener el bloque menos usado
                terminado = true;
            }
        }
        bloques[indiceReemplazar].setDatos(bloque);           // Subir bloque a cache
        bloques[indiceReemplazar].setEtiqueta(direccionBase); // Etiqueta del bloque
        bloques[indiceReemplazar].setValido(true);
        // cout << "Politica lru third revision: " << lruPolicy.front() << endl;
    }
    if (lruSize == 0)
    {
        bloques[0].setDatos(bloque);           // Subir bloque a cache
        bloques[0].setEtiqueta(direccionBase); // Etiqueta del bloque
        bloques[0].setValido(true);
        lruPolicy.push_back(direccionBase);
        // cout << "Terminado el reemplazo del bloque " << etiquetaBloqueReemplazar << " por el bloque " << direccionBase << endl;
    }

    /*cout << "TOMANDO DATOS DE LA CACHE " << endl;
    for (int i = 0; i < 32; i++)
    {

    for (const auto &dato : bloques[i].getDatos())
    {
        cout << hex << static_cast<int>(dato) << " ";
    }
    cout << endl;
    }*/
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
        // setDato(bloqueCache, offset, nuevoD); // Escribir dato en la cache
        // cout << "Escribiendo en la cache: " << hex << static_cast<int>(nuevoD) << endl;
        ans = true; // Se encuentra el dato
        setEstado(HIT);
        // actualizarLRU(bloqueCache); // Actualiza el contador LRU revisar el actualizar LRU
        conteoAccesos++;
    }
    else
    {
        cout << "No se encuentra el bloque en la cache" << endl;
        setEstado(MISS);
        // subirBloque(bloques[bloqueCache].getDatos(), blockAddress); // Subir bloque a cache
        // setDato(bloqueCache, offset, nuevoD);                       // Escribir dato en la cache
        //  actualizarLRU(bloqueCache);                                 // Actualiza el contador LRU no funciona aun
        conteoMisses++;
        ans = false; // No se encuentra el dato
    }
    cout << "Saliendo funcion escribir cache interna" << endl;
    return ans;
}

void Cache::guardarEnArchivo(const string &nombreArchivo)
{
    ofstream archivo(nombreArchivo);
    if (archivo.is_open())
    {
        archivo << "Contenido de la Cache:\n";
        for (const auto &bloque : this->bloques)
        {
            archivo << "Etiqueta: " << dec << bloque.getEtiqueta();
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

void Cache::actualizarLRU(int direccionBase)
{
    int direccionBloqueLRU = direccionBase / 16;
    auto it = lruPolicy.begin();
    while (it != lruPolicy.end())
    {
        if (*it == direccionBloqueLRU)
        {
            lruPolicy.erase(it);
            it = lruPolicy.end();
        }
        else
        {
            ++it;
        }
    }
    lruPolicy.push_back(direccionBloqueLRU); // Agregar el bloque a la politica LRU
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
    cout << "direccion bloque: " << dec << direccionBloque << endl;
    cout << "Setting dato en la cache: " << hex << static_cast<int>(nuevoDato) << endl;
    cout << "offset: " << offset << endl;
    for (int i = 0; i < 32; i++)
    {
        if (bloques[i].getEtiqueta() == direccionBloque)
        {
            cout << "Se encontro el bloque " << bloques[i].getEtiqueta() << " en la memoria cache" << endl;
            bloques[i].setDato(offset, nuevoDato); // Escribir dato en la cache
            cout << "Escribiendo en la cache: " << hex << static_cast<int>(nuevoDato) << endl;
        }
    }
    // bloques[direccionBloque].setDato(offset, nuevoDato);
}