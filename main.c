/* *Laboratorio III
   Materia: Inforatica II
   Autores: Bañares, Pablo
	    Velazquez Cruz, Joel

 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys?wait.h>
#include <semaphore.h>
#include <sys/mman.h>

sem_t *sem;
double *saldo;

void debito(char *archivo_montos, int p[]);
void credito(char *archivo_montos, int p[]);

int main(){
	int fd_credito[2];
	int fd_debito[2];
	pid_t pid_credito, pid_debito;

	printf("[Padre]: Iniciando \n");
sem = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,-1, 0);
saldo = mmap(NULL, sizeof(double), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,-1, 0);
sem_init(sem, 1, 1);
*saldo = 0.0;

if(pipe(fd_credito) == -1 || pipe (fd_debito) == -1){
perror("Error al asignar memoria compartida con mmap");
exit (EXIT_FAILURE);
}

pid_credito = fork();

if (pid_credito == 0){
close(fd_credito[0]);
close(fd_debito[0]);
close(fd_debito[1]);

exit(EXIT_SUCCESS);
}

pid_debito = fork ();

if (pid_debito == 0){
close(fd_debito[0]);
close(fd_credito[0]);
close(fd_credito[1]);

exit(EXIT_SUCCES);
}

close(fd_credito[1]);
close(fd_debito[1]);

printf("[padre]: Canales de comunicacion listos\n");


wait(NULL);
wait(NULL);

close(fd_credito[0]);
close(fd_debito[0]);
sem_destroy(sem);
munmap(sem, sizeof(sem_t));
munmap(saldo, sizeof(double));

return 0;

}

void credito (char *archivo_montos, int p[]){}

void debito(char *archivo_montos, int p[]){}


