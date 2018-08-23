// PE_HEADER.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include "iostream"
using namespace std;

#ifndef windows_H
#define windows_H
#endif

int _tmain(int argc, _TCHAR* argv[])
{

	PIMAGE_DOS_HEADER dosheader;
	PIMAGE_NT_HEADERS ntheader;
	PIMAGE_EXPORT_DIRECTORY exportheader;
	PIMAGE_SECTION_HEADER secheader;
	PIMAGE_IMPORT_DESCRIPTOR importheader;
	//IMAGE_DIRECTORY_ENTRY_IMPORT import;
	PIMAGE_THUNK_DATA thunkdata;
	PIMAGE_IMPORT_BY_NAME byname;

	HANDLE hdl = GetModuleHandle(NULL);

	dosheader = (PIMAGE_DOS_HEADER) hdl;
	printf("%p\r\n\r\n",hdl);
	
	ntheader = (PIMAGE_NT_HEADERS)((char*)hdl + dosheader->e_lfanew);
	//printf("%X", ntheader->OptionalHeader.);
	//cout<<sizeof(IMAGE_NT_HEADERS);
	secheader = (PIMAGE_SECTION_HEADER) ((unsigned char*)(ntheader)+ sizeof(IMAGE_NT_HEADERS));
	cout<<secheader->Name;
	secheader++;
	cout<<endl<<secheader->Name;
	cout<<endl<<secheader->SizeOfRawData;
	secheader++;
	cout<<endl<<secheader->Name;
	cout<<endl<<secheader->SizeOfRawData;
	return 1;

	importheader = (PIMAGE_IMPORT_DESCRIPTOR) ((ntheader->OptionalHeader.DataDirectory[1].VirtualAddress) + (char*)(hdl));
	
	thunkdata = (PIMAGE_THUNK_DATA)(importheader->OriginalFirstThunk + (char *)hdl);
	byname = (PIMAGE_IMPORT_BY_NAME)(thunkdata->u1.AddressOfData + (char*)hdl);
	
	printf("itsa mea mario: %p",*byname);
	//cout<<"does it work:  "<<(*(unsigned int*)byname>>31);  //find if ordinal number or function name

	cout<<byname->Name;
	cout<<endl<<byname->Hint;
	thunkdata++;
	byname = (PIMAGE_IMPORT_BY_NAME)(thunkdata->u1.AddressOfData + (char*)hdl);
	cout<<endl<<byname->Name;
	printf("\r\n%x\r\n", byname->Hint);
	exit(1);






	printf("\r\n\r\n\r\n%p\r\n", importheader->OriginalFirstThunk);
	printf("\r\n\r\n\r\n%x\r\n\r\n", importheader->FirstThunk);
	printf("%x" , (ntheader->OptionalHeader.DataDirectory[1].VirtualAddress));
	printf("\r\n%x" , (ntheader->OptionalHeader.DataDirectory[1].Size));

	









	//thunkdata = (PIMAGE_THUNK_DATA)((importheader->OriginalFirstThunk) + (char*)(hdl));

	//cout<<endl<<thunkdata->u1.Function;
	
	//secheader = (PIMAGE_SECTION_HEADER)ntheader->OptionalHeader.DataDirectory[1].VirtualAddress;
	//unsigned int location = ntheader->OptionalHeader.DataDirectory[1].Size;
	//printf("%X", location);
	
	//importheader = (PIMAGE_IMPORT_DESCRIPTOR)(unsigned char*)hdl + location;
	
	//cout<<importheader->Name;

	//thunkdata = (PIMAGE_THUNK_DATA)(unsigned char *)hdl + (importheader->OriginalFirstThunk);

	//cout<<thunkdata->u1.Function<<endl;
	//exit(1);

	//printf("%d",ntheader->OptionalHeader.SizeOfCode);

	//cout<<ntheader->OptionalHeader.DataDirectory[1].Size;
	
	
	CloseHandle(hdl);

	

	return 0;
}

