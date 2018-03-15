#include "Scene.h"


Scene::Scene():pBrush(NULL), pRenderTarget(NULL), pFactory(NULL)
{

}


Scene::~Scene()
{
}


HRESULT Scene::CreaFactoria()
{
	return	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);
}

void Scene::LiberaFactoria()
{
	SafeRelease(&pFactory);
}

void Scene::DrawClockHand(float fHandLength, float fAngle, float fStrokeWidth)
{
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(fAngle, ellipse.point));
	// endPoint defines one end of the hand.
	D2D_POINT_2F endPoint = D2D1::Point2F(ellipse.point.x,ellipse.point.y - (ellipse.radiusY * fHandLength));
	// Draw a line from the center of the ellipse to endPoint.
	pRenderTarget->DrawLine(ellipse.point, endPoint, pStroke, fStrokeWidth);
}

void Scene::DrawEsfera()
{
	pRenderTarget->FillEllipse(ellipse2, pBrush2);
	pRenderTarget->DrawEllipse(ellipse2, pStroke);
}
void Scene::DrawReloj()
{
	//ellipse2.point.x = rx;
	//ellipse2.point.y = ry;
	//ellipse2.radiusX = rr;
	//ellipse2.radiusY = rr;
	ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radio, radio);
	pRenderTarget->FillEllipse(ellipse, pBrush);
	pRenderTarget->DrawEllipse(ellipse, pStroke);
	DrawClockHand(0.6f, fHourAngle, 6);
	DrawClockHand(0.8f, fMinuteAngle, 4);
	DrawClockHand(0.9f, fSecondAngle, 2);
}
void Scene::RenderScene()
{
	pRenderTarget->BeginDraw();
	pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::DeepSkyBlue));
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	DrawEsfera();
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	DrawReloj();

	HRESULT hr = pRenderTarget->EndDraw();
	if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
	{
		DiscardGraphicsResources();
	}
}
bool Scene::HitTest(D2D1_ELLIPSE ellipse, float x, float y)
{
	const float a = ellipse.radiusX;
	const float b = ellipse.radiusY;
	const float x1 = x - ellipse.point.x;
	const float y1 = y - ellipse.point.y;
	const float d = ((x1 * x1) / (a * a)) + ((y1 * y1) / (b * b));
	return d <= 1.0f;
}
void Scene::ActualizaElipse(float x, float y, float width, float height)
{
	ellipse2.point.x = x; ellipse2.point.y = y;
	ellipse2 = D2D1::Ellipse(D2D1::Point2F(x, y), width, height);
}
// Recalculate drawing layout when the size of the window changes.

void Scene::CalculateLayout(RECT rc)
{
	D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
	if (pRenderTarget != NULL)
	{
		pRenderTarget->Resize(size);
		D2D1_SIZE_F size = pRenderTarget->GetSize();
		x = size.width / 16;
		y = size.height / 8;
		radio = min(x, y);
	}
}

HRESULT Scene::CreateGraphicsResources(HWND m_hwnd,RECT rc)
{
	HRESULT hr = S_OK;
	if (pRenderTarget == NULL)
	{
		
		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		hr = pFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&pRenderTarget);

		if (SUCCEEDED(hr))
		{
			const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0);
			hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);
			const D2D1_COLOR_F color1 = D2D1::ColorF(0, 0, 0);
			hr = pRenderTarget->CreateSolidColorBrush(color1, &pStroke);
			const D2D1_COLOR_F color2 = D2D1::ColorF(1.0f, 0.0f, 0.0f);
			hr = pRenderTarget->CreateSolidColorBrush(color2, &pBrush2);
			if (SUCCEEDED(hr))
			{
				CalculateLayout(rc);
			}
		}
	}
	return hr;
}

void Scene::DiscardGraphicsResources()
{
	SafeRelease(&pRenderTarget);
	SafeRelease(&pBrush);
	SafeRelease(&pStroke);
	SafeRelease(&pBrush2);
}