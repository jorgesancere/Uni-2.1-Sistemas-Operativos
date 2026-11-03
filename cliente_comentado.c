// Inicializamos las librerias necesarias para las funciones del sistema[cite: 217]
#include <stdio.h>      // Incluye la libreria estandar de entrada/salida (printf, snprintf)[cite: 217]
#include <stdlib.h>     // Incluye funciones de gestion de memoria y control del programa (exit)[cite: 217]
#include <string.h>     // Incluye funciones para manipular cadenas de caracteres (strlen)[cite: 217]
#include <unistd.h>     // Incluye llamadas al sistema POSIX (read, write, close)[cite: 217]
#include <sys/socket.h> // Incluye las definiciones principales para el uso de sockets[cite: 217]
#include <netinet/in.h> // Incluye las estructuras para direcciones de red (sockaddr_in)[cite: 217]
#include <arpa/inet.h>  // Incluye funciones para manipular direcciones IP (inet_addr)[cite: 217]
#include <sys/types.h>  // Incluye definiciones de tipos de datos del sistema[cite: 217]
#include <fcntl.h>      // Incluye opciones de control de archivos (no se usa mucho en este cliente)[cite: 217]

#define TAM_BUFFER 256  // Define una constante de 256 bytes para el tamaño de lectura[cite: 217]
#define PORT 9999       // Define el puerto 9999 al que nos conectaremos en el servidor[cite: 217]

//------------------------------------------------------[cite: 217]
//cliente[cite: 217]
//Uso: ./cliente IP_Servidor [fichero]   (por defecto pide index.html)[cite: 217]
int main(int argc, char *argv[]) { // Funcion principal que recibe el numero (argc) y valores (argv) de los argumentos[cite: 217]
    if (argc != 2 && argc != 3){ // Comprueba si el usuario NO ha introducido 2 ni 3 argumentos en la consola[cite: 217]
        printf("Uso: %s IP_Servidor [fichero]\n", argv[0]); // Muestra por pantalla como se debe usar el programa[cite: 217]
        exit(1); // Cierra el programa con un codigo de error (1)[cite: 217]
    }

    int sfd, n; // Declara 'sfd' para guardar el descriptor del socket y 'n' para los bytes leidos[cite: 217]
    struct sockaddr_in server_address; // Declara la estructura que guardara la IP y puerto del servidor[cite: 217]
    char buffer[TAM_BUFFER]; // Crea un array de 256 bytes para guardar lo que recibimos del servidor[cite: 217]
    char mensaje[300]; // Crea un array de 300 bytes para construir la peticion HTTP que vamos a enviar[cite: 217]
    char *fichero = "index.html"; // Define que, si no se especifica, el archivo a pedir es "index.html"[cite: 217]
    
    if (argc == 3) { // Si el usuario ha introducido 3 argumentos (ej: ./cliente IP archivo.txt)[cite: 217]
        fichero = argv[2]; // Sobrescribe el nombre del fichero por defecto con el que ha puesto el usuario[cite: 217]
    }

    // Peticion en formato http request: GET /fichero HTTP/1.1[cite: 217]
    snprintf(mensaje, sizeof(mensaje), // Formatea texto y lo guarda dentro de la variable 'mensaje' de forma segura[cite: 217]
             "GET /%s HTTP/1.1\r\n" // Escribe el metodo GET, inyecta el nombre del fichero y la version HTTP[cite: 217]
             "Host: localhost\r\n" // Añade la cabecera Host, requerida en peticiones HTTP/1.1[cite: 217]
             "Connection: close\r\n" // Pide al servidor que cierre la conexion al terminar de enviar[cite: 217]
             "\r\n", fichero); // Añade el doble salto de linea obligatorio de HTTP e inserta la variable 'fichero'[cite: 217]

    //Ip y puerto para saber donde conectarme[cite: 217]
    sfd = socket(AF_INET, SOCK_STREAM, 0); // Crea un socket IPv4 (AF_INET) y TCP (SOCK_STREAM), devolviendo su descriptor[cite: 217]
    server_address.sin_family = AF_INET; // Indica a la estructura de direccion que usaremos IPv4[cite: 217]
    server_address.sin_port = htons(PORT); // Convierte el puerto 9999 al formato de bytes de red (htons) y lo guarda[cite: 217]
    server_address.sin_addr.s_addr = inet_addr(argv[1]); // Convierte la IP introducida (texto) a binario de red y la guarda[cite: 217]

    connect(sfd, (struct sockaddr*)&server_address, sizeof(server_address)); // Conecta nuestro socket al servidor usando la IP/Puerto guardados[cite: 217]
    write(sfd, mensaje, strlen(mensaje)); // Envia por el socket la peticion HTTP contenida en 'mensaje'[cite: 217]

    //lectura que se repite hasta que el servidor cierra (read == 0)[cite: 217]
    while ((n = read(sfd, buffer, TAM_BUFFER)) > 0){ // Lee del socket al 'buffer' maximo 256 bytes, y entra al bucle si leyo algo (>0)[cite: 217]
        write(STDOUT_FILENO, buffer, n); // Escribe en la pantalla (STDOUT_FILENO) exactamente los 'n' bytes que acaba de leer[cite: 217]
    }

    close(sfd); // Cierra el socket para liberar los recursos de red de nuestro ordenador[cite: 217]
    return 0; // Finaliza el programa correctamente devolviendo 0 al sistema operativo[cite: 217]
}