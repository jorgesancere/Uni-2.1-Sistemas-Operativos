#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

// Manejador para despertar de los pause()
void manejador(int sig) {
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        exit(1);
    }

    char *objetivo = argv[1];
    int segundos = atoi(argv[2]);
    //pido mi pid
    pid_t pid_ejec = getpid();
    //Creo los otros
    pid_t pid_A;
    pid_t pid_B;
    pid_t pid_X;
    pid_t pid_Y;
    pid_t pid_Z;

    printf("Soy el proceso ejec: mi pid es %d\n", pid_ejec);

    // Hacer que el kil no mate, y avise para despertar
    signal(SIGUSR1, manejador);
    signal(SIGALRM, manejador);

    // 1. ejec crea a A
    pid_A = fork();
    if (pid_A == 0) {
        pid_A = getpid();
        printf("Soy el proceso A: mi pid es %d. Mi padre es %d\n", pid_A, getppid());

        // 2. A crea a B
        pid_B = fork();
        if (pid_B == 0) {
            pid_B = getpid();
            printf("Soy el proceso B: mi pid es %d. Mi padre es %d. Mi abuelo es %d\n", 
                   pid_B, getppid(), pid_ejec);

            // 3. B crea a X
            pid_X = fork();
            if (pid_X == 0) {
                pid_X = getpid();
                printf("Soy el proceso X: mi pid es %d. Mi padre es %d. Mi abuelo es %d. Mi bisabuelo es %d\n", 
                       pid_X, getppid(), pid_A, pid_ejec);

                pause(); // Espera la señal de Z

                if (strcmp(objetivo, "X") == 0) {
                    printf("Soy el proceso X con pid %d, he recibido la señal.\n", pid_X);
                    if (fork() == 0) {
                        execlp("ls", "ls", NULL);
                        exit(0);
                    }
                    wait(NULL);
                }

                printf("Soy X (%d) y muero\n", pid_X);
                exit(0);
            }

            // 4. B crea a Y
            pid_Y = fork();
            if (pid_Y == 0) {
                pid_Y = getpid();
                printf("Soy el proceso Y: mi pid es %d. Mi padre es %d. Mi abuelo es %d. Mi bisabuelo es %d\n", 
                       pid_Y, getppid(), pid_A, pid_ejec);

                pause(); // Espera la señal de Z

                if (strcmp(objetivo, "Y") == 0) {
                    printf("Soy el proceso Y con pid %d, he recibido la señal.\n", pid_Y);
                    if (fork() == 0) {
                        execlp("ls", "ls", NULL);
                        exit(0);
                    }
                    wait(NULL);
                }

                printf("Soy Y (%d) y muero\n", pid_Y);
                exit(0);
            }

            // 5. B crea a Z
            pid_Z = fork();
            if (pid_Z == 0) {
                pid_Z = getpid();
                printf("Soy el proceso Z: mi pid es %d. Mi padre es %d. Mi abuelo es %d. Mi bisabuelo es %d\n", 
                       pid_Z, getppid(), pid_A, pid_ejec);

                alarm(segundos);
                pause();

                // Enviar la señal al objetivo (con el [2])
                if (strcmp(objetivo, "A") == 0) kill(pid_A, SIGUSR1);
                else if (strcmp(objetivo, "B") == 0) kill(pid_B, SIGUSR1);
                else if (strcmp(objetivo, "X") == 0) kill(pid_X, SIGUSR1);
                else if (strcmp(objetivo, "Y") == 0) kill(pid_Y, SIGUSR1);

                // enviar señales a los hermanos pa que mueran
                if (strcmp(objetivo, "Y") != 0) kill(pid_Y, SIGUSR1);
                if (strcmp(objetivo, "X") != 0) kill(pid_X, SIGUSR1);

                printf("Soy Z (%d) y muero\n", pid_Z);
                exit(0);
            }

            // Si el objetivo era B
            if (strcmp(objetivo, "B") == 0) {
                pause();
                printf("Soy el proceso B con pid %d, he recibido la señal.\n", pid_B);
                if (fork() == 0) {   // crea a un hij para el pstree
                    execlp("pstree", "pstree", NULL);
                    exit(0);
                }
                wait(NULL);
            }

            // B espera a que terminen sus 3 hijos (se mueran) (Z, Y, X)
            wait(NULL);
            wait(NULL);
            wait(NULL);
            // Se muere B
            printf("Soy B (%d) y muero\n", pid_B);
            exit(0);
        }

        // Si el objetivo era A
        if (strcmp(objetivo, "A") == 0) {
            pause();
            // crea a un hij para el pstree
            printf("Soy el proceso A con pid %d, he recibido la señal.\n", pid_A);
            if (fork() == 0) {
                execlp("pstree", "pstree", NULL);
                exit(0);
            }
            wait(NULL);
        }

        // A espera a su hijo B
        wait(NULL);
        printf("Soy A (%d) y muero\n", pid_A);
        exit(0);
    }

    // ejec espera a su hijo A
    wait(NULL);
    printf("Soy ejec (%d) y muero\n", pid_ejec);
    return 0;
}