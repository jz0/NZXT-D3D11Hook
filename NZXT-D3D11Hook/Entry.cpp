#include "Include.hpp"

void Init()
{
	AllocConsole();
	SetConsoleTitle("dbg");
	freopen("CONOUT$", "w", stdout);

	uintptr_t NZXTOverlayRenderer = (uintptr_t)GetModuleHandleA("Graphics Hook64.dll");
	printf("[+] NZXTOverlayRenderer %p \n", NZXTOverlayRenderer);

	uintptr_t D3D11Present = Util::FindPattern("Graphics Hook64.dll", "40 57 48 83 EC 30 48 C7 44 24 20 FE FF FF FF 48 89 5C 24 40 48 89 6C 24 48 48 89 74 24 58 41 8B F0 8B EA 48 8B D9 41 0F B6 F8 40 80 E7 01");
	printf("[+] D3D11Present %p \n", D3D11Present);

	//Create IDXGISwapChain::Present hook via the hooked MinHook 
	//Hookception ¯\_(-___-)_/¯
	if (Hooks::MH_CreateHook(D3D11Present, (uintptr_t)Hooks::Present, (uintptr_t)&OriginalPresent))
	{
		//Enable hooks
		Hooks::MH_EnableHook(D3D11Present, true);
		Hooks::MH_EnableHookQue();
	}
}

bool __stdcall DllMain(HMODULE Module, ULONG Reason, PVOID Reserved)
{
	if (Reason == DLL_PROCESS_ATTACH)
		Init();

	return TRUE;
}