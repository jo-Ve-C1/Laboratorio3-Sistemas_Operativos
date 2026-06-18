/* *Laboratorio III
   Materia: Inforatica II
   Autores: Bañares, Pablo
	    Velazquez Cruz, Joel
  Url:https://github.com/jo-Ve-C1/Laboratorio3-Sistemas_Operativos.git


 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
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

if(sem == MAP_FAILED || saldo == MAP_FAILED) {
perror("error en mmap");
exit(EXIT_FAILURE);

}

sem_init(sem, 1, 1);
*saldo = 0.0;

if(pipe(fd_credito) == -1 || pipe (fd_debito) == -1){
perror("Error al asignar memoria compartida con mmap");
exit (EXIT_FAILURE);
}


//Hijo 1: Credito
pid_credito = fork();
if(pid_credito == -1){
perror("error en fork credito");
exit(1);
}


if (pid_credito == 0){
close(fd_credito[0]);
close(fd_debito[0]);
close(fd_debito[1]);

credito("credito.txt", fd_credito);
}

//Hijo 2: Debito
pid_debito = fork ();
if(pid_debito == -1){
perror("Error en fork debito");
exit(1);
}


if (pid_debito == 0){
close(fd_debito[0]);
close(fd_credito[0]);
close(fd_credito[1]);

debito("debito.txt", fd_debito);
}

close(fd_credito[1]);
close(fd_debito[1]);

int credito_activo = 1, debito_activo = 1;
double monto_recibido;

while(credito_activo || debito_activo){

if(credito_activo){
if(read(fd_credito[0], &monto_recibido, sizeof(double))> 0){
printf("[Credito]: recibido del hijo  -> %lf\n", monto_recibido);
}
else {
credito_activo = 0;
}
}
if(debito_activo){
if(read(fd_debito[0], &monto_recibido, sizeof(double))> 0){
printf("[Debito]: recibio del hijo -> %lf\n", monto_recibido);
}else{
debito_activo = 0;
}
}
}


wait(NULL);
wait(NULL);

printf("\nSaldo final de la cuenta = %lf\n", *saldo);


//Liberamos todos los recursos abiertos
close(fd_credito[0]);
close(fd_debito[0]);
sem_destroy(sem);
munmap(sem, sizeof(sem_t));
munmap(saldo, sizeof(double));

return 0;

}

void credito (char *archivo_montos, int p[]){
FILE *arch = fopen(archivo_montos, "r");
if(arch == NULL) {
printf("Error al abrir archivo de credito\n");
exit(1);
}
double valor;
while(fscanf(arch, "%lf", &valor)==1){
sem_wait(sem);//bloqueo semaforo

*saldo += valor;
write(p[1], &valor, sizeof(double));//mando dato al padre

sem_post(sem);//libero semaforo
}
fclose(arch);
close(p[1]); //cierro el pipe al terminal
exit(0);
}


void debito(char *archivo_montos, int p[]){
FILE *arch = fopen(archivo_montos, "r");
if(arch==NULL){
printf("error en leer el archivo debito\n");
exit(1);
}

double valor;
while(fscanf(arch, "%lf", &valor) == 1){
sem_wait(sem); //bloqueo semaforo

*saldo -= valor;
write(p[1], &valor, sizeof(double)); //envia el dato al padre

sem_post(sem); //libero semaforo
}

fclose(arch);
close(p[1]);
exit(0);

}



