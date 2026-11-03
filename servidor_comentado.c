#include <stdio.h>      // Incluye funciones estandar de entrada y salida (perror, sscanf)[cite: 224]
#include <stdlib.h>     // Incluye funciones del sistema (exit)[cite: 224]
#include <unistd.h>     // Incluye llamadas al sistema (read, write, close, fork)[cite: 224]
#include <sys/socket.h> // Incluye la base para crear y manejar sockets (socket, bind, listen, accept)[cite: 224]
#include <netinet/in.h> // Incluye las estructuras de direcciones para red (sockaddr_in)[cite: 224]
#include <sys/types.h>  // Incluye la definicion de tipos de datos estandar[cite: 224]
#include <fcntl.h>      // Incluye las constantes de control de archivos (como O_RDONLY para abrir)[cite: 224]
#include <string.h>     // Incluye funciones para copiar y medir cadenas de texto (strcpy, strlen)[cite: 224]
#include <arpa/inet.h>  // Incluye funciones para convertir formatos de red (htonl, htons)[cite: 224]

#define PORT 9999       // Define el puerto donde el servidor se quedara escuchando[cite: 224]
#define TAM_BUFFER 1024 // Define un buffer mas grande (1024 bytes) para leer archivos de golpe[cite: 224]

struct sockaddr_in server_address; // Declara globalmente la estructura que tendra la configuracion del servidor[cite: 224]
int sockfd, scfd; // Declara variables globales para el socket principal (sockfd) y el socket del cliente (scfd)[cite: 224]

void procesarcliente(int socket){ // Funcion que se encarga de entender al cliente y mandarle su archivo[cite: 224]
    char buffer[TAM_BUFFER]; // Espacio en memoria para leer la peticion y leer datos del archivo[cite: 224]
    char metodo[16], ruta[256]; // Variables para guardar "GET" y "/archivo.html" por separado[cite: 224]
    char resp[256]; // Variable para guardar las cabeceras HTTP de respuesta (200 OK o 404 Not Found)[cite: 224]
    int n, fd; // 'n' guardara los bytes leidos/escritos, y 'fd' es el descriptor del archivo fisico a abrir[cite: 224]

    //read del socket del cliente[cite: 224]
    n = read(socket, buffer, sizeof(buffer) - 1); // Lee la peticion que envio el cliente dejando 1 byte libre al final[cite: 224]
    buffer[n] = '\0'; // Coloca el caracter nulo al final para convertir los bytes leidos en una cadena de texto (String)[cite: 224]

    // De "GET /app.html HTTP/1.1" sacamos el metodo (GET) y la ruta (/app.html)[cite: 224]
    sscanf(buffer, "%15s %255s", metodo, ruta); // Extrae la primera y segunda palabra del texto leido, evadiendo desbordamientos[cite: 224]

    // ruta + 1 salta la "/" inicial: "/app.html" -> "app.html"[cite: 224]
    fd = open(ruta + 1, O_RDONLY); // Intenta abrir en disco el archivo pedido (sin el '/') en modo Solo Lectura[cite: 224]

    //antes de enviar, usar formato de http response[cite: 224]
    if (fd == -1) { // Comprueba si fd es -1, lo que indica que open falló (el archivo no existe)[cite: 224]
        // PLUS: el archivo no existe -> 404[cite: 224]
        strcpy(resp, "HTTP/1.1 404 Not Found\r\n" // Copia la primera linea de error 404 en la variable 'resp'[cite: 224]
                     "Content-Type: text/html; charset=UTF-8\r\n" // Indica al cliente que le enviamos un texto HTML[cite: 224]
                     "Connection: close\r\n" // Avisa que cerraremos la conexion tras enviar esto[cite: 224]
                     "\r\n" // Doble salto de linea para separar cabeceras del cuerpo del mensaje[cite: 224]
                     "<h1>404 Not Found</h1>\n"); // Mensaje visual de error que se mostrara si fuera un navegador[cite: 224]
        write(socket, resp, strlen(resp)); // Envia todo ese mensaje de error montado al socket del cliente[cite: 224]
    } else { // Si fd NO es -1, el archivo se abrio correctamente[cite: 224]
        strcpy(resp, "HTTP/1.1 200 OK\r\n" // Copia la cabecera HTTP de exito[cite: 224]
                     "Content-Type: text/html; charset=UTF-8\r\n" // Indica el tipo de contenido[cite: 224]
                     "Connection: close\r\n" // Avisa del cierre de conexion[cite: 224]
                     "\r\n"); // Doble salto de linea para finalizar las cabeceras[cite: 224]
        write(socket, resp, strlen(resp)); // Envia las cabeceras afirmativas al socket del cliente[cite: 224]

        // Cuerpo: leemos el archivo y lo mandamos al cliente[cite: 224]
        while ((n = read(fd, buffer, sizeof(buffer))) > 0) { // Bucle: lee un trozo (1024 max) del archivo fisico al 'buffer'[cite: 224]
            write(socket, buffer, n); // Envia exactamente ese trozo de archivo ('n' bytes) al cliente[cite: 224]
        }
        close(fd); // Cierra el archivo del disco porque ya terminamos de leerlo entero[cite: 224]
    }
}

int main(){ // Funcion principal del programa del servidor[cite: 224]

    //Crear el socket antes de hacer el bind[cite: 224]
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // Crea el socket TCP en modo IPv4 y guarda su descriptor[cite: 224]

    //COnfigurar la estructura sockaddr_in[cite: 224]
    server_address.sin_family = AF_INET; // Asigna IPv4 como tipo de direccion al servidor[cite: 224]
    server_address.sin_port = htons(PORT); //puerto en formato red // Convierte el 9999 al orden de bytes de red y lo asigna[cite: 224]
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); //acepta conexiones de cualquier direccion // Escucha en todas las IPs de la maquina[cite: 224]

    //HAcer un bind al puerto 9999[cite: 224]
    if (bind(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) { // Une (bind) el socket a la IP y puerto configurados[cite: 224]
        perror("bind"); // Si bind falla (ej: puerto usado), imprime el error en consola[cite: 224]
        exit(1); // Cierra el servidor completamente por error critico[cite: 224]
    }

    //listen[cite: 224]
    listen(sockfd, 10); // Activa el socket en modo 'escucha' y le permite dejar a 10 clientes maximo en cola de espera[cite: 224]

    while (1){ // Inicia un bucle infinito, el servidor siempre estara encendido esperando clientes[cite: 224]
        scfd = accept(sockfd, NULL, NULL); // El programa se pausa aqui hasta que alguien se conecta. Devuelve el socket dedicado (scfd)[cite: 224]
        if (fork() == 0) { // Crea un clon del programa (hijo) para atender al cliente en paralelo. Devuelve 0 al hijo[cite: 224]
            procesarcliente(scfd); // Solo el proceso hijo entra aqui y atiende la solicitud del cliente[cite: 224]
            close(scfd); // El proceso hijo cierra su copia del socket dedicado al cliente tras responderle[cite: 224]
            exit(0); // El proceso hijo se "suicida" (termina con exito) porque ya cumplio su tarea[cite: 224]
        }
        else { // Esto lo ejecuta el proceso original (padre) despues de crear al hijo[cite: 224]
            close(scfd); // El padre cierra su copia del socket dedicado, ya que el hijo es quien se encarga de esa comunicacion[cite: 224]
            // Al terminar el else, el padre vuelve instantaneamente al inicio del while para aceptar al siguiente cliente[cite: 224]
        }
    }
}