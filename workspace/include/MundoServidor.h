// MundoServidor.h: CMundo del programa servidor (Práctica 4)
//
//////////////////////////////////////////////////////////////////////

#if !defined(MUNDOSERVIDOR_H)
#define MUNDOSERVIDOR_H

#include <vector>
#include <pthread.h>
#include "Plano.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Esfera.h"
#include "Raqueta.h"


class CMundo  
{
public:
	void Init();
	CMundo();
	virtual ~CMundo();	
	
	void InitGL();	
	void OnKeyboardDown(unsigned char key, int x, int y);
	void OnTimer(int value);
	void OnDraw();	

	Esfera esfera;
	std::vector<Plano> paredes;
	Plano fondo_izq;
	Plano fondo_dcho;
	Raqueta jugador1;
	Raqueta jugador2;

	int puntos1;
	int puntos2;

	// P2: tuberia (FIFO) hacia el logger
	int fd_logger;

	// P4: tuberia servidor -> cliente (coordenadas)
	int fd_coordenadas;
	// P4: tuberia cliente -> servidor (teclas), la lee un hilo
	int fd_teclas;
	pthread_t thid1;
	void RecibeComandosJugador();
};

#endif // !defined(MUNDOSERVIDOR_H)
