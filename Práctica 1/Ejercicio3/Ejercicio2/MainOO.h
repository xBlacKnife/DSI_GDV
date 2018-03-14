#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1")
#include "Basewin.h"
#include "WindowsX.h"
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
	ID2D1SolidColorBrush *pStroke;
	D2D1_ELLIPSE ellipse;
	D2D1_ELLIPSE ellipse2;
	UINT_PTR Timer;
	void CalculateLayout();
	HRESULT CreateGraphicsResources();
	void DiscardGraphicsResources();
	void OnPaint();
	void Resize();
public:
	MainOO() : pFactory(NULL), pRenderTarget(NULL), pBrush(NULL),
		ellipse2(D2D1::Ellipse(D2D1::Point2F(), 0, 0)), ptMouse(D2D1::Point2F())
	{
	}
	PCWSTR ClassName() const { return L"Circle Window Class"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void OnLButtonDown(int pixelX, int pixelY, DWORD flags);
	void OnLButtonUp();
	void OnMouseMove(int pixelX, int pixelY, DWORD flags);

private:
	void DrawClockHand(float fHandLength, float fAngle, float fStrokeWidth);
	D2D1_POINT_2F ptMouse;
};

class DPIScale
{
	static float scaleX;
	static float scaleY;
public:
	static void Initialize(ID2D1Factory *pFactory)
	{
		FLOAT dpiX, dpiY;
		pFactory->GetDesktopDpi(&dpiX, &dpiY);
		scaleX = dpiX / 96.0f;
		scaleY = dpiY / 96.0f;
	}
	template <typename T>
	static D2D1_POINT_2F PixelsToDips(T x, T y)
	{
		return D2D1::Point2F(static_cast<float>(x) / scaleX, static_cast<float>(y) / scaleY);
	}
};
float DPIScale::scaleX = 1.0f;
float DPIScale::scaleY = 1.0f;

