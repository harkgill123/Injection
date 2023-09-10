#include <Windows.h>
#include <stdio.h>

int main(void){

    STARTUPINFO startupInfo = { 0 };
    PROCESS_INFORMATION processInfo = { 0 };

    if(!CreateProcessW(
        L"C:\\Windows\\System32\\notepad.exe",
        NULL,
        NULL,
        NULL,
        FALSE,
        BELOW_NORMAL_PRIORITY_CLASS,
        NULL,
        NULL,
        &startupInfo,
        &processInfo
    )){
        printf("(-) failed to create process, error: %ld", GetLastError());
        return EXIT_FAILURE;
    }
    printf("(+) Process succesfully created, PID: %ld", processInfo.dwProcessId);
    return EXIT_SUCCESS;
    }