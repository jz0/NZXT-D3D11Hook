#include "Include.hpp"

typedef HRESULT(*PresentScene)(IDXGISwapChain* Swapchain, __int64 SyncInterval, __int64 Flags);
extern PresentScene OriginalPresent;

namespace Hooks
{
	bool MH_CreateHook(uintptr_t Target, uintptr_t Detour, uintptr_t Original);
	bool MH_EnableHook(uintptr_t Target, bool Enable);
	bool MH_EnableHookQue();
	HRESULT Present(IDXGISwapChain* Swapchain, __int64 SyncInterval, __int64 Flags);
}