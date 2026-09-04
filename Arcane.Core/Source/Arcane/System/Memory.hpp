#pragma once

#include <Arcane/Core/Core.hpp>

// TODO: Remove this include and replace it with a custom forwarding function.
#include <utility>

namespace Arcane {

	/**
	 * Dynamically allocates a block of memory of the specified size and alignment.
	 * @param size The size of the memory block to allocate, in bytes.
	 * @param alignment The alignment of the memory block, in bytes.
	 * @returns A pointer to the allocated memory block, or nullptr if the allocation fails.
	 */
	void* allocate(u64 size, u64 alignment);

	/**
	 * Deallocates a previously allocated block of memory.
	 * @param pointer A pointer to the memory block to deallocate. If the pointer is nullptr, the function returns immediately without doing anything.
	 */
	void deallocate(void* pointer);

	/**
	 * Allocates memory for an array of objects of type T.
	 * @tparam T The type of the objects to allocate memory for.
	 * @param count The number of objects to allocate memory for.
	 * @returns A pointer to the allocated memory block, or nullptr if the allocation fails.
	 */
	template<typename T>
	T* allocate(u64 count = 1);

	/**
	 * Copies a block of memory from the source array to the destination array.
	 * @tparam T The type of the objects to copy.
	 * @param source A pointer to the source array.
	 * @param destination A pointer to the destination array.
	 * @param count The number of objects to copy.
	 * @returns A pointer to the destination array.
	 */
	template<typename T>
	T* copy(const T* source, T* destination, u64 count);

	void fill(void* data, u64 count, u8 value);

	/**
	 * A smart pointer that manages the lifetime of a dynamically allocated object and ensures unique ownership semantics.
	 * @tparam T The type of the managed object.
	 */
	template<typename T>
	class Unique {
	public:

		/**
		 * Creates a new Unique instance of type T, constructed with the provided arguments.
		 */
		template<typename... Args>
		static Unique<T> create(Args&&... args);

		/**
		 * Creates a new Unique instance that takes ownership of an existing dynamically allocated object of type T.
		 * @param pointer A pointer to the existing dynamically allocated object of type T.
		 * @returns A Unique instance that manages the provided object.
		 */
		static Unique<T> from(T* pointer);

		Unique() = default;

		// Copy constructor and copy assignment operator are deleted to enforce unique ownership.
		Unique(const Unique<T>& copy) = delete;
		Unique<T>& operator=(const Unique<T>& copy) = delete;

		/**
		 * Moves the ownership of the managed object from another Unique instance to this one.
		 * @param move The Unique instance to move from.
		 */
		Unique(Unique<T>&& move);

		/**
		 * Moves the ownership of the managed object from another Unique instance to this one.
		 * If this Unique instance already owns an object, that object will be destroyed before taking ownership of the new object.
		 * @param move The Unique instance to move from.
		 * @returns A reference to this Unique instance.
		 */
		Unique<T>& operator=(Unique<T>&& move);

		/**
		 * Destroys the managed object if it exists.
		 */
		~Unique();

		/**
		 * Destroys the managed object if it exists, leaving this Unique instance empty.
		 */
		void destroy();

		/**
		 * Moves the managed object out of this Unique instance, leaving it empty.
		 * @returns A pointer to the managed object that was moved out, or nullptr if this Unique instance was empty.
		 */
		T* take();

		/**
		 * Moves the ownership of the managed object from this Unique instance to a new Unique instance, leaving this Unique instance
		 * empty.
		 * @returns A new Unique instance that has ownership of the managed object.
		 */
		Unique<T> move();

		inline bool is_valid() const { return _pointer != nullptr; }

		inline T* pointer() { return _pointer; }
		inline const T* pointer() const { return _pointer; }
		inline T& reference() { return *_pointer; }
		inline const T& reference() const { return *_pointer; }

	private:
		Unique(T* pointer);

		T* _pointer = nullptr;
	};

	/**
	 * A smart pointer that manages the lifetime of a dynamically allocated object and provides shared ownership semantics.
	 * @tparam T The type of the managed object.
	 */
	template<typename T>
	class Shared {
	public:
		friend class Unique<T>;
		
		template<typename U>
		friend class Shared;

