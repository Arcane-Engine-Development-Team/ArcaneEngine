#include <Arcane/Core/Core.hpp>

#if defined(AR_PLATFORM_OS_WINDOWS)

#include <Arcane/System/Memory.hpp>

#include <windows.h>

namespace Arcane {

	void* allocate(u64 size, u64 alignment) {
		if (size == 0) return nullptr;
		if (alignment == 0) return nullptr;

		void* pointer = HeapAlloc(
			GetProcessHeap(),
			0,
			size);

		return pointer;
	}

	void deallocate(void* pointer) {
		if (pointer == nullptr) return;

		HeapFree(
			GetProcessHeap(),
			0,
			pointer);
	}

}

#endif // AR_PLATFORM_OS_WINDOWS