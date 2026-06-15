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

	pid_t pid_credito, pid_debito;

	printf("[Padre]: Iniciando \n");
sem = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,-1, 0);
saldo = mmap(NULL, sizeof(double), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,-1, 0);

if(sem== MAP_FAILED || saldo == MAP_FAILED){
perror("Error al asignar memoria compartida con mmap");
exit (EXIT_FAILURE);
}

sem_init(sem, 1, 1);
*saldo = 0.0;

printf("[Padre]: Creando Procesos Hijos...");

pid_credito = fork();

if (pid_credito<0){
perror("error al crear el hijo de credito");
exit(EXIT_FAILURE);
}

if(pid == 0){
printf("[hijo credito]: Proceso lanzado (PID: %d)\n", getpid());
//LLAMAR FUNCION

exit(EXIT_SUCCES);
}

pid_debito = fork ();
if(pid_debito <0){
perror ("Error hijo debito");
exit(EXIT_FAILURE);
}

if (pid_debito == 0){
printf("[hijo debito]: proceso lanzado (PID: %d)\n", getpid());
//funcion llamar
exit(EXIT_SUCCES);
}
wait(NULL);
wait(NULL);

printf("[Padre]: Hijos Finalizado. Saldo FInal: %.2lf\n",*saldo);

sem_destroy(sem);
munmap(sem, sizeof(sem_t));
munmap(saldo, sizeof(double));

return 0;

}

void credito (char *archivo_montos, int p[]){}

void debito(char *archivo_montos, int p[]){}


