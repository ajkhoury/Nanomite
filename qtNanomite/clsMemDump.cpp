#include "clsMemDump.h"

clsMemDump::clsMemDump(HANDLE hProc, PTCHAR FileBaseName, DWORD64 BaseOffset, DWORD64 Size)
{
	DWORD	OldProtection	= NULL,
			NewProtection	= 0x40,
			BytesReaded		= NULL;

	if(!VirtualProtectEx(hProc,(LPVOID)BaseOffset,Size,NewProtection,&OldProtection))
	{
		MessageBoxW(NULL,L"Failed to access Memory!",L"Nanomite",MB_OK);
		return;
	}
	
	LPVOID pBuffer = malloc(Size);
	if(!ReadProcessMemory(hProc,(LPVOID)BaseOffset,pBuffer,Size,(size_t*)&BytesReaded))
	{
		MessageBoxW(NULL,L"Failed to read Memory!",L"Nanomite",MB_OK);
		free(pBuffer);
		return;
	}
	
	PTCHAR FileName = (PTCHAR)malloc(MAX_PATH * sizeof(TCHAR));
	wsprintf(FileName,L"%s_%016I64X-%016I64X_%08X.bin",FileBaseName,BaseOffset,BaseOffset + Size,Size);

	HANDLE hFile = CreateFile(FileName, GENERIC_READ | GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		MessageBoxW(NULL,L"Failed to create File!",L"Nanomite",MB_OK);
		free(FileName);
		free(pBuffer);
		return;
	}

	if(!WriteFile(hFile,pBuffer,Size,&BytesReaded,NULL))
		MessageBoxW(NULL,L"Failed to write to File!",L"Nanomite",MB_OK);

	free(FileName);
	free(pBuffer);
	CloseHandle(hFile);

	MessageBoxW(NULL,L"Memory Dump finished!",L"Nanomite",MB_OK);
}

clsMemDump::~clsMemDump()
{

}