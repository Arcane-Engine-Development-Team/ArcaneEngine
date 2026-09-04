#pragma once

#include <Arcane/Core/Core.hpp>
#include <Arcane/System/Memory.hpp>

namespace Arcane {

	/**
	 * A non-owning view into a contiguous immutable block of memory of type T.
	 * Provides utilities to create subviews and access the underlying data.
	 * @tparam T The type of elements in the view.
	 */
	template<typename T>
	class View {
	public:
		/**
		 * Creates a view from a raw pointer and a length.
		 * @param pointer The pointer to the beginning of the memory block.
		 * @param length The number of elements in the memory block.
		 * @returns A View representing the memory block.
		 */
		static View<T> from(const T* pointer, u64 length);
		
		/**
		 * Creates a view from a pair of pointers defining the beginning and end of the memory block.
		 * @param begin The pointer to the beginning of the memory block.
		 * @param end The pointer to the end of the memory block.
		 * @returns A View representing the memory block.
		 */
		static View<T> from(const T* begin, const T* end);
		
		View() = default;

		/**
		 * Creates a subview of the current view.
		 * @param offset The starting index of the subview.
		 * @param length The number of elements in the subview.
		 * @returns A View representing the subview.
		 */
		View<T> view(u64 offset, u64 length);

		/**
		 * Returns a constant reference to the element at the specified index.
		 * @param index The index of the element to access.
		 * @returns A constant reference to the element at the specified index.
		 */
		inline const T& at(u64 index) const { return _pointer[index]; }

		inline bool is_valid() const { return _pointer != nullptr; }

		inline const T* pointer() const { return _pointer; }
		inline u64 length() const { return _length; }

		inline const T* begin() const { return _pointer; }
		inline const T* end() const { return _pointer + _length; }

	private:
		View(const T* pointer, u64 length);

		const T* _pointer = nullptr;
		u64 _length = 0;
	};

	/**
	 * A non-owning view into a contiguous mutable block of memory of type T.
	 * Provides utilities to create subslices, views, and access the underlying data.
	 * @tparam T The type of elements in the slice.
	 */
	template<typename T>
	class Slice {
	public:
		/**
		 * Creates a slice from a raw pointer and a length.
		 * @param pointer The pointer to the beginning of the memory block.
		 * @param length The number of elements in the memory block.
		 * @returns A Slice representing the memory block.
		 */
		static Slice<T> from(T* pointer, u64 length);

		/**
		 * Creates a slice from a pair of pointers defining the beginning and end of the memory block.
		 * @param begin The pointer to the beginning of the memory block.
		 * @param end The pointer to the end of the memory block.
		 * @returns A Slice representing the memory block.
		 */
		static Slice<T> from(T* begin, T* end);

		Slice() = default;

		/**
		 * Creates a view of the entire slice.
		 * @returns A View representing the entire slice.
		 */
		View<T> view() const;

		/**
		 * Creates a view of a subrange of the slice.
		 * @param offset The starting index of the subrange.
		 * @param length The number of elements in the subrange.
		 * @returns A View representing the subrange.
		 */
		View<T> view(u64 offset, u64 length) const;

		/**
		 * Creates a subrange of the current slice.
		 * @param offset The starting index of the subrange.
		 * @param length The number of elements in the subrange.
		 * @returns A Slice representing the subrange.
		 */
		Slice<T> slice(u64 offset, u64 length);

		/**
		 * Returns a reference to the element at the specified index.
		 * @param index The index of the element to access.
		 * @returns A reference to the element at the specified index.
		 */
		inline T& at(u64 index) const { return _pointer[index]; }

		inline bool is_valid() const { return _pointer != nullptr; }

		inline T* pointer() const { return _pointer; }
		inline u64 length() const { return _length; }

		inline T* begin() const { return _pointer; }
		inline T* end() const { return _pointer + _length; }

	private:
		Slice(T* pointer, u64 length);

		T* _pointer = nullptr;
		u64 _length = 0;
	};


	/**
	 * A dynamically resizable buffer that manages a contiguous block of memory of type T.
	 * Provides utilities to allocate, deallocate, resize, and create views of the memory block.
	 * @tparam T The type of elements in the buffer.
	 */
	template<typename T = u8>
	class Buffer {
	public:

		/**
		 * Dynamically allocates a new buffer with the specified number of elements.
		 * @param count The number of elements to allocate.
		 * @returns A Buffer representing the allocated memory block.
		 */
		static Buffer<T> allocate(u64 count);
		
		/**
		 * Creates a buffer from an existing dynamically allocated memory block and takes ownership of it.
		 * @param data The pointer to the beginning of the memory block.
		 * @param size The number of elements in the memory block.
		 * @returns A Buffer representing the memory block.
		 */
		static Buffer<T> from(T* data, u64 size);

