#include <windows.h>
#include "addresshunter.h"
#include "definitions.h"

#ifndef MSGSIZE
#define MSGSIZE 512
#endif



// kernel32.dll exports
typedef HLOCAL(WINAPI* LOCALFREE)(HLOCAL);
typedef HMODULE(WINAPI* LOADLIBRARYA)(LPCSTR);
typedef BOOL(WINAPI* WRITEFILE)(HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED);
typedef HANDLE(WINAPI* CREATENAMEDPIPEA)(LPCSTR,DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,LPSECURITY_ATTRIBUTES);
typedef BOOL(WINAPI* CONNECTNAMEDPIPE)(HANDLE,LPOVERLAPPED);
typedef BOOL(WINAPI* FLUSHFILEBUFFERS)(HANDLE);
typedef BOOL(WINAPI* DISCONNECTNAMEDPIPE)(HANDLE);
typedef BOOL(WINAPI* CLOSEHANDLE)(HANDLE);
typedef VOID(WINAPI* SLEEP)(DWORD);
typedef LPSTR(WINAPI* LSTRCATA)(LPSTR, LPSTR);
typedef int(WINAPI* LSTRLENA)(LPSTR);
typedef LPVOID(WINAPI* VIRTUALALLOC)(LPVOID, SIZE_T, DWORD, DWORD);

// user32.dll
typedef int(WINAPI* WSPRINTFA)(LPSTR, LPCSTR, ...);

// msvcrt.dll exports
typedef int(WINAPI* WPRINTF)(const wchar_t* format, ...);
typedef int(WINAPI* PRINTF)(const char* format, ...);
typedef void*(WINAPI* CALLOC)(size_t num, size_t size);
typedef void*(WINAPI* MEMCPY)(void*, const void*, size_t);

// Ws2_32.dll exports
typedef char*(WINAPI* INETNTOA)(IN_ADDR in);
typedef const char*(WINAPI* INETNTOP)(int, const void*, char*, size_t);

// Dnsapi.dll exports
typedef DNS_STATUS(WINAPI* DNSQUERY_W)(PCWSTR,WORD,DWORD,PVOID,PDNS_RECORD *,PVOID *);
typedef void(WINAPI* DNSRECORDLISTFREE)(PDNS_RECORD, DNS_FREE_TYPE);
typedef int(WINAPI* DNSGETCACHEDATATABLE)(PDNS_CACHE_ENTRY);



struct functionPointers
{
    LOCALFREE fnLocalFree;
    LOADLIBRARYA fnLoadLibrary;
    WRITEFILE  fnWriteFile;
    CREATENAMEDPIPEA fnCreateNamedPipeA;
    CONNECTNAMEDPIPE fnConnectNamedPipe;
    FLUSHFILEBUFFERS fnFlushFileBuffers;
    DISCONNECTNAMEDPIPE fnDisconnectNamedPipe;
    CLOSEHANDLE fnCloseHandle;
    SLEEP fnSleep;
    LSTRCATA fnLsrcatA;
    LSTRLENA fnLstrlenA;
    VIRTUALALLOC fnVirtuallAlloc;
    WSPRINTFA fnWsprintfA;
    WPRINTF fnWPrintf;
    PRINTF fnPrintf;
    CALLOC fnCalloc;
    MEMCPY fnMemcpy;
    INETNTOA fnInetntoa;
    INETNTOP fnInetntop;
    DNSQUERY_W fnDnsQuery_W;
    DNSRECORDLISTFREE fnDnsRecordListFree;
    DNSGETCACHEDATATABLE fnDnsGetCacheDataTable;
}; 



