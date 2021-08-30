#include "Hooks.hpp"

bool Hooks::MH_CreateHook(uintptr_t Target, uintptr_t Detour, uintptr_t Original)
{
	auto CreateHookPtr = Util::FindPattern("Graphics Hook64.dll", "48 89 5C 24 20 55 56 57 41 56 41 57 48 83 EC 60 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 58 33 DB");

	if (!CreateHookPtr)
		return false;

	using CreateHook = uintptr_t(__fastcall*)(void*, void*, void**);
	auto CreateHookFn = (CreateHook)CreateHookPtr;

	return CreateHookFn((void*)Target, (void*)Detour, (void**)Original) == 0 ? true : false;
}

bool Hooks::MH_EnableHook(uintptr_t Target, bool Enable)
{
	auto EnableHookPtr = Util::FindPattern("Graphics Hook64.dll", "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 33 DB 48 8B F1 8B FB E8 ? ? ? ? 48 39 1D ? ? ? ? 74 73 48 85 F6");

	if (!EnableHookPtr)
		return false;

	using EnableHook = uintptr_t(__fastcall*)(void*, bool);
	auto EnableHookFn = (EnableHook)EnableHookPtr;

	return EnableHookFn((void*)Target, Enable) == 0 ? true : false;
}

bool Hooks::MH_EnableHookQue()
{
	auto EnableHookQuePtr = Util::FindPattern("Graphics Hook64.dll", "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 30 33 FF 8B F7 E8 ? ? ? ?");

	if (!EnableHookQuePtr)
		return false;

	using EnableHookQue = uintptr_t(__stdcall*)(void);
	auto EnableHookQueFn = (EnableHookQue)EnableHookQuePtr;

	return EnableHookQueFn() == 0 ? true : false;
}

ID3D11Device* pDevice;
ID3D11DeviceContext* pContext;
ID3D11RenderTargetView* pRenderTarget;
int Width, Height;

PresentScene OriginalPresent;
HRESULT Hooks::Present(IDXGISwapChain* Swapchain, __int64  SyncInterval, __int64 Flags)
{
	if (!pDevice)
	{
		ID3D11Texture2D* TargetTexture = 0;
		ID3D11Texture2D* pBackBuffer = 0;
		D3D11_TEXTURE2D_DESC BackBufferDesc = { 0 };

		Swapchain->GetDevice(__uuidof(pDevice), (PVOID*)&pDevice);
		pDevice->GetImmediateContext(&pContext);

		Swapchain->GetBuffer(0, __uuidof(TargetTexture), (PVOID*)&TargetTexture);
		pDevice->CreateRenderTargetView(TargetTexture, nullptr, &pRenderTarget);
		pRenderTarget->Release();

		Swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (PVOID*)&pBackBuffer);
		pBackBuffer->GetDesc(&BackBufferDesc);

		Width = BackBufferDesc.Width;
		Height = BackBufferDesc.Height;

		printf("[+] Width: %d Height: %d \n", Width, Height);
	}

	printf("Hello \n");
	//printf("[+] Width: %d Height: %d \n", Width, Height);
	//printf("[+] Swapchain %p SyncInterval %d Flags %d \n", Swapchain, SyncInterval, Flags);
	return OriginalPresent(Swapchain, SyncInterval, Flags);
}