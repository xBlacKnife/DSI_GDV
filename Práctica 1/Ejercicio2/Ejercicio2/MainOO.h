#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1")
#include "Basewin.h"
template <class T> void SafeRelease(T **ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}
class MainOO : public Basewin <MainOO>
{
	ID2D1Factory *pFactory;
	ID2D1HwndRenderTarget *pRenderTarget;
	ID2D1SolidColorBrush *pBrush;
	D2D1_ELLIPSE ellipse;
	void CalculateLayout();
	HRESULT CreateGraphicsResources();
	void DiscardGraphicsResources();
	void OnPaint();
	void Resize();
public:
	MainOO() : pFactory(NULL), pRenderTarget(NULL), pBrush(NULL)
	{
	}
	PCWSTR ClassName() const { return L"Circle Window Class"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	void DrawClockHand(float fHandLength, float fAngle, float fStrokeWidth);
};

