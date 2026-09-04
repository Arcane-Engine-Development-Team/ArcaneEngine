#include <Arcane/Core/Core.hpp>
#include <Arcane/System/Memory.hpp>
#include <Arcane/Data/Buffer.hpp>
#include <Arcane/Data/String.hpp>

#include <iostream>

template<typename T>
void print(const T& element) {
	std::wcout << element;
}

int main() {
	Arcane::String<Arcane::c16> str = Arcane::String<Arcane::c16>::from(L"Hello, Arcane!\n");

	for (Arcane::u64 i = 0; i < str.length(); i++) {
		std::wcout << str.at(i);
	}
	std::wcout << std::endl;

	return 0;
}