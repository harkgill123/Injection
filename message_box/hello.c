#include <Windows.h>

int main(){

    MessageBoxW(
        NULL,
        L"Hi my Name is Harkirat",
        L"NewMessageBox",
        MB_YESNOCANCEL | MB_ICONASTERISK
    );

    return 0;
}