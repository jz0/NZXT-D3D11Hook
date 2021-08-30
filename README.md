# NZXT-D3D11Hook
IDXGISwapChain::Present hook using the NZXT Cam software

NZXT Cam utilizes [minhook](https://github.com/TsudaKageyu/minhook) to hook IDXGISwapChain::Present, this project hooks minhook and then hooks IDXGISwapChain::Present via the hooked minhook.
