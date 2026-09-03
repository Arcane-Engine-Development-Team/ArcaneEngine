#include <Arcane/Core/Core.hpp>
#include <Arcane/System/Memory.hpp>

#include <iostream>

int main() {
	Arcane::Shared<Arcane::i32> shared_integer = Arcane::Shared<Arcane::i32>::create(42);
	std::cout << "Shared integer value: " << shared_integer.reference() << std::endl;
	std::cout << "Reference count: " << shared_integer.reference_count() << std::endl;

	{
		Arcane::Shared<Arcane::u32> shared_unsigned_integer_copy = shared_integer.reinterpret_as<Arcane::u32>();
		std::cout << "Shared unsigned integer copy value: " << shared_unsigned_integer_copy.reference() << std::endl;
		std::cout << "Reference count after copy: " << shared_unsigned_integer_copy.reference_count() << std::endl;

		shared_unsigned_integer_copy.reference() = 100;
	}

	std::cout << "Shared integer value: " << shared_integer.reference() << std::endl;
	std::cout << "Reference count: " << shared_integer.reference_count() << std::endl;

	return shared_integer.reference();
}