#include "MainOO.h"
// Recalculate drawing layout when the size of the window changes.
void MainOO::CalculateLayout() {
	if (pRenderTarget != NULL)
	{
		D2D1_SIZE_F size = pRenderTarget->GetSize();
		const float x = size.width / 10;
		const float y = size.height / 10;
		const float radius = min(x, y);
		ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
	}
}
HRESULT MainOO::CreateGraphicsResources()
{
	HRESULT hr = S_OK;
	if (pRenderTarget == NULL)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
		hr = pFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&pRenderTarget);
		if (SUCCEEDED(hr))
		{
			const D2D1_COLOR_F color1 = D2D1::ColorF(1.0f, 1.0f, 0.0f);
			hr = pRenderTarget->CreateSolidColorBrush(color1, &pBrush);
			const D2D1_COLOR_F color2 = D2D1::ColorF(0.0f, 0.0f, 0.0f);
			hr = pRenderTarget->CreateSolidColorBrush(color2, &pStroke);
			if (SUCCEEDED(hr))
			{
				CalculateLayout();
			}
		}
	}
	return hr;
}
void MainOO::DiscardGraphicsResources()
{
	SafeRelease(&pRenderTarget);
	SafeRelease(&pBrush);
	SafeRelease(&pStroke);
}
void MainOO::OnPaint()
{
	HRESULT hr = CreateGraphicsResources();
	if (SUCCEEDED(hr))
	{
		PAINTSTRUCT ps;
		BeginPaint(m_hwnd, &ps);
		pRenderTarget->BeginDraw();

		/*pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));
		pRenderTarget->FillEllipse(ellipse, pBrush);*/

		pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));
		pRenderTarget->FillEllipse(ellipse, pBrush);
		pRenderTarget->FillEllipse(ellipse2, pBrush);
		pRenderTarget->DrawEllipse(ellipse, pStroke);
		pRenderTarget->DrawEllipse(ellipse2, pStroke);
		// Draw hands
		SYSTEMTIME time;
		GetLocalTime(&time);
		// 60 minutes = 30 degrees, 1 minute = 0.5 degree
		const float fHourAngle = (360.0f / 12) * (time.wHour) + (time.wMinute * 0.5f);
		const float fMinuteAngle = (360.0f / 60) * (time.wMinute);
		const float fSecondAngle = (360.0f / 60) * (time.wSecond);

		DrawClockHand(0.6f, fHourAngle, 6);
		DrawClockHand(0.85f, fMinuteAngle, 4);
		DrawClockHand(0.90f, fSecondAngle, 2);
		// Restore the identity transformation.
		pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		hr = pRenderTarget->EndDraw();
		if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
		{
			DiscardGraphicsResources();
		}
		EndPaint(m_hwnd, &ps);
	}
}
void MainOO::Resize()
{
	if (pRenderTarget != NULL)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
		pRenderTarget->Resize(size);
		CalculateLayout();
		InvalidateRect(m_hwnd, NULL, FALSE);
	}
}
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
	MainOO win;
	if (!win.Create(L"Circle", WS_OVERLAPPEDWINDOW))
	{
		return 0;
	}
	ShowWindow(win.Window(), nCmdShow);
	// Run the message loop.
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
LRESULT MainOO::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		if (FAILED(D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
		{
			return -1; // Fail CreateWindowEx.
		}
		Timer = SetTimer(m_hwnd, 0, 1000, (TIMERPROC)NULL);
		DPIScale::Initialize(pFactory);
		return 0;
	case WM_DESTROY:
		DiscardGraphicsResources();
		SafeRelease(&pFactory);
		KillTimer(m_hwnd, Timer);
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		OnPaint();
		return 0;
	case WM_SIZE:
		Resize();
		return 0;
	case WM_TIMER:
		// process the 1-second timer
		PostMessage(m_hwnd, WM_PAINT, NULL, NULL);
		return 0;
	case WM_LBUTTONDOWN:
		OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
		return 0;
	case WM_LBUTTONUP:
		OnLButtonUp();
		return 0;
	case WM_MOUSEMOVE:
		OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
		return 0;
	}
	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

void MainOO::OnLButtonDown(int pixelX, int pixelY, DWORD flags)
{
	SetCapture(m_hwnd);
	ellipse2.point = ptMouse = DPIScale::PixelsToDips(pixelX, pixelY);
	ellipse2.radiusX = ellipse2.radiusY = 1.0f;
	InvalidateRect(m_hwnd, NULL, FALSE);
}

void MainOO::OnLButtonUp()
{
	ReleaseCapture ();
}

void MainOO::OnMouseMove(int pixelX, int pixelY, DWORD flags)
{
	if (flags & MK_LBUTTON)
	{
		const D2D1_POINT_2F dips = DPIScale::PixelsToDips(pixelX, pixelY);
		const float width = (dips.x - ptMouse.x) / 2;
		const float height = (dips.y - ptMouse.y) / 2;
		const float x1 = ptMouse.x + width;
		const float y1 = ptMouse.y + height;
		ellipse2 = D2D1::Ellipse(D2D1::Point2F(x1, y1), width, height);
		InvalidateRect(m_hwnd, NULL, FALSE);
	}
}

void MainOO::DrawClockHand(float fHandLength, float fAngle, float fStrokeWidth)
{
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(fAngle, ellipse.point));
	// endPoint defines one end of the hand.
	D2D_POINT_2F endPoint = D2D1::Point2F(ellipse.point.x,ellipse.point.y - (ellipse.radiusY * fHandLength));
	// Draw a line from the center of the ellipse to endPoint.
	pRenderTarget->DrawLine(ellipse.point, endPoint, pStroke, fStrokeWidth);
}
