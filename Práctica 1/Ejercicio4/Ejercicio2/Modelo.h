#pragma once
enum class ClockMode { RunMode, StopMode };

class Modelo
{

public:
	//Modelo de Datos////////////////////////////
	float x, y, radio;								//Esfera
	float fHourAngle, fMinuteAngle, fSecondAngle;	//Agujas
													/////////////////////////////////////////////
	ClockMode clockMode = ClockMode::RunMode;

	Modelo();
	~Modelo();
	void Actualizar(int horas, int minutos, int segundos);
	void CambiaModo();
};

