#include <windows.h>
#include "beacon.h"
#define BUFSIZE 2048

WINBASEAPI HANDLE WINAPI KERNEL32$WaitNamedPipeA(LPCSTR lpNamedPipeName, DWORD nTimeOut);
WINBASEAPI HANDLE WINAPI KERNEL32$CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
WINBASEAPI BOOL WINAPI KERNEL32$ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
WINBASEAPI DWORD WINAPI KERNEL32$GetLastError (VOID);
WINBASEAPI VOID WINAPI KERNEL32$Sleep (DWORD);
WINBASEAPI void * WINAPI KERNEL32$VirtualAlloc (LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
WINBASEAPI LPSTR WINAPI KERNEL32$lstrcatA (LPSTR lpString1, LPCSTR lpString2);
WINBASEAPI int WINAPI KERNEL32$lstrlenA (LPSTR);
WINBASEAPI BOOL WINAPI KERNEL32$CloseHandle (HANDLE);

 
void go(char * args, int len) 
{
    //KERNEL32$Sleep(5000);
    char *bigbuffer = KERNEL32$VirtualAlloc(0, 0x4096, MEM_COMMIT, PAGE_READWRITE);
    char  chBuf[BUFSIZE]; 
    BOOL   fSuccess = FALSE; 
    DWORD  cbRead, cbToWrite, cbWritten, dwMode; 
    char pipename[] = "\\\\.\\pipe\\testpipe"; 
    HANDLE hPipe;
    int count = 1024;
    for (;;)
    {
        
        KERNEL32$WaitNamedPipeA(pipename,NMPWAIT_WAIT_FOREVER);
        hPipe=KERNEL32$CreateFileA(
            pipename,
            GENERIC_READ | GENERIC_WRITE,
            0,
            0,
            OPEN_EXISTING,
            0 ,
            0);

        if (hPipe != INVALID_HANDLE_VALUE)
            break;

        if (--count == 0)
            return;

    }

    count = 3;
    for(;;)
    {

        fSuccess = KERNEL32$ReadFile( 
            hPipe,    // pipe handle 
            chBuf,    // buffer to receive reply 
            BUFSIZE*sizeof(char),  // size of buffer 
            &cbRead,  // number of bytes read 
            NULL);    // not overlapped 

        if (!fSuccess || cbRead == 0)
        {   
            if (KERNEL32$GetLastError() == ERROR_BROKEN_PIPE)
            {
                BeaconPrintf(CALLBACK_ERROR, "[-] Client disconnected.\n"); 
            }
            else
            {
                BeaconPrintf(CALLBACK_OUTPUT, "[+] Pipe closed by module, GLE=%d.\n", KERNEL32$GetLastError()); 
            }
            break;
        }

        KERNEL32$lstrcatA(bigbuffer, chBuf);
        //BeaconPrintf(CALLBACK_OUTPUT, "Bytes read %d:", cbRead); 

        if ( KERNEL32$lstrlenA(bigbuffer) >= 0x4096 )
        {
            BeaconPrintf(CALLBACK_OUTPUT, bigbuffer); 
            bigbuffer[0] = '\0';
        }
        chBuf[0] = '\0';

    }

    BeaconPrintf(CALLBACK_OUTPUT, bigbuffer); 
    KERNEL32$CloseHandle(hPipe);

}