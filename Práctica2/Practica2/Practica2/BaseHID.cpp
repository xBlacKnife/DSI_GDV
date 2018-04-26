#include "BaseHID.h"

BaseHID::BaseHID()
{
}

BaseHID::~BaseHID()
{
}

BaseHID::BaseHID(float t)
{
	T = t / 1000; //Periodo de muestreo
	a = T / (0.1 + T); //Cte. de tiempo para filtros (depende de T)
}

bool BaseHID::gBU(WORD bit) { return (!(wButtons & bit) && (wLastButtons & bit)); }

bool BaseHID::gBD(WORD bit) { return ((wButtons & bit) && !(wLastButtons & bit)); }

float BaseHID::gLT() { return fLeftTrigger; }

float BaseHID::gRT() { return fRightTrigger; }

float BaseHID::gLJX() { return fThumbLX; }

float BaseHID::gLJY() { return fThumbLY; }

float BaseHID::gRJX() { return fThumbRX; }

float BaseHID::gRJY() { return fThumbRY; }

float BaseHID::gLJXf() { return fThumbLXf; }

float BaseHID::gLJYf() { return fThumbLYf; }

float BaseHID::gRJXf() { return fThumbRXf; }

float BaseHID::gRJYf() { return fThumbRYf; }

void BaseHID::sLR(float cantidad, float tiempo)
{
	fLeftVibration = cantidad;
	tLR = tiempo;
}

void BaseHID::sRR(float cantidad, float tiempo)
{
	fRightVibration = cantidad;
	tRR = tiempo;
}

bool BaseHID::BD(WORD Bit)
{
	return (wButtonsDown & Bit);
}

bool BaseHID::BU(WORD Bit)
{
	return (wButtonsUp & Bit);
}

bool BaseHID::GRLJ()
{
	return (Ro == NEG_POS);
}

void BaseHID::Actualiza()
{
	wLastButtons = wButtons; //Copia estado de botones
	bConected = LeeMando(); //Leo Mando

	if (bConected == true)
	{
		Mando2HID(); //Vuelco de Mando a HID normalizando

		//Actualizo Gestos de entrada gen�ricos (entradas)
		wButtonsDown = (~wLastButtons)&(wButtons);
		wButtonsUp = (wLastButtons)&(~wButtons);

		//Thumbs
		fThumbLXf = (1 - a) * fThumbLXf + a * fThumbLX;
		fThumbLYf = (1 - a) * fThumbLYf + a * fThumbLY;
		fThumbRXf = (1 - a) * fThumbRXf + a * fThumbRX;
		fThumbRYf = (1 - a) * fThumbRYf + a * fThumbRY;

		//Rotacion
		if ((fThumbLX > 0)&(fThumbLY > 0))
		{
			Ro = POS_POS;
			tRo = 1.0;
		}

		if (tRo > 0)
		{
			tRo -= T;
			if (Ro == NEG_POS) 
				Ro = POS_POS;
			if ((fThumbLX > 0)&(fThumbLY <= 0)&(Ro == POS_POS))
				Ro = POS_NEG;
			if ((fThumbLX <= 0)&(fThumbLY < 0)&(Ro == POS_NEG))
				Ro = NEG_NEG;
			if ((fThumbLX < 0)&(fThumbLY >= 0)&(Ro == NEG_NEG)) 
				Ro = NEG_POS;
		}
		else
			Ro = POS_POS;

		//Genero Gesto de feedback (salida)

		if (tLR > 0) 
			tLR -= T;
		else 
			sLR(0.0, 0.0);

		if (tRR > 0) 
			tRR -= T;
		else 
			sRR(0.0, 0.0);

		EscribeMando(); //Escribo en Mando el feedback
	}
}
