
// MundoServidor.cpp: CMundo del servidor (Práctica 4)
// Hace la física del juego, avisa al logger, envía coordenadas al cliente
// y recibe las teclas del cliente en un hilo.
//
//////////////////////////////////////////////////////////////////////
#include <fstream>
#include "MundoServidor.h"
#include "glut.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// Práctica 2 (FIFO logger) y Práctica 4 (FIFOs con el cliente + hilo)
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FIFO_LOGGER "/tmp/FIFO_logger"
#define FIFO_COORDENADAS "/tmp/FIFO_coordenadas"
#define FIFO_TECLAS      "/tmp/FIFO_teclas"
#define PUNTOS_FIN  3

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMundo::CMundo()
{
	Init();
}

CMundo::~CMundo()
{
	// P2: cerrar la tuberia hacia el logger
	close(fd_logger);
	// P4: cerrar y destruir la tuberia de coordenadas (la creo yo)
	close(fd_coordenadas);
	unlink(FIFO_COORDENADAS);
	// P4: cerrar la tuberia de teclas (la crea y destruye el cliente)
	close(fd_teclas);
}

void CMundo::InitGL()
{
	//Habilitamos las luces, la renderizacion y el color de los materiales
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	
	glMatrixMode(GL_PROJECTION);
	gluPerspective( 40.0, 800/600.0f, 0.1, 150);
}

void print(char *mensaje, int x, int y, float r, float g, float b)
{
	glDisable (GL_LIGHTING);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT) );

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glColor3f(r,g,b);
	glRasterPos3f(x, glutGet(GLUT_WINDOW_HEIGHT)-18-y, 0);
	int len = strlen (mensaje );
	for (int i = 0; i < len; i++) 
		glutBitmapCharacter (GLUT_BITMAP_HELVETICA_18, mensaje[i] );
		
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable( GL_DEPTH_TEST );
}
void CMundo::OnDraw()
{
	//Borrado de la pantalla	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Para definir el punto de vista
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();
	
	gluLookAt(0.0, 0, 17,  // posicion del ojo
		0.0, 0.0, 0.0,      // hacia que punto mira  (0,0,0)  
		0.0, 1.0, 0.0);      // definimos hacia arriba (eje Y)    

	/////////////////
	///////////
	//		AQUI EMPIEZA MI DIBUJO
	char cad[100];
	sprintf(cad,"Jugador1: %d",puntos1);
	print(cad,10,0,1,1,1);
	sprintf(cad,"Jugador2: %d",puntos2);
	print(cad,650,0,1,1,1);
	int i;
	for(i=0;i<paredes.size();i++)
		paredes[i].Dibuja();

	fondo_izq.Dibuja();
	fondo_dcho.Dibuja();
	jugador1.Dibuja();
	jugador2.Dibuja();
	esfera.Dibuja();

	/////////////////
	///////////
	//		AQUI TERMINA MI DIBUJO
	////////////////////////////

	//Al final, cambiar el buffer
	glutSwapBuffers();
}

void CMundo::OnTimer(int value)
{	
	jugador1.Mueve(0.025f);
	jugador2.Mueve(0.025f);
	esfera.Mueve(0.025f);
	int i;
	for(i=0;i<paredes.size();i++)
	{
		paredes[i].Rebota(esfera);
		paredes[i].Rebota(jugador1);
		paredes[i].Rebota(jugador2);
	}

	jugador1.Rebota(esfera);
	jugador2.Rebota(esfera);
	if(fondo_izq.Rebota(esfera))
	{
		esfera.centro.x=0;
		esfera.centro.y=rand()/(float)RAND_MAX;
		esfera.velocidad.x=2+2*rand()/(float)RAND_MAX;
		esfera.velocidad.y=2+2*rand()/(float)RAND_MAX;
		puntos2++;
		// P2: avisar al logger
		char cad[200];
		sprintf(cad, "Jugador 2 marca 1 punto, lleva un total de %d puntos.", puntos2);
		write(fd_logger, cad, strlen(cad) + 1);
	}

	if(fondo_dcho.Rebota(esfera))
	{
		esfera.centro.x=0;
		esfera.centro.y=rand()/(float)RAND_MAX;
		esfera.velocidad.x=-2-2*rand()/(float)RAND_MAX;
		esfera.velocidad.y=-2-2*rand()/(float)RAND_MAX;
		puntos1++;
		// P2: avisar al logger
		char cad[200];
		sprintf(cad, "Jugador 1 marca 1 punto, lleva un total de %d puntos.", puntos1);
		write(fd_logger, cad, strlen(cad) + 1);
	}

	// P4: enviar al cliente las coordenadas (al final del OnTimer)
	char cad[200];
	sprintf(cad, "%f %f %f %f %f %f %f %f %f %f %d %d",
		esfera.centro.x, esfera.centro.y,
		jugador1.x1, jugador1.y1, jugador1.x2, jugador1.y2,
		jugador2.x1, jugador2.y1, jugador2.x2, jugador2.y2,
		puntos1, puntos2);
	write(fd_coordenadas, cad, sizeof(cad));

	// P2 (ejercicio propuesto): fin de la partida a los 3 puntos
	if (puntos1 >= PUNTOS_FIN || puntos2 >= PUNTOS_FIN)
	{
		exit(0);	// llama al destructor de mundo -> cierra la FIFO -> el logger termina
	}

}

