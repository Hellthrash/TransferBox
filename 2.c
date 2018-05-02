/* implementar lector escritor
Restriccion los lectores leen al mismo tiempo
si llega un escritor (uno a la vez) los lectores
dejan de leer */

/* programa lectores y escritores */

//método de compilacion gcc SemaforoPOSIX.c -o SemaforoPOSIX -lpthread
#include<stdlib.h>
#include<stdio.h>
#include<sys/sem.h>
#include<semaphore.h>   //necesario para impementar sem_t
//#include<pthread.h>
#include<unistd.h>

/* unistd.h es para el uso de fork() */

int cuentalect;
semaphore x = 1, sescr = 1;

void lector()
{
	while (true)
	{

		semWait (x);
		cuentalect++;
		if (cuentalect == 1)
		semWait (sescr);
		semSignal (x);
		LEERDATO();
		semWait (x);
		cuentalect—;
		if (cuentalect == 0)
		semSignal (sescr);
		semSignal (x);
	}
}

void escritor()
{
	while (true)
	{
		semWait (sescr);
		ESCRIBIRDATO();
		semSignal (sescr);
	}
}

void main()
{
	cuentalect = 0;
	paralelos (lector, escritor);
}
