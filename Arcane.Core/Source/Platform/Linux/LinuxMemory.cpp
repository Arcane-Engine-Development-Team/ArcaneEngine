#include <Arcane/Core/Core.hpp>

#if defined(AR_PLATFORM_OS_LINUX)

#include <stdlib.h>

namespace Arcane {

    void* allocate(u64 size, u64 alignment) {
        if (size == 0) return nullptr;
        if (alignment == 0) return 0;

        void* pointer = aligned_alloc(alignment, size);
        return pointer;
    }

    void deallocate(void* pointer) {
        if (pointer == nullptr) return;
        free(pointer);
    }

}

#endif // AR_PLATFORM_OS_LINUX