void CMundo::OnKeyboardDown(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 's':jugador1.velocidad.y=-4;break;
	case 'w':jugador1.velocidad.y=4;break;
	case 'l':jugador2.velocidad.y=-4;break;
	case 'o':jugador2.velocidad.y=4;break;
	}
}

// P4: funcion que ejecuta el hilo
void* hilo_comandos(void* d)
{
	CMundo* p = (CMundo*) d;
	p->RecibeComandosJugador();
	return NULL;
}

// P4: bucle del hilo: lee la tecla que manda el cliente y mueve la raqueta
void CMundo::RecibeComandosJugador()
{
	// la FIFO de teclas la crea el cliente: esperamos a que exista
	while ((fd_teclas = open(FIFO_TECLAS, O_RDONLY)) < 0)
		usleep(100000);

	while (1) {
		usleep(10);
		char cad[100];
		if (read(fd_teclas, cad, sizeof(cad)) <= 0)
			break;	// el cliente ha cerrado la tuberia
		unsigned char key;
		sscanf(cad, "%c", &key);
		if (key == 's') jugador1.velocidad.y = -4;
		if (key == 'w') jugador1.velocidad.y = 4;
		if (key == 'l') jugador2.velocidad.y = -4;
		if (key == 'o') jugador2.velocidad.y = 4;
	}
}

void CMundo::Init()
{
	puntos1=0;
	puntos2=0;

	// P2: abrir la FIFO en escritura (la crea el logger, que se lanza antes)
	fd_logger = open(FIFO_LOGGER, O_WRONLY);
	if (fd_logger < 0) {
		perror("No se puede abrir la FIFO del logger");
		exit(1);
	}

	// P4: crear la tuberia de coordenadas y abrirla en escritura
	//     (se queda esperando hasta que arranca el cliente)
	unlink(FIFO_COORDENADAS);
	mkfifo(FIFO_COORDENADAS, 0600);
	fd_coordenadas = open(FIFO_COORDENADAS, O_WRONLY);
	if (fd_coordenadas < 0) {
		perror("No se puede abrir la FIFO de coordenadas");
		exit(1);
	}

	// P4: crear el hilo que recibe las teclas del cliente
	fd_teclas = -1;
	pthread_create(&thid1, NULL, hilo_comandos, this);

	Plano p;
	//pared inferior
	p.x1=-7;p.y1=-5;
	p.x2=7;p.y2=-5;
	paredes.push_back(p);

	//superior
	p.x1=-7;p.y1=5;
	p.x2=7;p.y2=5;
	paredes.push_back(p);

	fondo_izq.r=0;
	fondo_izq.x1=-7;fondo_izq.y1=-5;
	fondo_izq.x2=-7;fondo_izq.y2=5;

	fondo_dcho.r=0;
	fondo_dcho.x1=7;fondo_dcho.y1=-5;
	fondo_dcho.x2=7;fondo_dcho.y2=5;

	//a la izq
	jugador1.g=0;
	jugador1.x1=-6;jugador1.y1=-1;
	jugador1.x2=-6;jugador1.y2=1;

	//a la dcha
	jugador2.g=0;
	jugador2.x1=6;jugador2.y1=-1;
	jugador2.x2=6;jugador2.y2=1;
}
