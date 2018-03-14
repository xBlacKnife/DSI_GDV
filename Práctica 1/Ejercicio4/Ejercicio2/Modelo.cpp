#include "Modelo.h"


Modelo::Modelo()
{
}


Modelo::~Modelo()
{
}

void Modelo::Actualizar(int horas, int minutos, int segundos)
{
	if (clockMode == ClockMode::RunMode)
	{
		fHourAngle = (360.0f / 12) * (horas)+(minutos * 0.5f);
		fMinuteAngle = (360.0f / 60) * (minutos);
		fSecondAngle = (360.0f / 60) * (segundos);
	}
}

void Modelo::CambiaModo()
{
	if (clockMode == ClockMode::RunMode)
		clockMode = ClockMode::StopMode;
	else
		clockMode = ClockMode::RunMode;
}
