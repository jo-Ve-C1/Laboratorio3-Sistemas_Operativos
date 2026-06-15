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

	printf("[Padre]: Iniciando \n");
sem = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,-1, 0);
saldo = mmap(NULL, sizeof(double), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,-1, 0);

if(sem== MAP_FAILED || saldo == MAP_FAILED){
perror("Error al asignar memoria compartida con mmap");
exit (EXIT_FAILURE);
}

sem_init(sem, 1, 1);
*saldo = 0.0;

printf("[Padre]: Memoria compartida inicializada. Saldo base: %.2lf\n", *saldo);

sem_destroy(sem);
munmap(sem, sizeof(sem_t));
munmap(saldo, sizeof(double));

return 0;

}

void credito (char *archivo_montos, int p[]){}

void debito(char *archivo_montos, int p[]){}


