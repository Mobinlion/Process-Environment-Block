// PEB_ProcessEnvironmentBlock.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
using namespace std;
#include "windows.h"
#include "Winternl.h"

#define CUT_LIST(item) \
    item.Blink->Flink = item.Flink \
	item.Flink->Blink = item.Blink

#ifndef PEB_LDR_DATA
typedef struct _PEB_LDR_DATA
{
         ULONG Length;
         UCHAR Initialized;
         PVOID SsHandle;
         LIST_ENTRY InLoadOrderModuleList;
         LIST_ENTRY InMemoryOrderModuleList;
         LIST_ENTRY InInitializationOrderModuleList;
         PVOID EntryInProgress;
} PEB_LDR_DATA, *PPEB_LDR_DATA;
#endif

#ifndef LDR_MODULE
typedef struct _LDR_MODULE {
LIST_ENTRY InLoadOrderModuleList;  //<-- InLoad points here
LIST_ENTRY InMemoryOrderModuleList; //<-- PInMem points here
LIST_ENTRY InInitializationOrderModuleList;  //<-- InInitia points here
PVOID BaseAddress; 
PVOID EntryPoint; 
ULONG SizeOfImage; 
UNICODE_STRING FullDllName; 
UNICODE_STRING BaseDllName; 
ULONG Flags; 
SHORT LoadCount; 
SHORT TlsIndex; 
LIST_ENTRY HashTableEntry; 
ULONG TimeDateStamp;
} LDR_MODULE, *PLDR_MODULE;
#endif



typedef NTSTATUS (WINAPI *ZwQueryInformationProcess_mine)(
  _In_       HANDLE ProcessHandle,
  _In_       PROCESSINFOCLASS ProcessInformationClass,
  _Out_      PVOID ProcessInformation,
  _In_       ULONG ProcessInformationLength,
  _Out_opt_  PULONG ReturnLength
);

ZwQueryInformationProcess_mine info_proc;

int _tmain(int argc, _TCHAR* argv[])
{
	PPEB_LDR_DATA pebLdrData;
    PLDR_MODULE mod;

    __asm {
        mov eax, fs:[0x30]        //get PEB ADDR
        add eax, 0x0C        
        mov eax, [eax]            //get LoaderData ADDR
        mov pebLdrData, eax
    }

	mod = (PLDR_MODULE)pebLdrData->InLoadOrderModuleList.Flink;
	unsigned char floor[1024];
	printf("entry point: %p\r\n",mod->EntryPoint);
	
	printf("\r\nbase: %p \r\n\r\n",mod->BaseAddress);
	wcstombs ((char*)floor, mod->FullDllName.Buffer, mod->FullDllName.Length);
	printf("%s \r\n\r\n\r\n\r\n",floor);
	
	exit(1);
	getchar();
	//wcstombs ((char*)floor, mod->FullDllName.Buffer, mod->FullDllName.Length);
	//memcpy(floor, mod->FullDllName.Buffer, mod->FullDllName.Length);
	//printf("%s",floor);
	char *perc;
	 __asm {
        mov eax, fs:[0x04]        //get PEB ADDR
        //add eax, 0x0C        
        //mov eax, [eax]            //get LoaderData ADDR
        mov perc, eax
    }
	printf("\r\n%p\r\n", perc);
	
	 __asm {
        mov eax, fs:[0x08]        //get PEB ADDR
        //add eax, 0x0C        
        //mov eax, [eax]            //get LoaderData ADDR
        mov perc, eax
    }
	 printf("\r\n%p\r\n", perc);
	getchar();
	exit(1);


	//GetProcessId();
	HMODULE hdl;
	hdl = LoadLibraryA("NtDll.dll");
	if(hdl==NULL)
		cout<<"load failed";
	info_proc = (ZwQueryInformationProcess_mine)GetProcAddress(hdl,"ZwQueryInformationProcess");
	if(!info_proc)
		cout<<"getproc failed";

	PROCESS_BASIC_INFORMATION pr1;
	HANDLE hdlprocess = 0;
	hdlprocess = GetCurrentProcess();
	if(!hdlprocess)
		cout<<"getcurrentprocess failed";
	info_proc(hdlprocess, ProcessBasicInformation, (void *)&pr1, sizeof(pr1), NULL);

	PEB peb;
	cout<<sizeof(peb)<<endl;
	unsigned long int sizeread;
	ReadProcessMemory(hdlprocess, pr1.PebBaseAddress, &peb, 16, &sizeread);
		
	cout<<sizeread<<"  size read\r "<<endl;

	//ReadProcessMemory(hdlprocess, peb
	

	FreeLibrary(hdl);
	CloseHandle(hdlprocess);



	return 0;
}

