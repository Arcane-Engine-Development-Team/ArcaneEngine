#pragma once

namespace Arcane {

	template<typename T, typename Func>
	void foreach(T& object, Func func);

	template<typename T, typename Func>
	void foreach(const T& object, Func func);

	template<typename T, typename Func>
	void foreach(T& object, Func func) {
		for (auto& element : object) { func(element); }
	}

	template<typename T, typename Func>
	void foreach(const T& object, Func func) {
		for (const auto& element : object) { func(element); }
	}

}