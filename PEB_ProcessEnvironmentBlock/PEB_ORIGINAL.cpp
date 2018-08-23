#define CUT_LIST(item) \
    item.Blink->Flink = item.Flink; \
    item.Flink->Blink = item.Blink

void HideModule(HMODULE hModule)
{
    ULONG_PTR DllHandle = (ULONG_PTR)hModule;
    PPEB_LDR_DATA pebLdrData;
    PLDR_MODULE mod;

    __asm {
        mov eax, fs:[0x30]        //get PEB ADDR
        add eax, 0x0C        
        mov eax, [eax]            //get LoaderData ADDR
        mov pebLdrData, eax
    }

    for (
        mod = (PLDR_MODULE)pebLdrData->InLoadOrderModuleList.Flink;
        mod->BaseAddress != 0;
        mod = (PLDR_MODULE)mod->InLoadOrderModuleList.Flink
    ) {
        if ((HMODULE)mod->BaseAddress == hModule) {
            CUT_LIST(mod->InLoadOrderModuleList);
            CUT_LIST(mod->InInitializationOrderModuleList);
            CUT_LIST(mod->InMemoryOrderModuleList);
        
            ZeroMemory(mod, sizeof(LDR_MODULE));
            return;
        }
    }
}



//http://www.openrce.org/blog/view/844/How_to_hide_dll