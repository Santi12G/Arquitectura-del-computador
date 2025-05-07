import random

def generar_datos_hex(num_datos=2032, archivo_salida="datos_hex.txt"):
    # Crear o abrir el archivo para escritura
    with open(archivo_salida, "w") as archivo:
        for _ in range(num_datos):
            valor = random.randint(0, 255)
            archivo.write(f"0x{valor:02X}\n")

    print(f"Datos hexadecimales guardados en '{archivo_salida}'")

generar_datos_hex()
