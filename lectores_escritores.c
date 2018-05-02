#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
 
/* programa lectores y escritores */

int cuentalect, cuentaescr;
semaphore x = 1, y = 1, z = 1, sescr = 1, slect = 1;

void lector()
{
	while (true)
	{
		semWait (z);
		semWait (slect);
		semWait (x);
		cuentalect++;
		if (cuentalect == 1)
		semWait (sescr);
		semSignal (x);
		semSignal (slect);
		semSignal (z);
		LEERDATO();
		semWait (x);
		cuentalect—;
		if (cuentalect == 0)
		semSignal (sescr);
		semSignal (x);
	}
}

void escritor ()
{
	while (true)
	{
		semWait (y);
		cuentaescr++;
		if (cuentaescr == 1)
		semWait (slect);
		semSignal (y);
		semWait (sescr);
		ESCRIBIRDATO();
		semSignal (sescr);
		semWait (y);
		cuentaescr—;
		if (cuentaescr == 0)
	semSignal (slect);
	semSignal (y);
	}
}

void main()
{
	cuentalect = cuentaescr = 0;
	paralelos (lector, escritor);
}
