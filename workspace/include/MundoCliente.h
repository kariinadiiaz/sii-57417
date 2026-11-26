// MundoCliente.h: CMundo del programa cliente (Práctica 4)
//
//////////////////////////////////////////////////////////////////////

#if !defined(MUNDOCLIENTE_H)
#define MUNDOCLIENTE_H

#include <vector>
#include "Plano.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Esfera.h"
#include "Raqueta.h"
#include "DatosMemCompartida.h"

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

	// P3: memoria compartida con el bot
	DatosMemCompartida* pdatos;

	// P4: tuberia servidor -> cliente (coordenadas)
	int fd_coordenadas;
	// P4: tuberia cliente -> servidor (teclas)
	int fd_teclas;
};

#endif // !defined(MUNDOCLIENTE_H)
