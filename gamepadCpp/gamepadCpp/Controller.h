#pragma once

#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class Controller
{
private:
    IDirectInputDevice8* controller;
    DIJOYSTATE state;

public:
    Controller(IDirectInput8* dinput);

    ~Controller();

    void getState() {
        controller->GetDeviceState(sizeof(DIJOYSTATE), &state);
    }

    int getLeftStickX() {
        return state.lX;
    }

    int getLeftStickY() {
        return state.lY;
    }

    int getrlSwitch() {
        return state.lZ;
    }

    int getRightStickX() {
        return state.lRx;
    }

    int getRightStickY() {
        return state.lRy;
    }

    bool getButtonA() {
        return state.rgbButtons[0] & 0x80;
    }

    bool getButtonB() {
        return state.rgbButtons[1] & 0x80;
    }

    bool getButtonX() {
        return state.rgbButtons[2] & 0x80;
    }

    bool getButtonY() {
        return state.rgbButtons[3] & 0x80;
    }
};

