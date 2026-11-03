#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

//Librerias de memoria compartida
#include <sys/ipc.h>
#include <sys/shm.h>
//--
#include <time.h>

#define CANTIDAD 10

int main(void){
	// pide un hueco de memoria copartida (10 enteros)
	int shmid = shmget(IPC_PRIVATE, sizeof(int) * CANTIDAD, IPC_CREAT | 0666);
	if(shmid == -1){
		perror("Error al crear memoria compartida");
		exit(1);
	}
	//se engancha a la memroia compartida
	int *numeros = (int *) shmat(shmid, NULL, 0);
	if(numeros == (void *) -1){
		perror("Error al engancharse a la memoria compartida");
		exit(1);
	}
	//creo hijo
	pid_t pid = fork();

	if(pid == 0){
		//Este será el proceso hijo
		srand(time(NULL) ^ getpid()); // prepara el seed para aleatorios
			//genera nums
		printf("Soy el hijo (%d): los números generados son: ", getpid());
		for(int i = 0; i < CANTIDAD; i++){
			numeros[i] = rand() % 100;
			printf("%d",numeros[i]);
			//Comas para separar
			if(i < CANTIDAD - 1){
				printf(", ");
			}
		}
		printf("\n");

		shmdt(numeros);
		exit(0);
	}
	else{
		//Este será el proceso padre
		wait(NULL);
		//vuelve a generar los numeros y calcula la media
		int suma = 0;
		for(int i = 0; i < CANTIDAD; i++){
			suma += numeros[i];
		}
		double media = (double) suma / CANTIDAD;

		printf("Soy el padre (%d). Los números generados fueron: ", getpid());
		for(int i = 0; i < CANTIDAD; i++){
			printf("%d", numeros[i]);
			//Comas para separar
			if(i < CANTIDAD - 1){
				printf(", ");
			}
		}
		printf(". La media es de %2.f\n", media);
		
		//elimina todo pa que no quede basura x la memoria
		shmdt(numeros);
		shmctl(shmid, IPC_RMID, NULL);
	}

	return 0;
}
