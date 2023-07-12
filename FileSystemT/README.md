Notas de guia para File System
--

## Recibimos del Config
- IP_Memoria();
- IP_Escucha();
- puertoMemoria();
- puertoEscucha();
- pathSuperBloque();
- pathBitmap();
- pathBloques();
- pathFCB();
- retardoNumerico();

## PASOS
- Inicializar log y config :ok:
- Inicializamos el cliente (IP_Memoria() y puertoMemoria()) :ok:
- Inicializamos el servidor (IP_Escucha() y puertoEscucha()) :ok:
- Inicializar estructuras
    - Superbloque
    - FCB
    - Archivo Bloque
    - Bitmap
- Inicializar semaforos :ok:
- Iniciar los dos hilos
    - Atencion :ok:
    - Ejecutor :ok:
        - Falta funcion
- Atender peticiones
    - Recibimos peticion nueva :ok:
    - Listar nueva peticion para tener un orden (lista de instrucciones) :ok:
    - Se realiza la accion pedida
        - F_READ
        - F_WRITE
        - F_OPEN
        - F_CREATE
        - F_TRUNCATE
    - Mandamos respuesta :ok:
        - Falta que kernel la reciba
    - Liberamos lo que necesitamos :ok:
    - Cerramos peticion :ok:
- Finalizar y liberar todo 
    - Falta semaforos :ok:
    - Falta hilos -> DUDA ⁉️
