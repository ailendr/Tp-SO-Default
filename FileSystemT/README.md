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
- Inicializar semaforos
- Atender peticiones
    - Crear un hilo por cada peticion
    - Recibimos peticion nueva
    - Listar nueva peticion para tener un orden (lista de instrucciones)
    - Se realiza la accion pedida
        - F_READ
        - F_WRITE
        - F_OPEN
        - F_CREATE
        - F_TRUNCATE
    - Mandamos respuesta
    - Liberamos lo que necesitamos
    - Cerramos peticion
- Finalizar y liberar todo 
    - Falta semaforos