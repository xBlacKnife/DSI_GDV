#include <windows.h>
#include "basewin.h"
#include "Scene.h"
#include "WindowsX.h"

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


enum class EditionMode { SelectMode, DrawMode, DragMode };

class MainWindow : public BaseWindow<MainWindow>
{
	Scene   Escena;			//Objeto que gestiona la Escena
	RECT	AreaCliente;	//Area Cliente
	UINT_PTR Timer;			//Timer

	int OnCreate();
	void OnDestroy();
	void OnPaint();
	void OnResize();
	void OnTime();
	void OnLButtonDown(int pixelX, int pixelY, DWORD flags);
	void OnLButtonUp();
	void OnRButtonDown(int pixelX, int pixelY, DWORD flags);
	void OnRButtonUp();
	void OnMouseMove(int pixelX, int pixelY, DWORD flags);
	void OnKeyDown(WPARAM wParam);
	void PonCursor(EditionMode m);

	EditionMode mode = EditionMode::SelectMode;

public:
	MainWindow() 	{}
	PCWSTR  ClassName() const { return L"Circle Window Class"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};


int MainWindow::OnCreate()
{
	GetClientRect(m_hwnd, &AreaCliente);
	if (FAILED(Escena.CreaFactoria()))
	{
		return -1;  // Fail CreateWindowEx.
	}
	DPIScale::Initialize(Escena.pFactory);

	//Establezcpo un Timer T=1s; 
	Timer = SetTimer(m_hwnd, 0, 1000, (TIMERPROC)NULL);
	return 0;
}

void MainWindow::OnDestroy()
{
	Escena.DiscardGraphicsResources();
	Escena.LiberaFactoria();
	KillTimer(m_hwnd, Timer);
	PostQuitMessage(0);
}

void MainWindow::OnPaint()
{
	if (SUCCEEDED(Escena.CreateGraphicsResources(m_hwnd, AreaCliente)))
	{
		PAINTSTRUCT ps;
		BeginPaint(m_hwnd, &ps);
		Escena.RenderScene();
		EndPaint(m_hwnd, &ps);
	}
}

void MainWindow::OnResize()
{
	if (Escena.pRenderTarget != NULL)
	{
		GetClientRect(m_hwnd, &AreaCliente);
		Escena.CalculateLayout(AreaCliente);
		InvalidateRect(m_hwnd, NULL, FALSE);
	}
}

void MainWindow::OnTime()
{
	SYSTEMTIME	time;
	GetLocalTime(&time);		
	//Actualizo el tiempo de la escena
	Escena.Actualizar(time.wHour, time.wMinute, time.wSecond);
}

void MainWindow::OnLButtonDown(int pixelX, int pixelY, DWORD flags)
{
	const D2D1_POINT_2F dips = DPIScale::PixelsToDips(pixelX, pixelY);
	switch (mode)
	{
	case EditionMode::SelectMode:
	{
		if (Escena.HitTest(Escena.ellipse2, (float)dips.x, (float)dips.y))
		{
			mode = EditionMode::DragMode;
			PonCursor(EditionMode::DragMode);
			Escena.ptMouse = dips;
		}
		else
		{
			mode = EditionMode::DrawMode;
			PonCursor(EditionMode::DrawMode);
			Escena.ptMouse = dips;
			//Escena.ActualizaElipse(dips.x, dips.y, 1.0, 1.0);
			InvalidateRect(m_hwnd, NULL, FALSE);
		}
		break;
	}
	case EditionMode::DrawMode:
	{
		SetCapture(m_hwnd);
		Escena.ActualizaElipse(dips.x, dips.y, 1.0, 1.0);
		InvalidateRect(m_hwnd, NULL, FALSE);
		break;
	}
	case EditionMode::DragMode:
	{
		Escena.ptMouse = dips;
		break;
	}
	default:
		break;
	}
}

void MainWindow::OnLButtonUp()
{
	ReleaseCapture();
	mode = EditionMode::SelectMode;
	PonCursor(EditionMode::SelectMode);
}

void MainWindow::OnRButtonDown(int pixelX, int pixelY, DWORD flags)
{
	if (Escena.HitTest(Escena.ellipse2, pixelX, pixelY))
	{
		CHOOSECOLOR cc; // common dialog box structure
		static COLORREF acrCustClr[16]; // array of custom colors
		static DWORD rgbCurrent; // initial color selection
								 // Initialize CHOOSECOLOR
		ZeroMemory(&cc, sizeof(cc));
		cc.lStructSize = sizeof(cc);
		cc.hwndOwner = m_hwnd;
		cc.lpCustColors = (LPDWORD)acrCustClr;
		cc.rgbResult = rgbCurrent;
		cc.Flags = CC_FULLOPEN | CC_RGBINIT;
		if (ChooseColor(&cc) == TRUE)
		{
			Escena.pBrush2->SetColor(D2D1::ColorF((float)GetRValue(cc.rgbResult) / 255, (float)GetGValue(cc.rgbResult) / 255, (float)GetBValue(cc.rgbResult) / 255, 1.0f));
		}
	}
}

void MainWindow::OnRButtonUp()
{
	ReleaseCapture();
}

void MainWindow::OnMouseMove(int pixelX, int pixelY, DWORD flags) {

	LPWSTR cursor;
	HCURSOR hCursor;
	const D2D1_POINT_2F dips = DPIScale::PixelsToDips(pixelX, pixelY);
	const float width = (dips.x - Escena.ptMouse.x);
	const float height = (dips.y - Escena.ptMouse.y);
	const float x1 = Escena.ptMouse.x + width / 2;
	const float y1 = Escena.ptMouse.y + height / 2;

	switch (mode)
	{
	case EditionMode::SelectMode:
	{
		if (Escena.HitTest(Escena.ellipse2, (float)dips.x, (float)dips.y))
			PonCursor(EditionMode::DragMode);
		else
			PonCursor(EditionMode::SelectMode);
		break;
	}
	case EditionMode::DrawMode:
	{
		PonCursor(EditionMode::DrawMode);
		Escena.ActualizaElipse(x1, y1, width / 2, height / 2);
		InvalidateRect(m_hwnd, NULL, FALSE);
		break;
	}
	case EditionMode::DragMode:
	{
		Escena.ActualizaElipse(dips.x, dips.y, Escena.ellipse2.radiusX, Escena.ellipse2.radiusY);
		Escena.ptMouse = dips;
		InvalidateRect(m_hwnd, NULL, FALSE);
		break;
	}
	default:
		break;
	}
}

void MainWindow::OnKeyDown(WPARAM wParam)
{
	switch (wParam)
	{
	case VK_SPACE:
		Escena.CambiaModo();
		break;

	case VK_ESCAPE:
		OnDestroy();
		break;
	}
}

void MainWindow::PonCursor(EditionMode m)
{
	LPWSTR cursor = IDC_ARROW;
	HCURSOR hCursor;

	if (m == EditionMode::SelectMode)
		cursor = IDC_ARROW;
	else if (m == EditionMode::DrawMode)
		cursor = IDC_CROSS;
	else if (m == EditionMode::DragMode)
		cursor = IDC_HAND;

	hCursor = LoadCursor(NULL, cursor);
	SetCursor(hCursor);

}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
	MainWindow win;
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

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		return OnCreate();

	case WM_TIMER:
		// process the 1-second timer 
		OnTime();
		PostMessage(m_hwnd, WM_PAINT, NULL, NULL);
		return 0;

	case WM_DESTROY:
		OnDestroy();
		return 0;

	case WM_PAINT:
		OnPaint();
		return 0;

	case WM_SIZE:
		OnResize();
		return 0;

	case WM_LBUTTONDOWN:
		OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
		return 0;

	case WM_LBUTTONUP:
		OnLButtonUp();
		return 0;

	case WM_RBUTTONDOWN:
		OnRButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
		return 0;

	case WM_RBUTTONUP:
		OnRButtonUp();
		return 0;

	case WM_MOUSEMOVE:
		OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
		return 0;

	case WM_KEYDOWN:
		OnKeyDown(wParam);
		return 0;
	}
	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}