void loadFunctions(struct functionPointers* fnPtrs)
{

    UINT64 kernel32dll, msvcrtdll, dnsapidll, ws2_32dll, user32dll;
    
    // kernel32.dll exports
    kernel32dll = GetKernel32();

    char loadlibrarya_c[] = {'L', 'o', 'a', 'd', 'L', 'i', 'b', 'r', 'a', 'r', 'y', 'A', 0};
    fnPtrs->fnLoadLibrary = (void*)GetSymbolAddress((HANDLE)kernel32dll, loadlibrarya_c);

    char writefile_c[] = {'W', 'r', 'i', 't', 'e', 'F', 'i', 'l', 'e', 0};
    fnPtrs->fnWriteFile = (void*)GetSymbolAddress((HANDLE)kernel32dll, writefile_c);

    char createnamedpipea_c[] = { 'C', 'r', 'e', 'a', 't', 'e', 'N', 'a', 'm', 'e', 'd', 'P', 'i', 'p', 'e', 'A', 0 };
    fnPtrs->fnCreateNamedPipeA = (void*)GetSymbolAddress((HANDLE)kernel32dll, createnamedpipea_c);

    char connectnamedpipe_c[] = { 'C', 'o', 'n', 'n', 'e', 'c', 't', 'N', 'a', 'm', 'e', 'd', 'P', 'i', 'p', 'e', 0 };
    fnPtrs->fnConnectNamedPipe = (void*)GetSymbolAddress((HANDLE)kernel32dll, connectnamedpipe_c);

    char flushfilebuffers_c[] = { 'F', 'l', 'u', 's', 'h', 'F', 'i', 'l', 'e', 'B', 'u', 'f', 'f', 'e', 'r','s', 0 };
    fnPtrs->fnFlushFileBuffers = (void*)GetSymbolAddress((HANDLE)kernel32dll, flushfilebuffers_c);

    char disconnectnamedpipe_c[] = { 'D','i','s','c', 'o', 'n', 'n', 'e', 'c', 't', 'N', 'a', 'm', 'e', 'd', 'P', 'i', 'p', 'e', 0 };
    fnPtrs->fnDisconnectNamedPipe = (void*)GetSymbolAddress((HANDLE)kernel32dll, disconnectnamedpipe_c);

    char closehandle_c[] = {'C', 'l', 'o', 's', 'e', 'H', 'a', 'n', 'd','l','e', 0};
    fnPtrs->fnCloseHandle = (void*)GetSymbolAddress((HANDLE)kernel32dll, closehandle_c);
    
    char sleep_c[] = {'S', 'l', 'e', 'e', 'p', 0};
    fnPtrs->fnSleep = (void*)GetSymbolAddress((HANDLE)kernel32dll, sleep_c);

    char lstrcata_c[] = {'l', 's', 't', 'r', 'c', 'a', 't','A', 0};
    fnPtrs->fnLsrcatA = (void*)GetSymbolAddress((HANDLE)kernel32dll, lstrcata_c);

    char lstrlena_c[] = {'l', 's', 't', 'r', 'l', 'e', 'n','A', 0};
    fnPtrs->fnLstrlenA = (void*)GetSymbolAddress((HANDLE)kernel32dll, lstrlena_c);

    char virtualalloc_c[] = {'V', 'i', 'r', 't', 'u','a', 'l', 'A', 'l', 'l','o','c', 0};
    fnPtrs->fnVirtuallAlloc = (void*)GetSymbolAddress((HANDLE)kernel32dll, virtualalloc_c);

    //msvcrt.dll exports
    char msvcrt_c[] = {'m', 's', 'v', 'c', 'r', 't', '.', 'd', 'l', 'l', 0};
    msvcrtdll = (UINT64)((LOADLIBRARYA)fnPtrs->fnLoadLibrary)(msvcrt_c);

    char printf_c[] = {'p', 'r', 'i', 'n', 't', 'f', 0};
    fnPtrs->fnPrintf = (void*)GetSymbolAddress((HANDLE)msvcrtdll, printf_c);

    char calloc_c[] = { 'c', 'a', 'l', 'l', 'o', 'c', 0 };
    fnPtrs->fnCalloc = (void*)GetSymbolAddress((HANDLE)msvcrtdll, calloc_c);

    char memcpy_c[] = { 'm', 'e', 'm', 'c', 'p', 'y', 0 };
    fnPtrs->fnMemcpy = (void*)GetSymbolAddress((HANDLE)msvcrtdll, memcpy_c);

    char memset_c[] = { 'm', 'e', 'm', 's', 'e', 't', 0 };
    fnPtrs->fnMemcpy = (void*)GetSymbolAddress((HANDLE)msvcrtdll, memset_c);

    // Dnsapi.dll exports
    char dnsapi_c[] = { 'D', 'n', 's', 'a', 'p', 'i', '.', 'd', 'l', 'l', 0 };
    dnsapidll = (UINT64) ((LOADLIBRARYA)fnPtrs->fnLoadLibrary)(dnsapi_c);

    char dnsgetcachedatatable_c[] = { 'D', 'n', 's', 'G', 'e', 't', 'C', 'a', 'c', 'h', 'e', 'D', 'a', 't', 'a', 'T', 'a', 'b', 'l', 'e', 0 };
    fnPtrs->fnDnsGetCacheDataTable = (void*)GetSymbolAddress((HANDLE)dnsapidll, dnsgetcachedatatable_c);

    char dnsquery_w_c[] = { 'D', 'n', 's', 'Q', 'u', 'e', 'r', 'y', '_', 'W', 0 };
    fnPtrs->fnDnsQuery_W = (void*)GetSymbolAddress((HANDLE)dnsapidll, dnsquery_w_c);

    char dnsrecordlistfree_c[] = { 'D', 'n', 's', 'R', 'e', 'c', 'o', 'r', 'd', 'L', 'i', 's', 't', 'F', 'r', 'e', 'e', 0 };
    fnPtrs->fnDnsRecordListFree = (void*)GetSymbolAddress((HANDLE)dnsapidll, dnsrecordlistfree_c);

    // Ws2_32.dll exports
    char ws2_32_c[] = { 'W', 's', '2', '_', '3', '2', '.', 'd', 'l', 'l', 0 };
    ws2_32dll = (UINT64) ((LOADLIBRARYA)fnPtrs->fnLoadLibrary)(ws2_32_c);
    
    char inettoa_c[] = { 'i', 'n', 'e', 't', '_', 'n', 't', 'o', 'a', 0 };
    fnPtrs->fnInetntoa = (void*)GetSymbolAddress((HANDLE)ws2_32dll, inettoa_c);

    char inet_ntop_c[] = { 'i', 'n', 'e', 't', '_', 'n', 't', 'o', 'p', 0 }; 
    fnPtrs->fnInetntop = (void*)GetSymbolAddress((HANDLE)ws2_32dll, inet_ntop_c);

    // User32.dll
    char user32_c[] = { 'U', 's', 'e', 'r', '3', '2', '.', 'd', 'l', 'l', 0 };
    user32dll = (UINT64) ((LOADLIBRARYA)fnPtrs->fnLoadLibrary)(user32_c);

    char wsprintfA_c[] = { 'w', 's', 'p', 'r', 'i', 'n', 't', 'f', 'A', 0 };
    fnPtrs->fnWsprintfA = (void*)GetSymbolAddress((HANDLE)user32dll, wsprintfA_c);

    return;

}