		/**
		 * Creates a new shared instance of type T, constructed with the provided arguments.
		 */
		template<typename... Args>
		static Shared<T> create(Args&&... args);

		static Shared<T> from(Unique<T>& unique);

		Shared() = default;

		// Copy constructor and copy assignment operator are deleted to prevent unintended sharing.
		Shared(const Shared<T>& other) = delete;
		Shared<T>& operator=(const Shared<T>& other) = delete;

		/**
		 * Moves the ownership of the managed object from another shared instance to this one.
		 * @param other The Shared instance to move from.
		 */
		Shared(Shared<T>&& other);

		/**
		 * Moves the ownership of the managed object from another shared instance to this one.
		 * If this shared instance already owns an object, that object will be released before taking ownership of the new object.
		 * @param other The shared instance to move from.
		 * @returns A reference to this shared instance.
		 */
		Shared<T>& operator=(Shared<T>&& other);

		/**
		 * Decrements the reference count of the managed object and deletes it if this was the last shared instance that owned it.
		 */
		~Shared();

		/**
		 * Decrements the reference count of the managed object and deletes it if this was the last shared instance that owned it,
		 * leaving this shared instance empty.
		 */
		void drop();

		template<typename U>
		Shared<U> reinterpret_as();

		/**
		 * Creates a new shared instance that shares ownership of the managed object.
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
			T* pointer = nullptr;
			u64 count = 0;
		};

	private:
		Shared(Memory* memory);

		Memory* _memory = nullptr;
	};

	template<typename T>
	T* allocate(u64 count) {
		return reinterpret_cast<T*>(Arcane::allocate(
			count * sizeof(T),
			alignof(T)));
	}

	template<typename T>
	T* copy(const T* source, T* destination, u64 count) {
#if defined(AR_PLATFORM_COMPILER_GCC)
		return reinterpret_cast<T*>(__builtin_memcpy(
			destination,
			source,
			count * sizeof(T)));
#else
#	error "Unimplemented."
#endif
	}

	void fill(void* data, u64 count, u8 value) {
#if defined(AR_PLATFORM_COMPILER_GCC)
		__builtin_memset(data, value, count);
#else
#	error "Unimplemented."
#endif
	}

	template<typename T>
	template<typename... Args>
	Unique<T> Unique<T>::create(Args&&... args) {
		T* pointer = Arcane::allocate<T>();
		new (pointer) T(std::forward<Args>(args)...);

		return Unique<T>(pointer);
	}

	template<typename T>
	Unique<T> Unique<T>::from(T* pointer) {
		return Unique<T>(pointer);
	}

	template<typename T>
	Unique<T>::Unique(T* pointer) : _pointer(pointer) { }

	template<typename T>
	Unique<T>::Unique(Unique<T>&& other) : _pointer(other.take()) { }

	template<typename T>
	Unique<T>& Unique<T>::operator=(Unique<T>&& other) {
		if (this == &other) return *this;
		
		destroy();
		_pointer = other.take();

		return *this;
	}

	template<typename T>
	Unique<T>::~Unique() {
		destroy();
	}

	template<typename T>
	void Unique<T>::destroy() {
		if (_pointer == nullptr) return;
		
		_pointer->~T();
		deallocate(_pointer);
		
		_pointer = nullptr;
	}

	template<typename T>
	T* Unique<T>::take() {
		T* pointer = _pointer;
		_pointer = nullptr;
		return pointer;
	}

	template<typename T>
	Unique<T> Unique<T>::move() {
		return Unique<T>(take());
	}

	template<typename T>
	template<typename... Args>
	Shared<T> Shared<T>::create(Args&&... args) {
		Shared<T>::Memory* memory = Arcane::allocate<Shared<T>::Memory>();

		memory->count = 1;
		memory->pointer = Arcane::allocate<T>();
		new (memory->pointer) T(std::forward<Args>(args)...);

		return Shared<T>(memory);
	}

	template<typename T>
	Shared<T> Shared<T>::from(Unique<T>& unique) {
		if (!unique.is_valid()) return Shared<T>();

		Shared<T>::Memory* memory = Arcane::allocate<Shared<T>::Memory>();
		memory->count = 1;
		memory->pointer = unique.take();

		return Shared<T>(memory);
	}

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
			_memory->pointer->~T();
			deallocate(_memory->pointer);
			deallocate(_memory);
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