#include "Util.hpp"

uintptr_t Util::FindPattern(const char* Module, const char* Sig)
{
#define IN_RANGE(x, a, b) (x >= a && x <= b)
#define GET_BITS(x) (IN_RANGE((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xA): (IN_RANGE(x, '0', '9') ? x - '0': 0))
#define GET_BYTE(x) (GET_BITS(x[0]) << 4 | GET_BITS(x[1]))

	const auto ModuleHandle = GetModuleHandleA(Module);
	const auto DOSHeader = PIMAGE_DOS_HEADER(ModuleHandle);
	const auto NTHeaders = PIMAGE_NT_HEADERS(reinterpret_cast<std::uint8_t*>(ModuleHandle) + DOSHeader->e_lfanew);

	uintptr_t StartAddress = (uintptr_t)GetModuleHandleA(Module);
	uintptr_t Size = (StartAddress + (uintptr_t)NTHeaders->OptionalHeader.SizeOfImage);

	const char* Pat = Sig;
	uintptr_t FirstMatch = 0;
	for (uintptr_t pCur = StartAddress; pCur < Size; pCur++)
	{
		if (!*Pat) return FirstMatch;
		if (*(PBYTE)Pat == ('\?') || *(BYTE*)pCur == GET_BYTE(Pat))
		{
			if (!FirstMatch) FirstMatch = pCur;
			if (!Pat[2]) return FirstMatch;
			if (*(PWORD)Pat == ('\?\?') || *(PBYTE)Pat != ('\?')) Pat += 3;
			else Pat += 2;
		}
		else
		{
			Pat = Sig;
			FirstMatch = 0;
		}
	}
	return NULL;
}

uintptr_t Util::Dereference(uintptr_t Address, unsigned int Offset)
{
	if (Address == 0)
		return 0;

	if (sizeof(uintptr_t) == 8)
		return Address + (int)((*(int*)(Address + Offset) + Offset) + sizeof(int));

	return (uintptr_t) * (unsigned long*)(Address + Offset);
}