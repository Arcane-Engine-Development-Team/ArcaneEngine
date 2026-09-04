#include <Arcane/Core/Core.hpp>
#include <Arcane/System/Memory.hpp>
#include <Arcane/Data/Buffer.hpp>
#include <Arcane/Data/Algorithm.hpp>

#include <iostream>

template<typename T>
void print(const T& element) {
	std::cout << element << std::endl;
}

template<typename T>
void multiply(T& element) {
	element *= 2;
}

int main() {
	Arcane::Buffer buffer = Arcane::Buffer<Arcane::i32>::allocate(32);
	std::cout << "Buffer size: " << buffer.capacity() << std::endl;

	for (Arcane::u64 i = 0; i < buffer.capacity(); ++i) {
		buffer.at(i) = static_cast<Arcane::i32>(i);
		std::cout << "Buffer element " << i << ": " << buffer.at(i) << std::endl;
	}

	std::cout << "View: o: 16, l: 16" << std::endl;
	Arcane::View view = buffer.view(16, 16);
	Arcane::foreach(view, print<Arcane::i32>);
	std::cout << std::endl;

	std::cout << "Subview: o: 0, l: 8" << std::endl;
	Arcane::View subview = view.view(0, 8);
	Arcane::foreach(subview, print<Arcane::i32>);
	std::cout << std::endl;

	std::cout << "Slice: o: 16, l: 16" << std::endl;
	Arcane::Slice slice = buffer.slice(16, 16);
	Arcane::foreach(slice, multiply<Arcane::i32>);
	std::cout << std::endl;
	
	std::cout << "Subslice: o: 0, l: 8" << std::endl;
	Arcane::Slice subslice = slice.slice(8, 8);
	Arcane::foreach(subslice, multiply<Arcane::i32>);

	Arcane::foreach(buffer.view(), print<Arcane::i32>);

	return 0;
}