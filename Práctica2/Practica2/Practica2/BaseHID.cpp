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

void BaseHID::Actualiza()
{
	wLastButtons = wButtons; //Copia estado de botones
	bConected = LeeMando(); //Leo Mando
	if (bConected == true)
	{
		Mando2HID(); //Vuelco de Mando a HID normalizando
					 //Actualizo Gestos de entrada genéricos (entradas)
		wButtonsDown
			//Genero Gesto de feedback (salida)
			....
			EscribeMando(); //Escribo en Mando el feedback
	}
}

bool BaseHID::gBU(WORD bit) { return false; }

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
}

void BaseHID::sRR(float cantidad, float tiempo)
{
}

bool BaseHID::BD(WORD Bit)
{
	return false;
}

bool BaseHID::BU(WORD Bit)
{
	return false;
}

bool BaseHID::GRLJ()
{
	return false;
}
