#pragma once

#include <Arcane/Core/Core.hpp>

// TEMPORARY INCLUDE
// TODO: Remove this include and replace it with a custom forwarding function.
#include <utility>
#include <memory>

namespace Arcane {

    /**
     * Allocates a block of memory of the specified size and alignment.
     * @param size The size of the memory block to allocate, in bytes.
     * @param alignment The alignment of the memory block, in bytes.
     * @return A pointer to the allocated memory block, or nullptr if the allocation fails.
     */
    void* allocate(u64 size, u64 alignment);

    /**
     * Frees a previously allocated block of memory.
     * @param pointer A pointer to the memory block to free. If the pointer is nullptr, the function returns immediately without doing anything.
     */
    void free(void* pointer);

    /**
     * A smart pointer that manages the lifetime of a dynamically allocated object and provides shared ownership semantics.
     * The Shared class maintains a reference count to the managed object, and automatically deletes the object when the last Shared instance that owns it is destroyed or dropped.
     * @tparam T The type of the managed object.
     */
    template<typename T>
    class Shared {
    public:
        template<typename U>
        friend class Shared;

        /**
         * Creates a new Shared instance that manages a dynamically allocated object of type T, constructed with the provided arguments.
         */
        template<typename... Args>
        static Shared<T> create(Args&&... args);

        /**
         * Default constructor. Creates an empty Shared instance that does not manage any object.
         */
        Shared();

        // Copy constructor and copy assignment operator are deleted to prevent unintended sharing.
        Shared(const Shared<T>& other) = delete;
        Shared<T>& operator=(const Shared<T>& other) = delete;

        /**
         * The move constructor transfers ownership of the managed object from another Shared instance to this one, leaving the other instance empty. The reference count is not incremented.
         */
        Shared(Shared<T>&& other);

        /**
         * The move assignment operator transfers ownership of the managed object from another Shared instance to this one, leaving the other instance empty. The reference count is not incremented.
         */
        Shared<T>& operator=(Shared<T>&& other);

        /**
         * Destructor. Decrements the reference count of the managed object and deletes it if this was the last Shared instance that owned it.
         */
        ~Shared();

        /**
         * Drops ownership of the managed object, decrementing the reference count and deleting the object if this was the last Shared instance that owned it. After calling drop(), this Shared instance becomes empty and does not manage any object.
         */
        void drop();

        template<typename U>
        Shared<U> reinterpret_as();

        /**
         * Creates a new Shared instance that shares ownership of the managed object. Increments the reference count.
         */
        Shared<T> share();

        inline bool is_valid() const { return _memory != nullptr; }
        inline u64 reference_count() const { return _memory->count; }

        inline T* pointer() { return _memory->pointer; }
        inline const T* pointer() const { return _memory->pointer; }
        inline T& reference() { return *(_memory->pointer); }
        inline const T& reference() const { return *(_memory->pointer); }

    protected:
        struct Memory {
            T* pointer;
            u64 count;
        };

    private:

        Shared(Memory* memory);

        Memory* _memory;
    };

    template<typename T>
    template<typename... Args>
    Shared<T> Shared<T>::create(Args&&... args) {
        Shared<T>::Memory* memory = reinterpret_cast<Shared<T>::Memory*>(
            Arcane::allocate(
                sizeof(Shared<T>::Memory),
                alignof(Shared<T>::Memory)));

        memory->count = 1;
        memory->pointer = reinterpret_cast<T*>(Arcane::allocate(sizeof(T), alignof(T)));
        new (memory->pointer) T(std::forward<Args>(args)...);

        return Shared<T>(memory);
    }

    template<typename T>
    Shared<T>::Shared() : _memory(nullptr) { }

    template<typename T>
    Shared<T>::Shared(Memory* memory) : _memory(memory) { }

    template<typename T>
    Shared<T>::Shared(Shared<T>&& other) : _memory(other._memory) {
        other._memory = nullptr;
    }

    template<typename T>
    Shared<T>& Shared<T>::operator=(Shared<T>&& other) {
        if (this != &other) drop();
        
        _memory = other._memory;
        other._memory = nullptr;

        return *this;
    }

    template<typename T>
    Shared<T>::~Shared() {
        drop();
    }

    template<typename T>
    void Shared<T>::drop() {
        if (_memory == nullptr) return;

        _memory->count -= 1;
        if (_memory->count == 0) {
            std::destroy_at(_memory->pointer);
            Arcane::free(_memory->pointer);
            Arcane::free(_memory);
        }

        _memory = nullptr;
    }

    template<typename T>
    Shared<T> Shared<T>::share() {
        if (_memory == nullptr) return Shared<T>();

        _memory->count += 1;
        return Shared<T>(_memory);
    }

    template<typename T>
    template<typename U>
    Shared<U> Shared<T>::reinterpret_as() {
        if (_memory == nullptr) return Shared<U>();

        _memory->count += 1;
        return Shared<U>(reinterpret_cast<typename Shared<U>::Memory*>(_memory));
    }

}