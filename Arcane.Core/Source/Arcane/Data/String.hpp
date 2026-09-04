#pragma once

#include <Arcane/Core/Core.hpp>
#include <Arcane/Data/Buffer.hpp>

namespace Arcane {

	/**
	 * A dynamically resizable, non-null terminated string class that manages a string of characters of type T.
	 * Provides utilities to allocate, deallocate, copy, and manipulate strings.
	 * @tparam T The type of characters in the string.
	 */
	template<typename T = c8>
	class String {
	public:
		/**
		 * Allocates a new string with the specified capacity.
		 * @param capacity The capacity of the string to allocate.
		 * @returns A String representing the allocated memory.
		 */
		static String<T> allocate(u64 capacity);

		/**
		 * Creates a string from a null-terminated C-style string.
		 * @param cstring The C-style string to create the String from.
		 * @returns A String representing the C-style string.
		 */
		static String<T> from(const T* cstring);

		String() = default;
		String(const String<T>& other) = delete;
		String<T>& operator=(const String<T>& other) = delete;

		/**
		 * Transfers ownership of the buffer from another string.
		 * @param other The string to move from.
		 */
		String(String<T>&& other);

		/**
		 * Transfers ownership of the buffer from another string.
		 * @param other The string to move from.
		 * @returns A reference to the current string.
		 */
		String<T>& operator=(String<T>&& other);

		/**
		 * Deallocates the memory block if it is still owned by the string. After calling this function, the string becomes invalid.
		 */
		~String();

		/**
		 * Deallocates the memory block if it is still owned by the string, leaving this string invalid.
		 */
		void destroy();

		/**
		 * Creates a copy of the current string.
		 * @returns A new String representing the copy of the current string.
		 */
		String<T> copy() const;

		/**
		 * Transfers ownership of the buffer from the current string to a new string.
		 * @returns A new String representing the moved content of the current string.
		 */
		String<T> move();

		/**
		 * Accesses the character at the specified index.
		 * @param index The index of the character to access.
		 * @returns A reference to the character at the specified index.
		 */
		inline T& at(u64 index) { return _buffer.at(index); }

		/**
		 * Accesses the character at the specified index as a constant reference.
		 * @param index The index of the character to access.
		 * @returns A constant reference to the character at the specified index.
		 */
		inline const T& at(u64 index) const { return _buffer.at(index); }

		inline bool is_valid() const { return _buffer.is_valid(); }

		inline Buffer<T>& buffer() { return _buffer; }
		inline const Buffer<T>& buffer() const { return _buffer; }
		inline u64 length() const { return _length; }

		inline T* begin() { return _buffer.begin(); }
		inline const T* begin() const { return _buffer.begin(); }
		inline T* end() { return _buffer.begin() + _length; }
		inline const T* end() const { return _buffer.begin() + _length; }
	private:
		String(Buffer<T>&& buffer, u64 length);

		Buffer<T> _buffer;
		u64 _length = 0;
	};

	template<typename T>
	String<T> String<T>::allocate(u64 capacity) {
		Buffer<T> buffer = Buffer<T>::allocate(capacity);
		fill(buffer.data(), buffer.capacity(), 0);
		return String<T>(buffer, 0);
	}

	template<typename T>
	String<T> String<T>::from(const T* cstring) {
		u64 length = 0;
		while (cstring[length] != 0) length++;

		Buffer<T> buffer = Buffer<T>::allocate(length + 1);
		Arcane::copy(cstring, buffer.data(), length);

		return String<T>(buffer.move(), length);
	}

	template<typename T>
	String<T>::String(Buffer<T>&& buffer, u64 length)
	: _buffer(buffer.move()), _length(length) { }

	template<typename T>
	String<T>::String(String<T>&& move) 
	: _buffer(move._buffer.move()), _length(move._length) {
		move._length = 0;
	}

	template<typename T>
	String<T>& String<T>::operator=(String<T>&& move) {
		if (this == &move) return *this;
		
		destroy();
		_buffer = move._buffer.move();
		_length = move._length;
		move._length = 0;

		return *this;
	}

	template<typename T>
	String<T>::~String() {
		destroy();
	}

	template<typename T>
	void String<T>::destroy() {
		if (!_buffer.is_valid()) return;

		_buffer.deallocate();
		_length = 0;
	}

	template<typename T>
	String<T> String<T>::copy() const {
		Buffer<T> buffer = Buffer<T>::allocate(_length);
		Arcane::copy(_buffer.data(), buffer.data(), _length);

		return String<T>(buffer.move(), _length);
	}

	template<typename T>
	String<T> String<T>::move() {
		u64 length = _length;
		_length = 0;
		return String<T>(_buffer.move(), length);
	}

}