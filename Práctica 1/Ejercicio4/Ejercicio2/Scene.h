#include <d2d1.h>
#pragma comment(lib, "d2d1")
#pragma once
#include "Modelo.h"

template <class T> void SafeRelease(T **ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}
class Scene: public Modelo
{
public:
	Scene();
	~Scene();
	ID2D1Factory            *pFactory;
	ID2D1SolidColorBrush    *pBrush; //color de Esfera
	ID2D1SolidColorBrush    *pBrush2; //color de Esfera
	ID2D1SolidColorBrush    *pStroke; //color de flechas
	ID2D1HwndRenderTarget   *pRenderTarget;
	D2D1_ELLIPSE             ellipse;
	D2D1_ELLIPSE             ellipse2;
	D2D1_POINT_2F            ptMouse;

	//Manejar recursos gr�ficos
	HRESULT CreaFactoria();
	void	LiberaFactoria();
	HRESULT CreateGraphicsResources(HWND m_hwnd,RECT rc);
	void    DiscardGraphicsResources();
	
	//Layout y Dibujo
	void    CalculateLayout(RECT rc);
	void	DrawClockHand(float fHandLength, float fAngle, float fStrokeWidth);
	void	DrawEsfera();
	void    DrawReloj();
	void	RenderScene();

	bool HitTest(D2D1_ELLIPSE ellipse, float x, float y);
	void ActualizaElipse(float x, float y, float width, float height);
};
