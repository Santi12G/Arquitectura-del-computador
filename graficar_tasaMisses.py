import sys
import matplotlib.pyplot as plt

def graficar_tasa_misses(nombre_archivo):
    try:
        with open(nombre_archivo, 'r') as archivo:
            total_accesos = 0
            total_misses = 0
            tasa_misses = 0.0

            for linea in archivo:
                if "Total accesos:" in linea:
                    total_accesos = int(linea.strip().split(":")[1])
                elif "Total misses:" in linea:
                    total_misses = int(linea.strip().split(":")[1])
                elif "Tasa misses:" in linea:
                    tasa_misses = float(linea.strip().split(":")[1].replace('%', ''))

            tasa_hits = 100.0 - tasa_misses

            # Gráfico de barras
            etiquetas = ['Misses', 'Hits']
            valores = [tasa_misses, tasa_hits]
            colores = ['red', 'green']

            plt.bar(etiquetas, valores, color=colores)
            plt.title('Tasa de Misses vs Hits')
            plt.ylabel('Porcentaje (%)')
            plt.ylim(0, 100)
            plt.grid(axis='y', linestyle='--', alpha=0.7)
            plt.show()

    except FileNotFoundError:
        print(f"Error: El archivo '{nombre_archivo}' no existe.")
    except Exception as e:
        print(f"Error procesando el archivo: {e}")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Uso: python graficar_misses.py <archivo.txt>")
    else:
        graficar_tasa_misses(sys.argv[1])
