//Funciones necesarias
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <arpa/inet.h>


//puerto en el que escucha y tamaño del bufer
#define PORT 9999
#define TAM_BUFFER 1024

struct sockaddr_in server_address;
//variables globales
int sockfd, scfd;

void procesarcliente(int socket){
    char buffer[TAM_BUFFER];
    char metodo[16], ruta[256];
    char resp[256];
    int n, fd;

    //read del socket del cliente
    n = read(socket, buffer, sizeof(buffer) - 1);
    buffer[n] = '\0';

    // De "GET /app.html HTTP/1.1" sacamos el metodo (GET) y la ruta (/app.html)
    sscanf(buffer, "%15s %255s", metodo, ruta);

    // ruta + 1 salta la "/" inicial: "/app.html" -> "app.html"
    fd = open(ruta + 1, O_RDONLY);

    //antes de enviar, usar formato de http response
    if (fd == -1) {
        // PLUS: el archivo no existe -> 404
        strcpy(resp, "HTTP/1.1 404 Not Found\r\n"
                     "Content-Type: text/html; charset=UTF-8\r\n"
                     "Connection: close\r\n"
                     "\r\n"
                     "<h1>404 Not Found</h1>\n");
        write(socket, resp, strlen(resp));
    } else {
        strcpy(resp, "HTTP/1.1 200 OK\r\n"
                     "Content-Type: text/html; charset=UTF-8\r\n"
                     "Connection: close\r\n"
                     "\r\n");
        write(socket, resp, strlen(resp));

        // Cuerpo: leemos el archivo y lo mandamos al cliente
        while ((n = read(fd, buffer, sizeof(buffer))) > 0) {
            write(socket, buffer, n);
        }
        close(fd);
    }
}

int main(){

    //Crear el socket antes de hacer el bind
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //COnfigurar la estructura sockaddr_in
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT); //puerto en formato red
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); //acepta conexiones de cualquier direccion

    //HAcer un bind al puerto 9999
    if (bind(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("bind");
        exit(1);
    }

    //listen
    listen(sockfd, 10);

    while (1){
        scfd = accept(sockfd, NULL, NULL);
        if (fork() == 0) {
            procesarcliente(scfd);
            close(scfd);
            exit(0);
        }
        else {
            close(scfd);
        }
    }
}