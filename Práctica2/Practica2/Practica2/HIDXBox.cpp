#include "HIDXBox.h"


HIDXBox::HIDXBox() : BaseHID()
{
}

HIDXBox::HIDXBox(float t) : BaseHID(t)
{
}


HIDXBox::~HIDXBox()
{
}

bool HIDXBox::LeeMando()
{
	XBox.dwResult == XInputGetState(0, &XBox.State);
	return (XBox.dwResult == ERROR_SUCCESS);
}

void HIDXBox::EscribeMando()
{
	XBox.vibration.wLeftMotorSpeed = (WORD)(fLeftVibration * MAXUINT16);
	XBox.vibration.wRightMotorSpeed = (WORD)(fRightVibration * MAXUINT16);
	XInputSetState(0, &XBox.vibration);
}

void HIDXBox::Mando2HID()
{
	wButtons = XBox.State.Gamepad.wButtons;
	fLeftTrigger = (float)XBox.State.Gamepad.bLeftTrigger / (float)MAXBYTE;
	fRightTrigger = (float)XBox.State.Gamepad.bRightTrigger / (float)MAXBYTE;


	//Zona muerta
	if (XBox.State.Gamepad.sThumbLX > INPUT_DEADZONE)
		XBox.State.Gamepad.sThumbLX -= INPUT_DEADZONE;
	else if (XBox.State.Gamepad.sThumbLX < -INPUT_DEADZONE)
		XBox.State.Gamepad.sThumbLX += INPUT_DEADZONE;
	else
		XBox.State.Gamepad.sThumbLX = 0;

	if (XBox.State.Gamepad.sThumbLY > INPUT_DEADZONE)
		XBox.State.Gamepad.sThumbLY -= INPUT_DEADZONE;
	else if (XBox.State.Gamepad.sThumbLY < -INPUT_DEADZONE)
		XBox.State.Gamepad.sThumbLY += INPUT_DEADZONE;
	else
		XBox.State.Gamepad.sThumbLY = 0;

	if (XBox.State.Gamepad.sThumbRX > INPUT_DEADZONE)
		XBox.State.Gamepad.sThumbRX -= INPUT_DEADZONE;
	else if (XBox.State.Gamepad.sThumbRX < -INPUT_DEADZONE)
		XBox.State.Gamepad.sThumbRX += INPUT_DEADZONE;
	else
		XBox.State.Gamepad.sThumbRX = 0;

	if (XBox.State.Gamepad.sThumbRY > INPUT_DEADZONE)
		XBox.State.Gamepad.sThumbRY -= INPUT_DEADZONE;
	else if (XBox.State.Gamepad.sThumbRY < -INPUT_DEADZONE)
		XBox.State.Gamepad.sThumbRY += INPUT_DEADZONE;
	else
		XBox.State.Gamepad.sThumbRY = 0;

	//Normalizamos
	fThumbLX = (float)XBox.State.Gamepad.sThumbLX / (float)(MAXINT16 - INPUT_DEADZONE);
	fThumbLY = (float)XBox.State.Gamepad.sThumbLY / (float)(MAXINT16 - INPUT_DEADZONE);
	fThumbRX = (float)XBox.State.Gamepad.sThumbRX / (float)(MAXINT16 - INPUT_DEADZONE);
	fThumbRY = (float)XBox.State.Gamepad.sThumbRY / (float)(MAXINT16 - INPUT_DEADZONE);
}

void HIDXBox::Calibra()
{
	if (LeeMando())
		XBox.OffState = XBox.State;
}
