#include <windows.h>
#include <dinput.h>
#include <iostream>
#include "Controller.h"
#include "SerialSender.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#define BUTTON_A_ON (1 << 7)  // Bit thứ 7
#define BUTTON_A_OFF ~(1 << 7)  // Bit thứ 7
#define BUTTON_B_ON (1 << 6)  // Bit thứ 6
#define BUTTON_B_OFF ~(1 << 6)  // Bit thứ 6
#define BUTTON_X_ON (1 << 5)  // Bit thứ 5
#define BUTTON_X_OFF ~(1 << 5)  // Bit thứ 5
#define BUTTON_Y_ON (1 << 4)  // Bit thứ 4
#define BUTTON_Y_OFF ~(1 << 4)  // Bit thứ 4

struct Data {
    int8_t leftStickX;
    int8_t leftStickY;
    int8_t rightStickX;
    int8_t rightStickY;
    int8_t rightleftSwitch;
    int8_t button;
};

int8_t transData(int input)
{
    int minValue = 0;
    int maxValue = 65534;
    int minResult = -126;
    int maxResult = 126;
    return (minResult + ((maxResult - minResult) * (input - minValue)) / (maxValue - minValue));
}

int main() {
    SerialSender sender("COM4");

    if (!sender.open()) {
        return 1;
    }

    Data dataToSend;

    // Initialize COM
    CoInitialize(NULL);

    // Initialize DirectInput
    IDirectInput8* dinput;
    DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, NULL);

    // Initialize Xbox controller
    Controller xboxController(dinput);


    dataToSend.leftStickX = 10;



    while (true)
    {
        // Get data from the controller
        xboxController.getState();

        dataToSend.leftStickX = transData(xboxController.getLeftStickX());

        std::cout << dataToSend.leftStickX << std::endl;

        dataToSend.leftStickY = transData(xboxController.getLeftStickY());

        dataToSend.rightStickX = transData(xboxController.getRightStickX());
        dataToSend.rightStickY = transData(xboxController.getRightStickY());

        dataToSend.rightleftSwitch = transData(xboxController.getrlSwitch());

        if (xboxController.getButtonA() == true)
        {
            dataToSend.button = dataToSend.button | BUTTON_A_ON;
        }
        else
        {
            dataToSend.button = dataToSend.button & BUTTON_A_OFF;
        }

        if (xboxController.getButtonB() == true)
        {
            dataToSend.button = dataToSend.button | BUTTON_B_ON;
        }
        else
        {
            dataToSend.button = dataToSend.button & BUTTON_B_OFF;
        }

        if (xboxController.getButtonX() == true)
        {
            dataToSend.button = dataToSend.button | BUTTON_X_ON;
        }
        else
        {
            dataToSend.button = dataToSend.button & BUTTON_X_OFF;
        }

        if (!sender.send(&dataToSend, sizeof(Data))) {
            return 1;
        }
        Sleep(60);
    }

    std::cout << "break" << std::endl;

    return 0;
}
