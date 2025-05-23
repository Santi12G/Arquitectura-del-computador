#include "Procesador.h"
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

int main()
{
    // Crear una instancia del procesador
    Procesador procesador;

    // Inicializar la memoria desde el archivo
    string archivoMemoria = "InitializedDRAM.txt";
    cout << "Inicializando la memoria desde " << archivoMemoria << "...\n";
    procesador.inicializarMemoria(archivoMemoria);
    // procesador.imprimirMemoria(); // Mostrar la memoria inicializada @activar para temas de debug para mirar si la inicializacion fue correcta

    // Leer operaciones desde un archivo
    string archivoOperaciones = "prueba.txt";
    cout << "Leyendo operaciones desde " << archivoOperaciones << "...\n";
    procesador.leerArchivoOperaciones(archivoOperaciones);

    procesador.guardarCacheEnArchivo("cache.txt"); // Guardar la caché en un archivo

    procesador.imprimirMemoria(); // Guardar la memoria en un archivo
    // @Memoria: guardar contenido luego de procesar todas las operaciones

    procesador.exportarResultados("resultados.txt"); // Exportar resultados a un archivo

    int resultado = system("python graficar_tasaMisses.py resultados.txt"); // Ejecutar el script de Python para graficar

    if (resultado != 0)
    {
        std::cerr << "Error al ejecutar el script de Python." << std::endl;
    }
    /*





// Mostrar resultados en consola
cout << "\nResultados de la simulacion:\n";
procesador.mostrarResultados();
// Exportar resultados a un archivo

string archivoResultados = "resultados.txt";
cout << "Exportando resultados a " << archivoResultados << "...\n";
procesador.exportarResultados(archivoResultados);
*/
    /*
    // Guardar el contenido de la caché en un archivo
    cout << "\nGuardando el contenido de la caché en cache.txt...\n";
    procesador.imprimirCache();





    cout << "Simulación completada.\n";
    */
    return 0;
}
