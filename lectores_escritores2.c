/* Lectores y escritores con prioridad escritor */
/* Leandro Caloguerea Farias */
/* Diego Rojas Asenjo */

#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
 
#define MAXTHREAD 10  /* define # lectores */
 
 
void access_database();     /* prototypes */
void non_access_database(); 
 
void* reader(void*);
void* writer(void*);
 
sem_t q;        /* controla el flujo */
int rc = 0;     /* numero de lectores leyendo */
int wc = 0;
int write_request = 0;
 
int main()
{
    pthread_t readers[MAXTHREAD],writerTh;
    int index; 		/* establece control */
    int ids[MAXTHREAD]; /* readers and initialize mutex, q and db-set them to 1 */
    sem_init (&q,0,1);
    for(index = 0; index < MAXTHREAD; index ++)  
    {
        ids[index]=index+1;                             /* Crea lectores y valida errores */
        if(pthread_create(&readers[index],0,reader,&ids[index])!=0){
            perror("No se puede crear lector!");
            exit(1);                            
        }
    }
    if(pthread_create(&writerTh,0,writer,0)!=0){
        perror("No se puede crear escritor!");     /* Crea escritores y valida errores */
        exit(1);
    }
 
    pthread_join(writerTh,0);   
    sem_destroy (&q);
    return 0;
}
 
void* reader(void*arg)          /* Funcion para leer */
{
    int index = *(int*)arg;
    int can_read;
    while(1){
        can_read = 1;
 
        sem_wait(&q);
        if(wc == 0 && write_request == 0)  rc++;
        else                               can_read = 0;
        sem_post(&q);
 
        if(can_read) {
            access_database();
            printf("Lector %d leyendo... lectores activos %d \n", index,rc);
            sleep(index);
 
            sem_wait(&q);
            rc--;
            sem_post(&q);
        }
 
        sched_yield();
    }
    return 0;
}
;
void* writer(void*arg)      /* Funcion para escribir */
{
    int can_write;
    while(1){
        can_write = 1;
        non_access_database();
 
        sem_wait (&q);
        if(rc == 0)   wc++; // verifico que saque a todos los lectores antes de escribir y hago escritor+1
        else          { can_write = 0; write_request = 1; }
        sem_post(&q);
 
        if(can_write) {
            access_database();  
            printf("Escritor está ahora escribiendo...Numero de lectores actual: %d\n",rc);
 
            sleep(3);
 
            sem_wait(&q);
            wc--;
            write_request = 0;
            sem_post(&q);
        }
 
        sched_yield();
    }
    return 0;
}
 
void access_database()
{
 
}
 
 
void non_access_database()
{
 
}
