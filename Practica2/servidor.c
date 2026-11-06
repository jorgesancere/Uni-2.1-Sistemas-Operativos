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
#define TAM_BUFFER 256

struct sockaddr_in server_address;
//variables globales
int sockfd, scfd;


//como procesa el hijo:
void procesarcliente(int socket){
    //asignamos variables para pdoer leer lo que me pide el cliente
    char buffer[TAM_BUFFER];
    //tam para la variable metodo y para la ruta que quiere el cliente
    char metodo[16], ruta[256];
    //tam para la variable respuesta
    char resp[256];
    int n, fd;

    //numero de bytes que lees del buffer -1 
    n = read(socket, buffer, sizeof(buffer) - 1);
    //sabemos que al final hay eof, asi que lo asignamos
    buffer[n] = '\0';

    // De "GET /app.html HTTP/1.1" sacamos el metodo (GET) y la ruta (/app.html)
    //escaneo lo que hay en el buffer (los 15 primeros son el metodo, los otros 255, la ruta)

    sscanf(buffer, "%15s %255s", metodo, ruta);
s
    // ruta + 1 evita la "/" inicial: "/app.html" -> "app.html"
    //guardo en fd la ruta en formato lectura
    fd = open(ruta + 1, O_RDONLY);

    //antes de enviar, usar formato de http response
    if (fd == -1) {
        // si no existe el archivo: 
        strcpy(resp, "HTTP/1.1 404 Not Found\r\n"
                     "Content-Type: text/html; charset=UTF-8\r\n"
                     "Connection: close\r\n"
                     "\r\n"
                     "<h1>404 Not Found</h1>\n");

        write(socket, resp, strlen(resp));


    } else { //en el caso de que exista el archivo: 

        strcpy(resp, "HTTP/1.1 200 OK\r\n"
                     "Content-Type: text/html; charset=UTF-8\r\n"
                     "Connection: close\r\n"
                     "\r\n");
        write(socket, resp, strlen(resp));
            //como existe, leemos lo que dice el archivo y leemos el archivo y lo mandamos al cliente
        while ((n = read(fd, buffer, sizeof(buffer))) > 0) {
            //voy escribiendo en el buffer el mensaje
            write(socket, buffer, n);
        }
        //cuando acabe, paro de mandar cosas
        close(fd);
    }
}

int main(){

    //Crear el socket antes de hacer el bind
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //COnfigurar la estructura sockaddr_in, preparo la conexino con el servidor (dirIP:puerto...)
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT); //puerto
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); //acepta conexiones de cualquier direccion

    //HAcer un bind al puerto 9999
    if (bind(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("bind");
        exit(1);
    }

    //listen
    listen(sockfd, 10);


    //lo dejamos en bucle esperando a un cliente, ahi hace fork y sigue esperando
    while (1){
        scfd = accept(sockfd, NULL, NULL);
        if (fork() == 0) {
            //El hijo hace lo siguiebnte:
            procesarcliente(scfd);
            close(scfd);
            exit(0);
        }
        else {
            close(scfd);
        }
    }
}