#include "Include.hpp"

namespace Util
{
	uintptr_t FindPattern(const char* Module, const char* Sig);
	uintptr_t Dereference(uintptr_t Address, unsigned int Offset);
}