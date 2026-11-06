
//Inicializamos las librerias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <fcntl.h>
#define TAM_BUFFER 256
#define PORT 9999

//cliente
//Uso: ./cliente IP_Servidor [fichero]   (por defecto pide index.html)
int main(int argc, char *argv[]) {
    if (argc != 2 && argc != 3){
        //si se pide algo mal, sale y muere
        printf("Uso: %s IP_Servidor [fichero] (por defecto es el index.html)\n", argv[0]);
        exit(1);
    }
    //creo variables y asigno la estructura esa
    int sfd, n;
    struct sockaddr_in server_address;
    char buffer[TAM_BUFFER];
    //tamaaño maximo del mensaje
    char mensaje[300];
    //fichero x defecto
    char *fichero = "index.html";
    //siquiero pedir un fichero que no sea el de por defecto, lo asignamos
    if (argc == 3) {
        fichero = argv[2];
    }

    // Peticion en formato http request: GET /fichero HTTP/1.1

    //ahora hago la peticion
    snprintf(mensaje, sizeof(mensaje),
             "GET /%s HTTP/1.1\r\n"
             "Host: localhost\r\n"
             "Connection: close\r\n"
             "\r\n", fichero);

    //Ip y puerto para saber donde conectarme
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr(argv[1]);
    
    //me conecto al servidor, y le envio lo que quiero
    connect(sfd, (struct sockaddr*)&server_address, sizeof(server_address));
    write(sfd, mensaje, strlen(mensaje));

    //lectura que se repite hasta que el servidor cierra (read == 0)
    while ((n = read(sfd, buffer, TAM_BUFFER)) > 0){
        write(STDOUT_FILENO, buffer, n); //muestra por pantalla lo que manda el servidor
    }

    close(sfd);
    return 0;
}