		Buffer() = default;
		Buffer(const Buffer<T>& copy) = delete;
		Buffer<T>& operator=(const Buffer<T>& copy) = delete;

		/**
		 * Transfers ownership of the memory block from another buffer.
		 * @param move The buffer to move from.
		 */
		Buffer(Buffer<T>&& move);

		/**
		 * Transfers ownership of the memory block from another buffer.
		 * @param move The buffer to move from.
		 * @returns A reference to the current buffer.
		 */
		Buffer<T>& operator=(Buffer<T>&& move);

		/**
		 * Deallocates the memory block if it is still owned by the buffer.
		 */
		~Buffer();

		/**
		 * Deallocates the memory block if it is still owned by the buffer. After calling this function, the buffer becomes invalid.
		 */
		void deallocate();

		/**
		 * Takes ownership of the memory block from the buffer. After calling this function, the buffer becomes invalid.
		 * @return A pointer to the memory block that was taken from the buffer.
		 */
		T* take();

		/**
		 * Resizes the buffer to the specified capacity. If the new capacity is smaller than the current capacity, the buffer may be 
		 * truncated. If the new capacity is larger, additional memory will be allocated.
		 * @param capacity The new capacity for the buffer.
		 */
		void resize(u64 capacity);

		/**
		 * Creates a view of the buffer's memory block.
		 * @returns A View representing the buffer's memory block.
		 */
		View<T> view();
		
		/**
		 * Creates a view of a specific range within the buffer's memory block.
		 * @param offset The starting offset of the range.
		 * @param length The length of the range.
		 * @returns A View representing the specified range of the buffer's memory block.
		 */
		View<T> view(u64 offset, u64 length);

		/**
		 * Creates a slice of the buffer's memory block.
		 * @returns A Slice representing the buffer's memory block.
		 */
		Slice<T> slice();

		/**
		 * Creates a slice of a specific range within the buffer's memory block.
		 * @param offset The starting offset of the range.
		 * @param length The length of the range.
		 * @returns A Slice representing the specified range of the buffer's memory block.
		 */
		Slice<T> slice(u64 offset, u64 length);

		/**
		 * Creates a copy of the buffer's memory block. The new buffer will have its own separate memory allocation.
		 */
		Buffer<T> copy();

		/**
		 * Creates a copy of a specific range within the buffer's memory block. The new buffer will have its own separate memory 
		 * allocation.
		 * @param offset The starting offset of the range.
		 * @param length The length of the range.
		 * @returns A Buffer representing the specified range of the buffer's memory block.
		 */
		Buffer<T> copy(u64 offset, u64 length);

		/**
		 * Moves the buffer's memory block to a new buffer. After calling this function, the current buffer becomes invalid.
		 * @returns A Buffer representing the moved memory block.
		 */
		Buffer<T> move();

		/**
		 * Accesses the element at the specified index.
		 * @param index The index of the element to access.
		 * @returns A reference to the element at the specified index.
		 */
		inline T& at(u64 index) { return _data[index]; }

		/**
		 * Accesses the element at the specified index as an immutable reference.
		 * @param index The index of the element to access.
		 * @returns A const reference to the element at the specified index.
		 */
		inline const T& at(u64 index) const { return _data[index]; }

		inline bool is_valid() const { return _data != nullptr; }

		inline T* data() { return _data; }
		inline const T* data() const { return _data; }
		inline u64 capacity() const { return _capacity; }

	private:
		Buffer(T* data, u64 capacity);

		T* _data = nullptr;
		u64 _capacity = 0;
	};

	template<typename T>
	View<T> View<T>::from(const T* pointer, u64 size) {
		if (pointer == nullptr) return View<T>();
		if (size == 0) return View<T>();

		View<T> view;
		view._pointer = pointer;
		view._length = size;
		return view;
	}

	template<typename T>
	View<T> View<T>::from(const T* begin, const T* end) {
		if (begin == nullptr) return View<T>();
		if (end == nullptr) return View<T>();
		if (begin >= end) return View<T>();

		View<T> view;
		view._pointer = begin;
		view._length = static_cast<u64>(end - begin);
		return view;
	}

	template<typename T>
	View<T>::View(const T* pointer, u64 length) : _pointer(pointer), _length(length) {}

	template<typename T>
	View<T> View<T>::view(u64 offset, u64 length) {
		if (offset + length > _length) return View<T>();

		return View<T>::from(_pointer + offset, length);
	}

