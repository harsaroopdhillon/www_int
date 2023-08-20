#pragma warning(disable:4201)

#include <Windows.h>
#include<winternl.h>
#include <winnt.h>
#include <stdio.h>
typedef struct _EXPORT_DIRECTORY_TABLE {
	DWORD ExportFlags;
	DWORD TimeStamp;
	WORD MajorVersion;
	WORD MinorVersion;
	DWORD NameRVA;
	DWORD OrdinalBase;
	DWORD ExportAddressTableSize;
	DWORD NamePointerTableSize;
	DWORD ExportAddressTableRVA;
	DWORD NamePointerTableRVA;
	DWORD OrdinalTableRVA;
} EXPORT_DIRECTORY_TABLE, *PEXPORT_DIRECTORY_TABLE;


extern INT _strcmp(LPCSTR s1, LPCSTR s2);
extern INT _wcscmp(LPCWSTR s1, LPCWSTR s2);
extern BOOL BlockFile(LPCWSTR pwszSourceFile);

PVOID My_GetModuleBase(LPCWSTR MODULE) {
	PVOID Current_peb = NtCurrentTeb()->ProcessEnvironmentBlock;
	PPEB peb = (PPEB)Current_peb;
	PLIST_ENTRY tail = &peb->Ldr->InMemoryOrderModuleList;
	PLIST_ENTRY list = tail->Flink;
	do {
		PLDR_DATA_TABLE_ENTRY module = (PLDR_DATA_TABLE_ENTRY)list;
		if (!_wcscmp(module->FullDllName.Buffer, MODULE)) {
			PVOID BaseAddr = module->Reserved2[0];
			//if(!BlockFile(module->FullDllName.Buffer)) {
			//	return NULL;
		//	}
			return BaseAddr;
		}
		list = list->Flink;
	} while (list != tail);
	return NULL;
}

PVOID My_GetModuleProcedureAddress(PVOID MODULE, LPCSTR RoutineName) {
	PIMAGE_DOS_HEADER ImgDosHdrs = (PIMAGE_DOS_HEADER)MODULE;
	if (ImgDosHdrs->e_magic != IMAGE_DOS_SIGNATURE)
		return NULL;
	PIMAGE_NT_HEADERS32 ImgNtHdrs32 = (PIMAGE_NT_HEADERS32)((PCHAR)MODULE + ImgDosHdrs->e_lfanew);
	if (ImgNtHdrs32->Signature != IMAGE_NT_SIGNATURE)
		return NULL;
	if (ImgNtHdrs32->FileHeader.SizeOfOptionalHeader < 96 || ImgNtHdrs32->OptionalHeader.NumberOfRvaAndSizes == 0)
		return NULL;

	DWORD ExportTable = ImgNtHdrs32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
	if (!ExportTable)
		return NULL;

	PEXPORT_DIRECTORY_TABLE pExportTable = (PEXPORT_DIRECTORY_TABLE)((PCHAR)MODULE + ExportTable);
	PVOID OrdinalTable = (PBYTE)MODULE + pExportTable->OrdinalTableRVA;
	PVOID NamePointerTable = (PBYTE)MODULE + pExportTable->NamePointerTableRVA;
	PVOID ExportAddressTable = (PBYTE)MODULE + pExportTable->ExportAddressTableRVA;

	for (DWORD i = 0; i < pExportTable->NamePointerTableSize; i++) {
		DWORD NameRVA = ((PDWORD)NamePointerTable)[i];
		const PCHAR NameAddr = (PCHAR)MODULE + NameRVA;

		if (_strcmp(NameAddr, RoutineName))
			continue;

		WORD od = ((PWORD)OrdinalTable)[i] + (WORD)pExportTable->OrdinalBase;
		WORD RealOrdinal = od - (WORD)pExportTable->OrdinalBase;
		DWORD ExportAddr = 0;
		ExportAddr = ((PDWORD)ExportAddressTable)[RealOrdinal];
		PVOID RoutineAddr = (PCHAR)MODULE + ExportAddr;
		return RoutineAddr;
	}
	return NULL;
}
