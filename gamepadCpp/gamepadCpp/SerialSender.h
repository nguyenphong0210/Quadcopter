#include <iostream>
#include <windows.h>

class SerialSender {
public:
    SerialSender(const char* portName) : portName(portName), hSerial(INVALID_HANDLE_VALUE) {}

    ~SerialSender() {
        if (hSerial != INVALID_HANDLE_VALUE) {
            CloseHandle(hSerial);
        }
    }

    bool open() {
        hSerial = CreateFileA(portName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
        if (hSerial == INVALID_HANDLE_VALUE) {
            std::cout << "Error: Could not open serial port." << std::endl;
            return false;
        }

        DCB dcbSerialParams = { 0 };
        dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
        if (!GetCommState(hSerial, &dcbSerialParams)) {
            std::cout << "Error: Could not get serial port state." << std::endl;
            CloseHandle(hSerial);
            return false;
        }

        dcbSerialParams.BaudRate = CBR_9600;
        dcbSerialParams.ByteSize = 8;
        dcbSerialParams.StopBits = ONESTOPBIT;
        dcbSerialParams.Parity = NOPARITY;
        if (!SetCommState(hSerial, &dcbSerialParams)) {
            std::cout << "Error: Could not set serial port parameters." << std::endl;
            CloseHandle(hSerial);
            return false;
        }

        return true;
    }

    bool send(const void* data, size_t dataSize) {
        if (hSerial == INVALID_HANDLE_VALUE) {
            std::cout << "Error: Serial port is not open." << std::endl;
            return false;
        }

        DWORD bytesWritten;
        if (!WriteFile(hSerial, data, dataSize, &bytesWritten, NULL)) {
            std::cout << "Error: Could not write to serial port." << std::endl;
            return false;
        }

        return true;
    }

private:
    const char* portName;
    HANDLE hSerial;
};
