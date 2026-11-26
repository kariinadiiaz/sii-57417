// bot.cpp - Práctica 3
// Controla la raqueta1 leyendo/escribiendo en el fichero proyectado en memoria.
// Se lanza DESPUÉS del tenis (el tenis crea el fichero).
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "DatosMemCompartida.h"

#define FICHERO_BOT "/tmp/datos_bot"

int main(int argc, char* argv[])
{
	DatosMemCompartida* dat;	// 1. puntero a DatosMemCompartida
	char* direccion;
	int fd;

	// 2. abrir el fichero creado por el tenis y proyectarlo en memoria
	fd = open(FICHERO_BOT, O_RDWR);
	if (fd == -1)
	{
		perror("Error al abrir el archivo (¿has lanzado antes el tenis?)");
		return 1;
	}
	direccion = (char*)mmap(NULL, sizeof(DatosMemCompartida),
	                        PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (direccion == MAP_FAILED)
	{
		perror("Error en la proyeccion mmap");
		return 1;
	}
	// 3. cerrar el descriptor
	close(fd);
	// 4. guardar la direccion en el puntero
	dat = (DatosMemCompartida*)direccion;

	// 5. bucle infinito cada 25 ms
	while (1)
	{
		usleep(25000);
		float centro_raqueta = (dat->raqueta1.y1 + dat->raqueta1.y2) / 2;
		float y_esfera = dat->esfera.centro.y;

		if (y_esfera > centro_raqueta + 0.1f)      dat->accion = 1;	// arriba
		else if (y_esfera < centro_raqueta - 0.1f) dat->accion = -1;	// abajo
		else                                       dat->accion = 0;	// nada
	}
	munmap(direccion, sizeof(DatosMemCompartida));
	return 0;
}
