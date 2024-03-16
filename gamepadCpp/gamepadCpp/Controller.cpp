#include "Controller.h"

Controller::Controller(IDirectInput8* dinput)
{
    dinput->CreateDevice(GUID_Joystick, &controller, NULL);
    controller->SetDataFormat(&c_dfDIJoystick);
    controller->SetCooperativeLevel(NULL, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
    controller->Acquire();
}

Controller::~Controller()
{
    controller->Unacquire();
    controller->Release();
}