	template<typename T>
	Slice<T> Slice<T>::from(T* pointer, u64 size) {
		if (pointer == nullptr) return Slice<T>();
		if (size == 0) return Slice<T>();

		return Slice<T>(pointer, size);
	}

	template<typename T>
	Slice<T>::Slice(T* pointer, u64 length) : _pointer(pointer), _length(length) { }

	template<typename T>
	Slice<T> Slice<T>::from(T* begin, T* end) {
		if (begin == nullptr) return Slice<T>();
		if (end == nullptr) return Slice<T>();
		if (begin >= end) return Slice<T>();

		return Slice<T>(begin, static_cast<u64>(end - begin));
	}

	template<typename T>
	View<T> Slice<T>::view() const {
		return View<T>::from(_pointer, _length);
	} 

	template<typename T>
	View<T> Slice<T>::view(u64 offset, u64 length) const {
		if (offset + length > _length) return View<T>();

		return View<T>::from(_pointer + offset, length);
	}

	template<typename T>
	Slice<T> Slice<T>::slice(u64 offset, u64 length) {
		if (offset + length > _length) return Slice<T>();

		return Slice<T>::from(_pointer + offset, length);
	}

	template<typename T>
	Buffer<T> Buffer<T>::allocate(u64 count) {
		T* data = Arcane::allocate<T>(count);

		if (data == nullptr) return Buffer<T>();
		
		return Buffer<T>(data, count);
	}

	template<typename T>
	Buffer<T> Buffer<T>::from(T* data, u64 size) {
		if (data == nullptr) return Buffer<T>();
		if (size == 0) return Buffer<T>();

		return Buffer<T>(data, size);
	}

	template<typename T>
	Buffer<T>::Buffer(T* data, u64 capacity) : _data(data), _capacity(capacity) { }

	template<typename T>
	Buffer<T>::Buffer(Buffer<T>&& move) : _data(move._data), _capacity(move._capacity) {
		if (this == &move) return;

		move._data = nullptr;
		move._capacity = 0;
	}

	template<typename T>
	Buffer<T>& Buffer<T>::operator=(Buffer<T>&& move) {
		if (this == &move) return *this;

		deallocate();

		_data = move._data;
		_capacity = move._capacity;

		move._data = nullptr;
		move._capacity = 0;

		return *this;
	}

	template<typename T>
	Buffer<T>::~Buffer() {
		deallocate();
	}

	template<typename T>
	void Buffer<T>::deallocate() {
		if (!is_valid()) return;

		Arcane::deallocate(_data);
		_data = nullptr;
		_capacity = 0;
	}

	template<typename T>
	void Buffer<T>::resize(u64 capacity) {
		if (capacity <= _capacity) return;

		T* new_data = Arcane::allocate<T>(capacity);
		if (new_data == nullptr) return;

		Arcane::copy<T>(new_data, _data, _capacity);
		Arcane::deallocate(_data);

		_data = new_data;
		_capacity = capacity;
	}

	template<typename T>
	View<T> Buffer<T>::view() {
		if (!is_valid()) return View<T>();
		return View<T>::from(_data, _capacity);
	}

	template<typename T>
	View<T> Buffer<T>::view(u64 offset, u64 length) {
		if (!is_valid()) return View<T>();
		if (offset + length > _capacity) return View<T>();

		return View<T>::from(_data + offset, length);
	}

	template<typename T>
	Slice<T> Buffer<T>::slice() {
		if (!is_valid()) return Slice<T>();
		return Slice<T>::from(_data, _capacity);
	}

	template<typename T>
	Slice<T> Buffer<T>::slice(u64 offset, u64 length) {
		if (!is_valid()) return Slice<T>();
		if (offset + length > _capacity) return Slice<T>();

		return Slice<T>::from(_data + offset, length);
	}

	template<typename T>
	Buffer<T> Buffer<T>::copy() {
		if (!is_valid()) return Buffer<T>();

		Buffer<T> new_buffer = Buffer<T>::allocate(_capacity);
		Arcane::copy<T>(new_buffer.data(), _data, _capacity);
		return new_buffer;
	}

	template<typename T>
	Buffer<T> Buffer<T>::copy(u64 offset, u64 length) {
		if (!is_valid()) return Buffer<T>();
		if (offset + length > _capacity) return Buffer<T>();

		Buffer<T> new_buffer = Buffer<T>::allocate(length);
		Arcane::copy<T>(new_buffer.data(), _data + offset, length);
		return new_buffer;
	}

	template<typename T>
	Buffer<T> Buffer<T>::move() {
		return Buffer<T>(take(), _capacity);
	}

	template<typename T>
	T* Buffer<T>::take() {
		T* data = _data;
		
		_data = nullptr;
		_capacity = 0;

		return data;
	}

}