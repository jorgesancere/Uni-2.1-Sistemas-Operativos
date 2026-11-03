#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define TAM_BUFFER 256

int main(int argc, char*argv[]){
	if(argc != 3){
		fprintf(stderr, "Uso: %s archivo_origen archivo_destino\n", argv[0]);
		exit(1);
	}

	int fd_origen = open(argv[1], O_RDONLY);
	if(fd_origen == -1){
		perror("Error al abrir el archivo origen");
		exit(1);
	}

	int fd_destino = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(fd_destino == -1){
		perror("Error al crear el archivo destino");
		exit(1);
	}

	int fd[2];
	if(pipe(fd) == -1){
		perror("Error al crear la tubería");
		exit(1);
	}

	pid_t pid = fork();
	if(pid == -1){
		perror("Error al crear el proceso hijo");
		exit(1);
	}

	if(pid == 0){
		//Proceso hijo
		//Cierro lo que no me interesa no voy ausar

		close(fd[1]);
		close(fd_origen);

		char buffer[TAM_BUFFER];
		int n;

		while((n = read(fd[0], buffer, TAM_BUFFER)) > 0){
			write(fd_destino, buffer, n);
		}

		if(n == -1){
			perror("Error al leer la tubería");
		}
		//Lo cierro todo y me muero
		close(fd[0]);
		close(fd_destino);
		exit(0);
	}

	else{
		//Proceso padre
		//Cierro lo que no me interesa no voy ausar
		close(fd[0]);
		close(fd_destino);

		char buffer[TAM_BUFFER];
		int n;

		while((n = read(fd_origen, buffer, TAM_BUFFER)) > 0){
			write(fd[1], buffer, n);
		}

		if(n == -1){
			perror("Error al leer el archivo origen");
		}
		//Lo ceirro todo y espero a que mi hijo se muera
		close(fd_origen);
		close(fd[1]);

		wait(NULL);
	}

	return 0;
}