HANDLE createPipe(struct functionPointers* fnPtrs)
{

    char szPipename[] = { '\\', '\\', '.', '\\','p', 'i', 'p', 'e', '\\', 't', 'e', 's', 't', 'p', 'i', 'p', 'e', 0 };
    HANDLE hPipe = ((CREATENAMEDPIPEA)(fnPtrs->fnCreateNamedPipeA))(
            szPipename,             // pipe name 
            PIPE_ACCESS_DUPLEX | FILE_FLAG_FIRST_PIPE_INSTANCE,       // read/write access 
            PIPE_TYPE_MESSAGE |       // message type pipe 
            PIPE_READMODE_MESSAGE |   // message-read mode 
            PIPE_WAIT,                // blocking mode 
            1, // max. instances  
            MSGSIZE,                  // output buffer size 
            MSGSIZE,                  // input buffer size 
            0,                        // client time-out 
            NULL);

        if (hPipe == INVALID_HANDLE_VALUE) 
            return NULL;
        
        while (!((CONNECTNAMEDPIPE)(fnPtrs->fnConnectNamedPipe))(hPipe,0));

        return hPipe;

}

void closePipe(struct functionPointers* fnPtrs, HANDLE hPipe, PDWORD dwBytesWritten)
{

    ((FLUSHFILEBUFFERS)fnPtrs->fnFlushFileBuffers)(hPipe);
    ((DISCONNECTNAMEDPIPE)fnPtrs->fnDisconnectNamedPipe)(hPipe);
    ((CLOSEHANDLE)fnPtrs->fnCloseHandle)(hPipe);

    return;

}

void printOutput(struct functionPointers* fnPtrs, HANDLE hPipe, char* szValueBuffer, char* format, void* szValue) 
{

    DWORD dwBytesWritten;
    ((WSPRINTFA)(fnPtrs->fnWsprintfA))(szValueBuffer, format, szValue);
    ((WRITEFILE)fnPtrs->fnWriteFile)(hPipe, szValueBuffer, 512, &dwBytesWritten, 0);
    szValueBuffer[0] = '\0';
    
    return;

}